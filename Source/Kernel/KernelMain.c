#include "Include/VgaGraphics.h"
#include "Include/Interrupt.h"
#include "Include/Kbd.h"

#include "../StandardLib/Include/String.h"
#include "../StandardLib/Include/Stream.h"

int posx = 0;
int posy = 0;

void EventHandler(KeyEventArgs args) {
  char c = Kbd_GetGermanKeyMapChar(args.KeyCode, *args.Modifiers);
  if (c) {
    VGA_DrawChar(posx, posy, c, White);
    posx += 8;
  }
}



void KernelMain(void) {
  VGA_InitializeColorPalette();

  INT_InitializeIDT();
  INT_InitializePIC(0xfd, 0xff);
  INT_EnableInterrupts();

  Kbd_SetKeyDownCallback(EventHandler);



  // halt forever
  while (1) __asm__ __volatile__("hlt");
  
}

