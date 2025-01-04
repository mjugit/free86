#include "Include/FastVga.h"


/**
 * @brief Writes a single byte value to the specified VGA port.
 *
 * This function outputs an 8-bit value to a given VGA I/O port.
 * It uses inline assembly with the `outb` instruction to send
 * the specified value to the given port.
 *
 * @param port The VGA port to which the byte will be written.
 * @param value The 8-bit value to be sent.
 */
inline void _FVGA_WriteByte(uint16_t port, uint8_t value) {
  __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}


/**
 * @brief Reads a single byte from the specified VGA port.
 *
 * This function retrieves an 8-bit value from a given VGA I/O port.
 * It uses inline assembly with the `inb` instruction to read the
 * byte from the provided port.
 *
 * @param port The VGA port from which the byte will be read.
 * @return The 8-bit value read from the specified port.
 */
static inline unsigned char _FVGA_ReadByte(uint16_t port) {
  uint8_t ret;
  __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}


/**
 * @brief Sequencer index for the plane mask register.
 * @def FVGA_SEQUENCER_PLANE_MASK_INDEX
 *
 * Used to update the plane mask using the VGA sequencer ports.
 */
#define FVGA_SEQUENCER_PLANE_MASK_INDEX 0x02


/**
 * @brief Sets the VGA plane mask register.
 *
 * The plane mask determines which VGA memory planes are writable.
 *
 * @param planeMask A bitmask where each bit enables or disables
 *                  writing to a specific memory plane (0-3).
 */
void FVGA_SetPlaneMask(uint8_t planeMask) {
  _FVGA_WriteByte(FVGA_SEQUENCER_INDEX, FVGA_SEQUENCER_PLANE_MASK_INDEX);
  _FVGA_WriteByte(FVGA_SEQUENCER_DATA, planeMask);
}


/**
 * @brief Graphics controller index for the bit mask register.
 * @def FVGA_GRAPHICS_BITMASK_INDEX
 *
 * Used to update the plane mask using the VGA sequencer ports.
 */
#define FVGA_GRAPHICS_BITMASK_INDEX 0x08


/**
 * @brief Sets the VGA bit mask register.
 *
 * The bit mask determines which bits within a memory plane are affected
 * during write operations. It allows selective manipulation of pixel data
 * at a finer granularity than the plane mask.
 *
 * @param bitMask A bitmask where each bit enables or disables writing
 *                to a specific bit position (0-7) within the targeted plane.
 */
void FVGA_SetBitMask(uint8_t bitMask) {
  _FVGA_WriteByte(FVGA_GRAPHICS_INDEX, FVGA_GRAPHICS_BITMASK_INDEX);
  _FVGA_WriteByte(FVGA_GRAPHICS_DATA, bitMask);
}


/**
 * @brief Sets a palette color in the VGA DAC.
 *
 * This function defines a specific RGB color at a given palette index.
 * VGA DAC registers only accept 6-bit values for red, green, and blue components.
 *
 * @param paletteIndex The index in the VGA palette (0-255) to set.
 * @param color        The RGB color (6 bits per channel) to assign to the palette index.
 */
void FVGA_SetPaletteColor(uint8_t paletteIndex, Rgb64 color) {
  _FVGA_WriteByte(FVGA_DAC_INDEX, paletteIndex);
  _FVGA_WriteByte(FVGA_DAC_DATA, color.Red % 64);
  _FVGA_WriteByte(FVGA_DAC_DATA, color.Green % 64);
  _FVGA_WriteByte(FVGA_DAC_DATA, color.Blue % 64); 
}


/**
 * @brief Maps a logical color index to a palette color index.
 *
 * This function updates the VGA attribute controller to map a logical
 * color index (used in pixel rendering) to a specific palette index.
 *
 * @param colorIndex   The logical color index (0-15) used in rendering.
 * @param paletteIndex The palette index (0-255) to map to the logical color.
 */
void FVGA_UsePaletteColor(uint8_t colorIndex, uint8_t paletteIndex) {
  // Read from the status port to sync the VGA controller
  _FVGA_ReadByte(FVGA_STATUS);

  // Redefine color mapping
  _FVGA_WriteByte(FVGA_ATTRIBUTE_INDEX, (colorIndex & 0x1f));
  _FVGA_WriteByte(FVGA_ATTRIBUTE_INDEX, (paletteIndex & 0xff));

  // Enable video output by setting the video enable bit
  _FVGA_ReadByte(FVGA_STATUS);
  _FVGA_WriteByte(FVGA_ATTRIBUTE_INDEX, 0x20);
}


/**
 * @brief Pointer to the render buffer used for off-screen drawing.
 */
static uint8_t *_FVGA_RenderBuffer;


/**
 * @brief Pointer to VGA video memory.
 */
static uint8_t *_FVGA_VideoMemory = (uint8_t*)FVGA_VRAM_ADDRESS;


/**
 * @brief Initializes the VGA subsystem.
 *
 * Allocates memory for the render buffer, which holds the off-screen representation
 * of the video planes.
 */
void FVGA_Initialize(void) {
  _FVGA_RenderBuffer = KMem_Allocate(FVGA_PLANE_BYTES * 4);
}


/**
 * @brief Waits for the vertical synchronization (VSync) of the VGA controller.
 *
 * This function synchronizes with the VGA's vertical retrace signal by polling
 * the status register. It ensures that subsequent rendering operations are performed
 * during the VSync period to avoid visible tearing effects on the display.
 *
 * The function checks bit 3 of the VGA status register (`0x08`), which indicates
 * whether the VGA controller is currently in the vertical retrace phase.
 *
 * @note This operation is blocking and will halt execution until the retrace period starts.
 */
void FVGA_SynchronizeWithVSync(void) {
  while (!(_FVGA_ReadByte(FVGA_STATUS) & 0x08))
    ;
}


/**
 * @brief Refreshes the screen from the render buffer.
 *
 * Copies the contents of the render buffer to VGA video memory for all four planes.
 * Ensures all bits are written by setting the bit mask to 0xFF.
 */
void FVGA_RefreshScreen(void) {
  FVGA_SetBitMask(0xff);
  FVGA_SynchronizeWithVSync();
  
  for (int plane = 0; plane < 4; plane++) {
    FVGA_SetPlaneMask(1 << plane);
    uint8_t *renderBuffer = &_FVGA_RenderBuffer[FVGA_PLANE_BYTES * plane];
    KMem_Copy(_FVGA_VideoMemory, renderBuffer, FVGA_PLANE_BYTES);
  }
}


/**
 * @brief Draws a rectangle in the VGA frame buffer with a specified color.
 *
 * This function modifies the off-screen render buffer to draw a rectangle at the
 * specified position and dimensions. The rectangle is drawn by manipulating bits
 * within the appropriate memory planes to represent the given color.
 *
 * @param x      The x-coordinate of the top-left corner of the rectangle.
 * @param y      The y-coordinate of the top-left corner of the rectangle.
 * @param width  The width of the rectangle in pixels.
 * @param height The height of the rectangle in pixels.
 * @param color  The 4-bit planar VGA color to use (one bit per plane).
 *
 * @note This function only updates the render buffer. To display the changes
 *       on the screen, you need to call `FVGA_RefreshScreen()`.
 */
void FVGA_DrawRect(int x, int y, int width, int height, uint8_t color) {
  for (int plane = 0; plane < 4; plane++) {
    FVGA_SetPlaneMask(1 << plane);
    uint8_t *buffer = &_FVGA_RenderBuffer[FVGA_PLANE_BYTES * plane];

    for (int row = y; row < y + height; row++) {
      for (int col = x; col < x + width; col++) {
	int byteIndex = (row * FVGA_BYTES_X) + (col / 8);
	int bitIndex = col % 8;

	if (color & (1 << plane))
	  buffer[byteIndex] |= (1 << (7 - bitIndex));
        else
	  buffer[byteIndex] &= ~(1 << (7 - bitIndex));
      }
    }
  }
}


/**
 * @brief Internal seed for the pseudorandom number generator.
 *
 * This variable stores the current state of the XOR-Shift random number
 * generator. It is updated with each call to `_FVGA_GetRandom()`. The
 * initial value is set to `0x12345678`, but it can be reinitialized
 * with `_FVGA_SetRandomSeed()`.
 */
static uint32_t _FVGA_RandomSeed = 0x12345678;


/**
 * @brief Sets the seed for the random number generator.
 *
 * This function initializes the internal seed used by the random number
 * generator. If the provided seed is zero, a default seed (`0x12345678`)
 * will be used instead.
 *
 * @param seed The new seed for the random number generator.
 */
static void _FVGA_SetRandomSeed(uint32_t seed) {
  _FVGA_RandomSeed = seed
    ? seed
    : 0x12345678;
}


/**
 * @brief Generates a 32-bit pseudorandom number.
 *
 * This function uses a simple XOR-Shift algorithm to produce pseudorandom
 * 32-bit integers. The internal state of the generator is updated with
 * each call, ensuring that subsequent calls produce different values.
 *
 * @return A pseudorandom 32-bit unsigned integer.
 */
static uint32_t _FVGA_GetRandom(void) {
  _FVGA_RandomSeed ^= _FVGA_RandomSeed << 13;
  _FVGA_RandomSeed ^= _FVGA_RandomSeed >> 17;
  _FVGA_RandomSeed ^= _FVGA_RandomSeed << 5;

  return _FVGA_RandomSeed;
}


/**
 * @brief Generates a pseudorandom 8-bit number.
 *
 * This function returns the least significant 8 bits of a 32-bit pseudorandom
 * number generated by `_FVGA_GetRandom()`. It is useful for generating random
 * bytes in contexts where only small values are required.
 *
 * @return A pseudorandom 8-bit unsigned integer.
 */
static uint8_t _FVGA_GetRandomByte(void) {
  return (uint8_t)(_FVGA_GetRandom() & 0xff);
}


/**
 * @brief Draws a rectangle with alpha blending into the VGA render buffer.
 *
 * This function blends a rectangle onto the VGA render buffer at the specified
 * position and dimensions. The blending is performed per-pixel using a given
 * opacity level, with random dithering for opacity simulation.
 *
 * @param x        The x-coordinate of the top-left corner of the rectangle.
 * @param y        The y-coordinate of the top-left corner of the rectangle.
 * @param width    The width of the rectangle in pixels.
 * @param height   The height of the rectangle in pixels.
 * @param color    The 4-bit planar VGA color to use (one bit per plane).
 * @param opacity  The blending opacity (0 = fully transparent, 255 = fully opaque).
 *
 * @note This function only modifies the render buffer. Call `FVGA_RefreshScreen()` 
 *       to display the changes.
 */
void FVGA_DrawRectWithBlending(int x, int y, int width, int height, uint8_t color, uint8_t opacity) {
  uint32_t seed = _FVGA_GetRandom();
  
  for (int plane = 0; plane < 4; plane++) {
    _FVGA_SetRandomSeed(seed);
    FVGA_SetPlaneMask(1 << plane);
    uint8_t *buffer = &_FVGA_RenderBuffer[FVGA_PLANE_BYTES * plane];
    for (int row = y; row < y + height; row++) {
      for (int col = x; col < x + width; col++) {	
	int byteIndex = (row * FVGA_BYTES_X) + (col / 8);
	int bitIndex = (7 - (col % 8));

	if (_FVGA_GetRandomByte() > opacity)
	  continue;
	
	if (color & (1 << plane))
	  buffer[byteIndex] |= (1 << (7 - bitIndex));
        else
	  buffer[byteIndex] &= ~(1 << (7 - bitIndex));
      }
    }
  }
}


/**
 * @brief Draws a line in the VGA frame buffer using the Bresenham algorithm.
 *
 * This function draws a straight line between two points `(startX, startY)` and
 * `(endX, endY)` in the VGA render buffer. The color is applied to the relevant
 * planes based on the 4-bit planar VGA color.
 *
 * @param startX The x-coordinate of the starting point of the line.
 * @param startY The y-coordinate of the starting point of the line.
 * @param endX   The x-coordinate of the ending point of the line.
 * @param endY   The y-coordinate of the ending point of the line.
 * @param color  The 4-bit planar VGA color to use (one bit per plane).
 *
 * @note This function only modifies the render buffer. Call `FVGA_RefreshScreen()` 
 *       to display the changes.
 */
void FVGA_DrawLine(int startX, int startY, int endX, int endY, uint8_t color) {
  int deltaX = (endX > startX)
    ? (endX - startX)
    : (startX - endX);
    
  int deltaY = (endY > startY)
    ? (endY - startY)
    : (startY - endY);
    
  int stepX = (startX < endX)
    ? 1
    : -1;
    
  int stepY = (startY < endY)
    ? 1
    : -1;
    
  int error = deltaX - deltaY;

  for (int planeIndex = 0; planeIndex < 4; planeIndex++) {
    FVGA_SetPlaneMask(1 << planeIndex);
    uint8_t *planeBuffer = &_FVGA_RenderBuffer[FVGA_PLANE_BYTES * planeIndex];

    int currentX = startX;
    int currentY = startY;
    
    while (1) {
      int bufferIndex = (currentY * FVGA_BYTES_X) + (currentX / 8);
      int bitPosition = currentX % 8;

      if (color & (1 << planeIndex)) {
	planeBuffer[bufferIndex] |= (1 << (7 - bitPosition));
      } else {
	planeBuffer[bufferIndex] &= ~(1 << (7 - bitPosition));
      }

      if (currentX == endX && currentY == endY) {
	break;
      }

      int doubleError = 2 * error;
      if (doubleError > -deltaY) {
	error -= deltaY;
	currentX += stepX;
      }
      if (doubleError < deltaX) {
	error += deltaX;
	currentY += stepY;
      }
    }
  }
}




/**
 * @brief 8x8 pixel monospace font bitmap for ASCII characters.
 *
 * This array contains the 8x8 pixel bitmaps for the first 128 ASCII characters.
 * Each character is represented by 8 bytes, where each byte corresponds to
 * one row of the character. Each bit in a byte represents a single pixel:
 * - `1` indicates that the pixel is "on".
 * - `0` indicates that the pixel is "off".
 *
 * The character encoding follows the standard ASCII table:
 * - Index `0x00` corresponds to the null character (NUL).
 * - Index `0x20` corresponds to a space (' ').
 * - Index `0x41` corresponds to the letter 'A'.
 *
 * This font is commonly used for text rendering in VGA modes that support
 * character-based or planar graphics rendering.
 */
static const uint8_t _FVGA_Font8x8[128][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (;)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};


/**
 * @brief Draws a single 8x8 character onto the VGA render buffer.
 *
 * This function renders a character from a predefined 8x8 font at the specified
 * screen coordinates and color. Each pixel of the character is mapped to the VGA
 * planar memory system for accurate representation.
 *
 * @param x         The x-coordinate of the top-left corner of the character.
 * @param y         The y-coordinate of the top-left corner of the character.
 * @param character The ASCII character to render (range: 0-127).
 * @param color     The 4-bit planar VGA color to use (one bit per plane).
 *
 * @note This function only modifies the render buffer. Call `FVGA_RefreshScreen()` 
 *       to display the changes on the screen.
 */
void FVGA_DrawChar(int x, int y, char character, uint8_t color) {
    if (character < 0 || character > 127) {
        return; // Only ASCII characters 0-127 are supported
    }

    const uint8_t *bitmap = _FVGA_Font8x8[(int)character];

    for (int plane = 0; plane < 4; plane++) {
        FVGA_SetPlaneMask(1 << plane);
        uint8_t *buffer = &_FVGA_RenderBuffer[FVGA_PLANE_BYTES * plane];

        for (int row = 0; row < 8; row++) {
            uint8_t rowBitmap = bitmap[row];

            for (int col = 0; col < 8; col++) {
                int byteIndex = ((y + row) * FVGA_BYTES_X) + ((x + col) / 8);
                int bitIndex = (x + col) % 8;
                uint8_t pixelOn = (rowBitmap & (1 << col))
		  ? 1
		  : 0;

                if (pixelOn) {
                    if (color & (1 << plane))
                        buffer[byteIndex] |= (1 << (7 - bitIndex));
                    else
                        buffer[byteIndex] &= ~(1 << (7 - bitIndex));
                }
            }
        }
    }
}


/**
 * @brief Draws a null-terminated string onto the VGA render buffer.
 *
 * This function renders a string of characters using the `FVGA_DrawChar` function,
 * starting at the specified coordinates. If the string exceeds the screen width, 
 * it wraps to the next line.
 *
 * @param x       The x-coordinate of the top-left corner where the string starts.
 * @param y       The y-coordinate of the top-left corner where the string starts.
 * @param string  A null-terminated string to be drawn.
 * @param color   The 4-bit planar VGA color to use for the text.
 *
 * @note This function only modifies the render buffer. Call `FVGA_RefreshScreen()` 
 *       to display the changes on the screen.
 */
void FVGA_DrawString(int x, int y, const char *string, uint8_t color) {
  if (!string)
    return;
  
  for (const char *ptr = string; *ptr; ptr++) {
    FVGA_DrawChar(x, y, *ptr, color);

    x += 8;
    if (x >= FVGA_PIXELS_X) {
      x = 0;
      y += 8;
    }
  }
}
