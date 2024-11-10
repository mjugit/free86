#include "Include/VgaGraphics.h"
#include "Include/Interrupt.h"

#include "../StandardLib/Include/String.h"

void KernelMain(void) {
  VGA_InitializeColorPalette();

  INT_InitializeIDT();
  INT_InitializePIC(0xfd, 0xff);
  INT_EnableInterrupts();

  char buffer[100];
  short value = 4321;
  String_Format(buffer, "%dw", value);
  VGA_DrawString(0, 0, buffer, White);
  
  //VGA_DrawTestImage();
  
  // halt forever
  while (1) __asm__ __volatile__("hlt");
  
}

