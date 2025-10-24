/*
	
  Copyright © 2025 Maximilian Jung

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the “Software”), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
*/

#include "../Modules/Include/Heap.h"
#include "../Modules/Include/String.h"
#include "../Modules/Include/Stream.h"
#include "../Modules/Include/Gfx.h"
#include "Include/Interrupt.h"
#include "Include/Keyboard.h"


use(Keyboard);
use(Stream);
use(Interrupt);
use(Heap);
use(Memory);
use(Gfx);
use(String);



stream inputStream;
stream outputStream;


// Dynamic memory
static HeapArea* _Kernel_DynMemory;

static void _InitializeHeap(void) {
  void* heapLimitBottom = (void*)0x20000;
  void* heapLimitTop = (void*)0x60000;

  U32 totalHeapSize = heapLimitTop - heapLimitBottom;
  _Kernel_DynMemory = Heap.Initialize(heapLimitBottom, totalHeapSize);
}


// Interrupts
static IdtEntry _Kernel_Idt[256];
static IdtDescriptor _Kernel_IdtDescriptor;



KeyCode keyCode = 0;
static KeyModifiers  _KeyModifiers;
static KeyEventArgs _KeyArgs;

static void _ProcessKeyboardInput(void) {
  U16 scanCode = Keyboard.ReadScanCode();
  if (!scanCode)
    return;
  
  bool wasKeyDown = (scanCode & 0xFF) & 0x80;
  U8 keyCode = Keyboard.GetKeyCode(wasKeyDown ? (scanCode - 0x80) : scanCode);
  Keyboard.UpdateModifiers(keyCode, wasKeyDown, &_KeyModifiers);

  _KeyArgs = (KeyEventArgs) {
    .KeyCode = keyCode,
    .WasKeyPress = wasKeyDown,
    .Modifiers = &_KeyModifiers,
    .Handled = false
  };

  char ch = Keyboard.GetChar(keyCode, _KeyModifiers);
  if (wasKeyDown && ch)
    Stream.Write(&inputStream, &ch, 1);
}





#define screenSize (80 * 40)
char screenBuffer[screenSize];
char* screenPtr;

void _HandleInput(void) {
  U32 bytesFree = screenBuffer + sizeof(screenBuffer) - screenPtr;

  if (!bytesFree) {
    Memory.Move(screenBuffer + 80, screenBuffer, 80);
    bytesFree = 80;
  }
  
  U32 bytesRead = Stream.Read(&inputStream, screenPtr, bytesFree);
  screenPtr += bytesRead;
}


char textBuffer[256] = { };
void KernelMain() {
  _InitializeHeap();

  Interrupt.SetUpAll(_Kernel_Idt, &_Kernel_IdtDescriptor);

  screenPtr = screenBuffer;
  
  if (Gfx.Core.Initialize(640, 480, 4, _Kernel_DynMemory))
    Gfx.Core.Refresh();

  for (;;) {
    String.Format(textBuffer, "Used  = %d\nFree  = %d\nTotal = %d",
		  _Kernel_DynMemory->TotalBytesUsed,
		  _Kernel_DynMemory->TotalBytesFree,
		  _Kernel_DynMemory->TotalBytes);
    
    _ProcessKeyboardInput();
    _HandleInput();

    Gfx.Draw.String(0, 0, textBuffer, 0x7, Courier);
    Gfx.Draw.String(0, 30, screenBuffer, 0xf, Courier);
    Gfx.Core.Refresh();
   
    __asm__ __volatile__("hlt");
  }
}


