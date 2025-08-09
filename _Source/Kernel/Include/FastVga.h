#ifndef _FAST_VGA_H_
#define _FAST_VGA_H_

#include <stdint.h>

#include "KernelMemory.h"


/**
 * @brief The default horizontal screen resolution for VGA mode.
 * @def FVGA_PIXELS_X
 *
 * Indicates the number of horizontal pixels.
 */
#define FVGA_PIXELS_X 640


/**
 * @brief The bytes per screen row in the default horizontal screen resolution for VGA mode.
 * @def FVGA_BYTES_X
 *
 * Indicates the number of horizontal bytes.
 */
#define FVGA_BYTES_X ((FVGA_PIXELS_X) / 8)


/**
 * @brief The default vertical screen resolution for VGA mode.
 * @def FVGA_PIXELS_Y
 *
 * Indicates the number of vertical pixels.
 */
#define FVGA_PIXELS_Y 480

/**
 * @brief The amount of bytes per plane.
 * @def FVGA_PLANE_BYTES
 *
 * Indicates the number of bytes per plane.
 */
#define FVGA_PLANE_BYTES ((FVGA_BYTES_X) * (FVGA_PIXELS_Y))


/**
 * @brief The base address of the VGA video memory.
 * @def FVGA_VRAM_ADDRESS
 *
 * Starting memory address for VGA video framebuffer.
 */
#define FVGA_VRAM_ADDRESS 0xa0000




/**
 * @brief VGA sequencer register index port.
 * @def FVGA_SEQUENCER_INDEX
 *
 * Used to select a specific VGA sequencer register before writing data.
 */
#define FVGA_SEQUENCER_INDEX 0x3c4

/**
 * @brief VGA sequencer register data port.
 * @def FVGA_SEQUENCER_DATA
 *
 * Used to write data to the currently selected VGA sequencer register.
 */
#define FVGA_SEQUENCER_DATA 0x3c5


/**
 * @brief VGA graphics controller register index port.
 * @def FVGA_GRAPHICS_INDEX
 *
 * Used to select a VGA graphics controller register before writing data.
 */
#define FVGA_GRAPHICS_INDEX 0x3ce

/**
 * @brief VGA graphics controller register data port.
 * @def FVGA_GRAPHICS_DATA
 *
 * Used to write data to the currently selected VGA graphics controller register.
 */
#define FVGA_GRAPHICS_DATA 0x3cf


/**
 * @brief VGA attribute controller register index port.
 * @def FVGA_ATTRIBUTE_INDEX
 *
 * Used to select a VGA attribute controller register before writing data.
 */
#define FVGA_ATTRIBUTE_INDEX 0x3c0

/**
 * @brief VGA attribute controller register data port.
 * @def FVGA_ATTRIBUTE_DATA
 *
 * Used to write data to the currently selected VGA attribute controller register.
 */
#define FVGA_ATTRIBUTE_DATA 0x3c1


/**
 * @brief VGA CRT (Cathode Ray Tube) controller register index port.
 * @def FVGA_CRT_INDEX
 *
 * Used to select a VGA CRT controller register before writing data.
 */
#define FVGA_CRT_INDEX 0x3d4

/**
 * @brief VGA CRT (Cathode Ray Tube) controller register data port.
 * @def FVGA_CRT_DATA
 *
 * Used to write data to the currently selected VGA CRT controller register.
 */
#define FVGA_CRT_DATA 0x3d5


/**
 * @brief VGA DAC (Digital-to-Analog Converter) controller index port.
 * @def FVGA_DAC_INDEX
 *
 * Used to select a VGA DAC register before writing data.
 */
#define FVGA_DAC_INDEX 0x3c8

/**
 * @brief VGA DAC (Digital-to-Analog Converter) controller data port.
 * @def FVGA_DAC_DATA
 *
 * Used to write color data to the currently selected VGA DAC register.
 */
#define FVGA_DAC_DATA 0x3c9


/**
 * @brief VGA status port.
 * @def FVGA_STATUS
 *
 * Used to read the current status of the VGA subsystem (e.g., vertical retrace).
 */
#define FVGA_STATUS 0x3da


/**
 * @brief Represents a 6-bit RGB color used in VGA palettes.
 *
 * This structure holds the red, green, and blue components of a color, each
 * represented as a 6-bit value (0-63). It is primarily used for defining
 * colors in the VGA DAC palette.
 *
 * @struct Rgb64
 */
typedef struct Rgb64 {
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
} Rgb64;




/**
 * @brief Sets the VGA plane mask register.
 *
 * The plane mask determines which VGA memory planes are writable.
 *
 * @param planeMask A bitmask where each bit enables or disables
 *                  writing to a specific memory plane (0-3).
 */
extern void FVGA_SetPlaneMask(uint8_t planeMask);


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
extern void FVGA_SetBitMask(uint8_t bitMask);


/**
 * @brief Sets a palette color in the VGA DAC.
 *
 * This function defines a specific RGB color at a given palette index.
 * VGA DAC registers only accept 6-bit values for red, green, and blue components.
 *
 * @param paletteIndex The index in the VGA palette (0-255) to set.
 * @param color        The RGB color (6 bits per channel) to assign to the palette index.
 */
extern void FVGA_SetPaletteColor(uint8_t paletteIndex, Rgb64 color);


/**
 * @brief Maps a logical color index to a palette color index.
 *
 * This function updates the VGA attribute controller to map a logical
 * color index (used in pixel rendering) to a specific palette index.
 *
 * @param colorIndex   The logical color index (0-15) used in rendering.
 * @param paletteIndex The palette index (0-255) to map to the logical color.
 */
extern void FVGA_UsePaletteColor(uint8_t colorIndex, uint8_t paletteIndex);


/**
 * @brief Initializes the VGA subsystem.
 *
 * Allocates memory for the render buffer, which holds the off-screen representation
 * of the video planes.
 */
extern void FVGA_Initialize(void);


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
extern void FVGA_SynchronizeWithVSync(void);


/**
 * @brief Refreshes the screen from the render buffer.
 *
 * Copies the contents of the render buffer to VGA video memory for all four planes.
 * Ensures all bits are written by setting the bit mask to 0xFF.
 */
extern void FVGA_RefreshScreen(void);


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
extern void FVGA_DrawRect(int x, int y, int width, int height, uint8_t color);


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
extern void FVGA_DrawRectWithBlending(int x, int y, int width, int height, uint8_t color, uint8_t opacity);


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
extern void FVGA_DrawLine(int x1, int y1, int x2, int y2, uint8_t color);


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
extern void FVGA_DrawChar(int x, int y, char character, uint8_t color);


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
extern void FVGA_DrawString(int x, int y, const char *string, uint8_t color);


/**
 * @brief Sets a specific block in the VGA render buffer with the given color and bitmask.
 *
 * This function modifies a single block (byte) of the VGA render buffer, applying the specified
 * bitmask to determine which bits are affected and setting them to the provided color.
 *
 * @param blockOffset The offset (in bytes) within the render buffer to modify.
 * @param bitmask     A bitmask specifying which bits in the block should be modified (0-255).
 * @param color       The 4-bit planar VGA color to apply to the specified bits.
 *
 * @note This function directly modifies the render buffer. To display the changes, call 
 *       `FVGA_RefreshScreen()`.
 */
extern void FVGA_SetBlock(uint16_t blockOffset, uint8_t bitmask, uint8_t color);


/**
 * @brief Sets a single pixel in the VGA render buffer to the specified color.
 *
 * This function calculates the byte offset and bit position of the pixel in the VGA
 * planar memory layout and uses `FVGA_SetBlock` to update the render buffer.
 *
 * @param x     The x-coordinate of the pixel (0-based).
 * @param y     The y-coordinate of the pixel (0-based).
 * @param color The 4-bit planar VGA color to apply to the pixel.
 *
 * @note This function modifies the render buffer only. Call `FVGA_RefreshScreen()`
 *       to display the changes on the screen.
 */
extern void FVGA_SetPixel(uint16_t x, uint16_t y,  uint8_t color);

#endif
