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

  
  Kbd_SetKeyDownCallback(EventHandler);
  Window myWin = {
    .LocationX = 40,
    .LocationY = 40,
    .SizeX = 300,
    .SizeY = 200,
    .Title = "A window!"
  };
  
  Window_DrawBorder(&myWin);
  Window_Clear(&myWin);
  Window_DrawMenu(&myWin);
  
  KMem_Initialize((void*)0x7000, (SystemLowMemoryKiB - 28) * 1024);
  char buff[100];
  uint32_t bytesFree = KMem_GetBytesFree();
  String_Format(buff, "%d Bytes free", bytesFree);
  VGA_PrintString(0, 0, buff, Red);

  void *ptr = KMem_Allocate(123);
  bytesFree = KMem_GetBytesFree();
  String_Format(buff, "%d Bytes free", bytesFree);
  VGA_PrintString(0, 8, buff, Red);

  KMem_Free(ptr);
  bytesFree = KMem_GetBytesFree();
  String_Format(buff, "%d Bytes free", bytesFree);
  VGA_PrintString(0, 16, buff, Red);
  
  // halt forever
  while (1) __asm__ __volatile__("hlt");
  
}

