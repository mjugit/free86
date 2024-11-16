#include "Include/VgaGraphics.h"
#include "Include/Interrupt.h"
#include "Include/Keyboard.h"

#include "../StandardLib/Include/String.h"
#include "../StandardLib/Include/Stream.h"

void KernelMain(void) {
  VGA_InitializeColorPalette();

  INT_InitializeIDT();
  INT_InitializePIC(0xfd, 0xff);
  Stream_Initialize(&InputStream);
  INT_EnableInterrupts();

  // VGA_DrawTestImage();
  short posx = 0;
  short posy = 0;

  Charset ger = Keyboard_GetGermanCharset();
  Stream input;

  Stream_Initialize(&input);
  Keyboard_SetInputStream(&input);
  Keyboard_SetCharset(&ger);

  char c = 0;
  do {
    if (!Stream_Read(&input, &c))
      continue;

    VGA_DrawChar(posx, posy, c, White);
    if (c == '\b') {
      posx -= 8;
      continue;
    }
    
    posx += 8;
    if (posx >= 640 || c == '\n') {
      posy += 8;
      posx = 0;
    }
  } while (true);



  // halt forever
  while (1) __asm__ __volatile__("hlt");
  
}

