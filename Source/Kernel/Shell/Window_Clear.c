#include "../Include/Shell.h"

void Window_Clear(Window *window) {
  uint16_t contentAreaStartX = window->LocationX + WIN_BORDER_WIDTH;
  uint16_t contentAreaSizeX = window->SizeX - (WIN_BORDER_WIDTH * 2);
  uint16_t contentAreaStartY = window->LocationY + WIN_HEADER_HEIGHT + WIN_MENU_HEIGHT;
  uint16_t contentAreaHeight = window->SizeY + window->LocationY - contentAreaStartY;

  VGA_DrawRect(contentAreaStartX, contentAreaStartY, contentAreaSizeX, contentAreaHeight, White);
}
