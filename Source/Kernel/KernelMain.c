#include "Include/Interrupt.h"
#include "Include/Kbd.h"
#include "Include/Shell.h"
#include "Include/KernelMemory.h"
#include "Include/FastVga.h"

#include "../StandardLib/Include/String.h"
#include "../StandardLib/Include/Stream.h"

extern uint16_t SystemLowMemoryKiB;

void _Kernel_InitializeInterrupts(void) {
  INT_InitializeIDT();
  INT_InitializePIC(0xfd, 0xff);
  INT_EnableInterrupts();  
}

void _Kernel_InitializeHeap(void) {
  void *heapStart = (void*)0xf000;
  KMem_Initialize(heapStart, (SystemLowMemoryKiB - 64) * 1024);
}


void KernelMain(void) {
  _Kernel_InitializeInterrupts();
  _Kernel_InitializeHeap();

  FVGA_Initialize();

  const uint8_t BLUE = 1;
  const uint8_t GREEN = 2;
  const uint8_t RED = 4;

  FVGA_SetPaletteColor(RED, (Rgb64) { 63, 30, 30 });
  FVGA_SetPaletteColor(BLUE, (Rgb64) { 30, 30, 63 });
  FVGA_SetPaletteColor(GREEN, (Rgb64) { 30, 63, 30 });
  
  FVGA_UsePaletteColor(RED, RED);
  FVGA_UsePaletteColor(BLUE, BLUE);
  FVGA_UsePaletteColor(GREEN, GREEN);
  
  FVGA_DrawRect(0, 0, 640, 480, RED);
  for (uint16_t y = 0; y < 480; y++) {
    FVGA_DrawRectWithBlending(0, y, 320, 1, BLUE, (y / 2) % 255);
  }

  for (uint16_t y = 0; y < 480; y++) {
    FVGA_DrawRectWithBlending(320, y, 320, 1, GREEN, (y / 2) % 255);
  }
  
  FVGA_RefreshScreen();
  
  // halt forever
  while (1) __asm__ __volatile__("hlt");
  
}

