#include "../Include/Shell.h"

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
void Window_Clear(Window *window) {
  uint16_t contentAreaStartX = window->LocationX + WIN_BORDER_WIDTH;
  uint16_t contentAreaSizeX = window->SizeX - (WIN_BORDER_WIDTH * 2);
  uint16_t contentAreaStartY = window->LocationY + WIN_HEADER_HEIGHT + WIN_MENU_HEIGHT;
  uint16_t contentAreaHeight = window->SizeY + window->LocationY - contentAreaStartY - WIN_BORDER_WIDTH;

  VGA_SetRect(contentAreaStartX, contentAreaStartY, contentAreaSizeX, contentAreaHeight, White);
}
