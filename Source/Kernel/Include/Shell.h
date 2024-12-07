#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdint.h>

#include "Kbd.h"
#include "VgaGraphics.h"


#define WIN_BORDER_WIDTH 2

#define WIN_HEADER_HEIGHT 12
#define WIN_MENU_HEIGHT 10



typedef struct _win {
  uint16_t LocationX;
  uint16_t LocationY;
  uint16_t SizeX;
  uint16_t SizeY;
  uint16_t CursorPosition;
  
  KeyEventHandler OnKeyDown;
  KeyEventHandler OnKeyUp;

  char Title[50];
  char InputBuffer[256];
} Window;



extern void Window_DrawBorder(Window *window);
extern void Window_DrawMenu(Window *window);
extern void Window_Clear(Window *window);

#endif
