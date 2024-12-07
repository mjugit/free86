#include "../Include/Shell.h"

static const color_t BorderColor = Blue;

void Window_DrawBorder(Window *window) {
  uint16_t headerStartX = window->LocationX;
  uint16_t headerSizeX = window->SizeX;
  uint16_t headerStartY = window->LocationY;
  
  VGA_DrawRect(headerStartX, headerStartY, headerSizeX, WIN_HEADER_HEIGHT, Blue);
  VGA_DrawString(headerStartX + 5, headerStartY + 2, window->Title, White);

  uint16_t borderLeftTopX = window->LocationX;
  uint16_t borderLeftTopY = window->LocationY + WIN_HEADER_HEIGHT;
  uint16_t borderLeftSizeY = window->SizeY - WIN_HEADER_HEIGHT;
  
  VGA_DrawRect(borderLeftTopX, borderLeftTopY, WIN_BORDER_WIDTH, borderLeftSizeY, BorderColor);

  uint16_t borderRightTopX = window->LocationX + window->SizeX - WIN_BORDER_WIDTH;
  uint16_t borderRightTopY = window->LocationY + WIN_HEADER_HEIGHT;
  uint16_t borderRightSizeY = window->SizeY - WIN_HEADER_HEIGHT;
  
  VGA_DrawRect(borderRightTopX, borderRightTopY, WIN_BORDER_WIDTH, borderRightSizeY, BorderColor);

  uint16_t borderBottomTopX = window->LocationX + WIN_BORDER_WIDTH;
  uint16_t borderBottomTopY = window->SizeY + window->LocationY - WIN_BORDER_WIDTH;
  uint16_t borderBottomSizeX = window->SizeX - (WIN_BORDER_WIDTH * 2);

  VGA_DrawRect(borderBottomTopX, borderBottomTopY, borderBottomSizeX, WIN_BORDER_WIDTH, BorderColor);
}
