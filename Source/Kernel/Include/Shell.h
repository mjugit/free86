#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdint.h>

#include "Kbd.h"
#include "VgaGraphics.h"


/**
 * @brief Defines the width of a window's border in pixels.
 *
 * This constant specifies the thickness of the border drawn around a window.
 * The value is used by window-drawing functions to maintain consistent
 * visual design.
 */
#define WIN_BORDER_WIDTH 2


/**
 * @brief Defines the height of a window's header in pixels.
 *
 * This constant specifies the vertical space allocated for the header
 * of a window, which typically displays the title or other information.
 */
#define WIN_HEADER_HEIGHT 12


/**
 * @brief Defines the height of a window's menu in pixels.
 *
 * This constant specifies the vertical space allocated for the menu
 * of a window, which typically contains actionable items or options.
 */
#define WIN_MENU_HEIGHT 10




/**
 * @brief Represents a graphical window structure.
 *
 * The `Window` structure encapsulates all the necessary attributes
 * and event handlers for managing a graphical window in the system.
 *
 * Fields:
 * - `LocationX` and `LocationY`: The top-left position of the window on the screen.
 * - `SizeX` and `SizeY`: The dimensions of the window in pixels.
 * - `CursorPosition`: The current cursor position within the window.
 * - `OnKeyDown` and `OnKeyUp`: Event handlers for key press and release events.
 * - `Title`: The title of the window (maximum 50 characters).
 * - `InputBuffer`: A buffer for storing user input (maximum 256 characters).
 */
typedef struct _win {
    uint16_t LocationX;       ///< The X-coordinate of the window's top-left corner.
    uint16_t LocationY;       ///< The Y-coordinate of the window's top-left corner.
    uint16_t SizeX;           ///< The width of the window in pixels.
    uint16_t SizeY;           ///< The height of the window in pixels.
    uint16_t CursorPosition;  ///< The current cursor position in the window.

    KeyEventHandler OnKeyDown; ///< Callback for key press events.
    KeyEventHandler OnKeyUp;   ///< Callback for key release events.

    char Title[50];           ///< The title displayed in the window's header.
    char InputBuffer[256];    ///< A buffer to store user input.
} Window;




/**
 * @brief Draws the border and header for a specified window.
 *
 * This function renders the header and border of a window based on its
 * position and dimensions. The header is filled with a predefined color,
 * and the window's title is displayed in white text. The border surrounds
 * the content area with a consistent thickness.
 *
 * @param window A pointer to the `Window` structure representing the window to be drawn.
 */
extern void Window_DrawBorder(Window *window);


/**
 * @brief Draws the menu area of a specified window.
 *
 * This function renders the menu section of a window, located just below
 * the window header. The menu is filled with a predefined background color,
 * ensuring consistent visual styling.
 *
 * @param window A pointer to the `Window` structure representing the window
 *               whose menu area is to be drawn.
 */
extern void Window_DrawMenu(Window *window);


/**
 * @brief Clears the content area of a specified window.
 *
 * This function erases all content within the defined content area of a window,
 * leaving the border, header, and menu intact. The content area is filled with
 * the color white to reset its state.
 *
 * The function calculates the content area based on the window's location,
 * size, and predefined constants for border, header, and menu dimensions.
 *
 * @param window A pointer to the `Window` structure representing the window to be cleared.
 */
extern void Window_Clear(Window *window);


#endif
