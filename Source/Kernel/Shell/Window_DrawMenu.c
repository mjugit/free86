#include "../Include/Shell.h"

static const color_t MenuBackgroundColor = LightYellow;

void Window_DrawMenu(Window *window) {
  uint16_t menuStartX = window->LocationX + WIN_BORDER_WIDTH;
  uint16_t menuSizeX = window->SizeX - (WIN_BORDER_WIDTH * 2);
  uint16_t menuStartY = window->LocationY + WIN_HEADER_HEIGHT;
  
  VGA_DrawRect(menuStartX, menuStartY, menuSizeX, WIN_MENU_HEIGHT, LightYellow);
}
