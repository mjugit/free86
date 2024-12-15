#include "Include/VgaGraphics.h"
#include "Include/Interrupt.h"
#include "Include/Kbd.h"
#include "Include/Shell.h"

#include "../StandardLib/Include/String.h"
#include "../StandardLib/Include/Stream.h"

int posx = 0;
int posy = 0;

void EventHandler(KeyEventArgs args) {
  
  if (args.KeyCode == KEY_DOWN) {
    if (posy < 480 / 8)
      posy +=8;
    return;
  }

  if (args.KeyCode == KEY_UP) {
    if (posy >= 8)
      posy -=8;
    return;
  }
  
  char c = Kbd_GetGermanKeyMapChar(args.KeyCode, *args.Modifiers);
  
  if (c) {
    VGA_SetBlock(posx, posy, Black);
    VGA_PrintChar(posx, posy, c, White);
    posx += 8;
  }
  
}



void KernelMain(void) {
  VGA_InitializeColorPalette();

  INT_InitializeIDT();
  INT_InitializePIC(0xfd, 0xff);
  INT_EnableInterrupts();

  Kbd_SetKeyDownCallback(EventHandler);

  Window myWin = {
    .LocationX = 20,
    .LocationY = 20,
    .SizeX = 300,
    .SizeY = 200,
    .Title = "A window!"
  };
  
  Window_DrawBorder(&myWin);
  Window_Clear(&myWin);
  Window_DrawMenu(&myWin);

  Window myWin2 = {
    .LocationX = 120,
    .LocationY = 120,
    .SizeX = 400,
    .SizeY = 200,
    .Title = "Another one!"
  };

  
  Window_DrawBorder(&myWin2);
  Window_Clear(&myWin2);
  Window_DrawMenu(&myWin2);
  
  // halt forever
  while (1) __asm__ __volatile__("hlt");
  
}

