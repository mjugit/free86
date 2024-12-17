#include "Include/VgaGraphics.h"
#include "Include/Interrupt.h"
#include "Include/Kbd.h"
#include "Include/Shell.h"
#include "Include/KernelMemory.h"

#include "../StandardLib/Include/String.h"
#include "../StandardLib/Include/Stream.h"

extern uint16_t SystemLowMemoryKiB;


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

  KMem_Initialize((void*)0x7000, (SystemLowMemoryKiB - 28) * 1024);
  Kbd_SetKeyDownCallback(EventHandler);


  VGA_DrawTestImage();
  
  // halt forever
  while (1) __asm__ __volatile__("hlt");
  
}

