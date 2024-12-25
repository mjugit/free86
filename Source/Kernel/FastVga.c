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

	if (color & (1 << plane)) {
	  buffer[byteIndex] |= (1 << (7 - bitIndex));
	} else {
	  buffer[byteIndex] &= ~(1 << (7 - bitIndex));
	}
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
    for (int plane = 0; plane < 4; plane++) {
        FVGA_SetPlaneMask(1 << plane);
        uint8_t *buffer = &_FVGA_RenderBuffer[FVGA_PLANE_BYTES * plane];

        for (int row = y; row < y + height; row++) {
            for (int col = x; col < x + width; col++) {
                int byteIndex = (row * FVGA_BYTES_X) + (col / 8);
                int bitIndex = col % 8;

                uint8_t foregroundBit = (color & (1 << plane)) ? 1 : 0;
                uint8_t backgroundBit = (buffer[byteIndex] & (1 << (7 - bitIndex))) ? 1 : 0;

                uint8_t randomThreshold = _FVGA_GetRandomByte() % 255;
                if (randomThreshold < opacity) {
                    uint8_t blendedBit = (foregroundBit * opacity + backgroundBit * (255 - opacity)) / 255;

                    if (blendedBit) {
                        buffer[byteIndex] |= (1 << (7 - bitIndex));
                    } else {
                        buffer[byteIndex] &= ~(1 << (7 - bitIndex));
                    }
                }
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





