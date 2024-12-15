#include "../Include/Shell.h"

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
void Window_DrawMenu(Window *window) {
    // Calculate the menu's position and size
    uint16_t menuStartX = window->LocationX + WIN_BORDER_WIDTH;
    uint16_t menuSizeX = window->SizeX - (WIN_BORDER_WIDTH * 2);
    uint16_t menuStartY = window->LocationY + WIN_HEADER_HEIGHT;

    // Draw the menu background
    VGA_SetRect(menuStartX, menuStartY, menuSizeX, WIN_MENU_HEIGHT, LightYellow);
}

