#include "Include/VgaGraphics.h"
#include "Include/Interrupt.h"

void KernelMain(void) {
  VGA_InitializeColorPalette();

  INT_InitializeIDT();
  INT_InitializePIC(0xfd, 0xff);
  INT_EnableInterrupts();

  // halt forever
  while (1) __asm__ __volatile__("hlt");
  
}
