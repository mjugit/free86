#ifndef _VGA_GRAPHICS_H_
#define _VGA_GRAPHICS_H_

#include <stdint.h>


/**
 * @brief VGA sequencer register index port.
 * @def VGA_SEQUENCER_INDEX
 *
 * Used to select a specific VGA sequencer register before writing data.
 */
#define VGA_SEQUENCER_INDEX 0x3c4

/**
 * @brief VGA sequencer register data port.
 * @def VGA_SEQUENCER_DATA
 *
 * Used to write data to the currently selected VGA sequencer register.
 */
#define VGA_SEQUENCER_DATA 0x3c5


/**
 * @brief VGA graphics controller register index port.
 * @def VGA_GRAPHICS_INDEX
 *
 * Used to select a VGA graphics controller register before writing data.
 */
#define VGA_GRAPHICS_INDEX 0x3ce

/**
 * @brief VGA graphics controller register data port.
 * @def VGA_GRAPHICS_DATA
 *
 * Used to write data to the currently selected VGA graphics controller register.
 */
#define VGA_GRAPHICS_DATA 0x3cf


/**
 * @brief VGA attribute controller register index port.
 * @def VGA_ATTRIBUTE_INDEX
 *
 * Used to select a VGA attribute controller register before writing data.
 */
#define VGA_ATTRIBUTE_INDEX 0x3c0

/**
 * @brief VGA attribute controller register data port.
 * @def VGA_ATTRIBUTE_DATA
 *
 * Used to write data to the currently selected VGA attribute controller register.
 */
#define VGA_ATTRIBUTE_DATA 0x3c1


/**
 * @brief VGA CRT (Cathode Ray Tube) controller register index port.
 * @def VGA_CRT_INDEX
 *
 * Used to select a VGA CRT controller register before writing data.
 */
#define VGA_CRT_INDEX 0x3d4

/**
 * @brief VGA CRT (Cathode Ray Tube) controller register data port.
 * @def VGA_CRT_DATA
 *
 * Used to write data to the currently selected VGA CRT controller register.
 */
#define VGA_CRT_DATA 0x3d5


/**
 * @brief VGA DAC (Digital-to-Analog Converter) controller index port.
 * @def VGA_DAC_INDEX
 *
 * Used to select a VGA DAC register before writing data.
 */
#define VGA_DAC_INDEX 0x3c8

/**
 * @brief VGA DAC (Digital-to-Analog Converter) controller data port.
 * @def VGA_DAC_DATA
 *
 * Used to write color data to the currently selected VGA DAC register.
 */
#define VGA_DAC_DATA 0x3c9


/**
 * @brief VGA status port.
 * @def VGA_STATUS
 *
 * Used to read the current status of the VGA subsystem (e.g., vertical retrace).
 */
#define VGA_STATUS 0x3da


/**
 * @brief The default horizontal screen resolution for VGA mode.
 * @def VGA_SCREEN_X
 *
 * Indicates the number of horizontal pixels.
 */
#define VGA_SCREEN_X 640

/**
 * @brief The default vertical screen resolution for VGA mode.
 * @def VGA_SCREEN_Y
 *
 * Indicates the number of vertical pixels.
 */
#define VGA_SCREEN_Y 480


/**
 * @brief The base address of the VGA video memory.
 * @def VIDEO_MEMORY_ADDRESS
 *
 * Starting memory address for VGA video framebuffer.
 */
#define VIDEO_MEMORY_ADDRESS 0xa0000




/**
 * @brief Enumeration of VGA standard colors.
 *
 * This enum defines the 16 standard VGA colors, each associated with an index
 * in the VGA palette. These colors are used for text rendering, shapes, and
 * other graphical elements in VGA modes.
 */
typedef enum {
  Black = 0,       ///< Black color.
  Red = 1,         ///< Red color.
  Green = 2,       ///< Green color.
  Yellow = 3,      ///< Yellow color.
  Blue = 4,        ///< Blue color.
  Purple = 5,      ///< Purple (magenta) color.
  Cyan = 6,        ///< Cyan (light blue-green) color.
  LightGray = 7,   ///< Light gray color.
  DarkGray = 8,    ///< Dark gray color.
  LightRed = 9,    ///< Light red color.
  LightGreen = 10, ///< Light green color.
  LightYellow = 11,///< Light yellow color.
  LightBlue = 12,  ///< Light blue color.
  LightPurple = 13,///< Light purple color.
  LightCyan = 14,  ///< Light cyan color.
  White = 15       ///< White color.
} color_t;




/**
 * @brief Sets the VGA plane mask register.
 *
 * The plane mask determines which VGA memory planes are writable.
 *
 * @param planeMask A bitmask where each bit enables or disables
 *                  writing to a specific memory plane (0-3).
 */
extern void VGA_SetPlaneMask(uint8_t plane_mask);

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
extern void VGA_SetBitMask(uint8_t bit_mask);

/**
 * @brief Sets a color in the VGA palette.
 *
 * This function updates a specific color in the VGA Digital-to-Analog Converter (DAC) palette.
 * VGA palette colors are defined using 6-bit values for each of the red, green, and blue channels.
 *
 * @param index The palette index (0-255) to be updated.
 * @param red64 The intensity of the red channel (0-63).
 * @param green64 The intensity of the green channel (0-63).
 * @param blue64 The intensity of the blue channel (0-63).
 */
extern void VGA_SetPaletteColor(uint8_t index, uint8_t red64, uint8_t green64, uint8_t blue64);

/**
 * @brief Updates the color mapping between the 16-color palette and the 256 colors available in VGA.
 *
 * This function assigns a specific color from the 256-color VGA palette to one of the 16 hardware
 * palette entries used for indexed color modes. It enables and configures the VGA attribute controller
 * for this purpose.
 *
 * @param color The 5-bit index (0-31) of the hardware palette entry to redefine.
 * @param paletteIndex The 8-bit index (0-255) of the color in the VGA palette to assign.
 *
 * @remark The calls to `_VGA_ReadByte()` are necessary to synchronize the VGA controller
 *         and ensure the attribute controller is ready for configuration.
 */
extern void VGA_PickColor(color_t color, uint8_t paletteIndex);


/**
 * @brief Sets a block of pixels in VGA planar graphics mode.
 *
 * This function modifies a block of pixels at a specified memory offset.
 * It uses a bitmask to determine which bits (pixels) within a byte should
 * be affected and assigns them a specified color. The function handles
 * both clearing and setting the pixels in a memory-efficient manner.
 *
 * @param offset The memory offset in the VGA framebuffer where the block resides.
 * @param bitMask A bitmask indicating which pixels (bits) in the byte should be modified.
 *                Each bit corresponds to a pixel in planar graphics mode.
 * @param color The VGA plane mask (0-15) that determines which color planes are affected.
 *              This effectively sets the pixel color.
 */
extern void VGA_SetBlock(uint16_t offset, uint8_t bitMask, color_t color);


/**
 * @brief Sets a single pixel to a specified color in VGA planar graphics mode.
 *
 * This function calculates the memory location and bit position of a pixel
 * based on its (x, y) coordinates and sets it to the specified color. It uses
 * `VGA_SetBlock` to apply the appropriate bitmask and color.
 *
 * @param x The horizontal coordinate of the pixel (0 to VGA_SCREEN_X - 1).
 * @param y The vertical coordinate of the pixel (0 to VGA_SCREEN_Y - 1).
 * @param color The VGA plane mask (0-15) that determines the color of the pixel.
 */
extern void VGA_SetPixel(uint16_t x, uint16_t y, color_t color);


/**
 * @brief Draws a filled rectangle in VGA planar graphics mode.
 *
 * This function renders a filled rectangle of a given size and color,
 * starting at a specific (x, y) position. It calculates the necessary
 * bitmasks for bytes at the rectangle's edges and ensures proper alignment
 * for non-byte-aligned coordinates.
 *
 * @param startx The horizontal coordinate of the rectangle's top-left corner.
 * @param starty The vertical coordinate of the rectangle's top-left corner.
 * @param sizex The width of the rectangle in pixels.
 * @param sizey The height of the rectangle in pixels.
 * @param color The VGA plane mask (0-15) that determines the color of the rectangle.
 */
extern void VGA_SetRect(uint16_t startx, uint16_t starty, uint16_t sizex, uint16_t sizey, color_t color);


/**
 * @brief Draws a single character on the screen in a specific color.
 *
 * This function renders a character from the monospace font at a specified
 * (x, y) position on the screen. It retrieves the 8x8 bitmap for the character
 * and uses the `VGA_DrawPixel` function to draw each pixel in the specified color.
 *
 * @param startx The horizontal coordinate of the top-left corner of the character.
 * @param starty The vertical coordinate of the top-left corner of the character.
 * @param character The ASCII character to be drawn.
 * @param color The VGA plane mask (0-15) that determines the color of the character.
 *
 * @note Ensure that `startx` and `starty` are within the screen bounds to avoid
 *       out-of-bounds memory access.
 */
extern void VGA_PrintChar(uint16_t startx, uint16_t starty, const char character, color_t color);


/**
 * @brief Draws a string on the screen in a specific color.
 *
 * This function renders a null-terminated string starting at the specified
 * (x, y) position. Each character is drawn using the `VGA_DrawChar` function,
 * and characters are spaced according to the monospace font's width.
 *
 * @param startx The horizontal coordinate of the top-left corner of the string.
 * @param starty The vertical coordinate of the top-left corner of the string.
 * @param text A pointer to the null-terminated string to be drawn.
 * @param color The VGA plane mask (0-15) that determines the color of the string.
 *
 * @note Ensure that the string fits within the screen bounds. Line wrapping is
 *       not handled by this function.
 */
extern void VGA_PrintString(uint16_t startx, uint16_t starty, const char *text, color_t color);


/**
 * @brief Draws a horizontal line on the screen.
 *
 * This function draws a straight horizontal line starting from the specified
 * (x, y) position, extending for the given width in pixels, and using the specified color.
 *
 * @param startx The horizontal starting position of the line.
 * @param starty The vertical position of the line.
 * @param sizex The width of the line in pixels.
 * @param color The VGA plane mask (0-15) that determines the color of the line.
 */
extern void VGA_DrawHorizontalLine(uint16_t startx, uint16_t starty, uint16_t sizex, color_t color);


/**
 * @brief Draws a vertical line on the screen.
 *
 * This function draws a straight vertical line starting from the specified
 * (x, y) position, extending for the given height in pixels, and using the specified color.
 *
 * @param startx The horizontal position of the line.
 * @param starty The vertical starting position of the line.
 * @param sizey The height of the line in pixels.
 * @param color The VGA plane mask (0-15) that determines the color of the line.
 */
extern void VGA_DrawVerticalLine(uint16_t startx, uint16_t starty, uint16_t sizey, color_t color);


/**
 * @brief Initializes the default system color palette.
 *
 * This function configures the VGA palette with a predefined set of 16 colors.
 * It assigns RGB values to the 256-color VGA palette and maps the first 16
 * palette entries to standard color names (e.g., Black, Red, Green).
 * These colors are commonly used in text mode and graphical interfaces.
 *
 * The colors are initialized using `VGA_SetPaletteColor` to define the RGB values
 * and `VGA_PickColor` to map them to the 16-color hardware palette.
 */
extern void VGA_InitializeColorPalette(void);


/**
 * @brief Draws a test image to validate VGA drawing functionality.
 *
 * This function creates a test pattern consisting of two rows of colored rectangles
 * and displays text in all 16 colors of the VGA palette. It is intended for verifying
 * that colors, shapes, and text rendering work as expected in VGA mode.
 *
 * The test image includes:
 * - 16 rectangles (8 per row), each 45x45 pixels, drawn in sequential VGA colors.
 * - Two lines of text in each of the 16 colors, showcasing uppercase and lowercase letters.
 */
extern void VGA_DrawTestImage(void);

#endif
