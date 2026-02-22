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
#include "../Modules/Include/Memory.h"
#include "Include/Interrupt.h"
#include "Include/Keyboard.h"
#include "../Modules/GfxTk/Include/GfxTk.h"


use(Keyboard);
use(Stream);
use(Interrupt);
use(Heap);
use(Memory);
use(String);

stream inputStream;
stream outputStream;


// ----------------------------------------------------------------------
// Heap
// ----------------------------------------------------------------------

static HeapArea* _Kernel_DynMemory;

static void _InitializeHeap(void) {
  void* heapLimitBottom = (void*)0x20000;
  void* heapLimitTop = (void*)0x60000;

  U32 totalHeapSize = heapLimitTop - heapLimitBottom;
  _Kernel_DynMemory = Heap.Initialize(heapLimitBottom, totalHeapSize);
}



// ----------------------------------------------------------------------
// Interrupts
// ----------------------------------------------------------------------

static IdtEntry _Kernel_Idt[256];
static IdtDescriptor _Kernel_IdtDescriptor;



// ----------------------------------------------------------------------
// Keyboard
// ----------------------------------------------------------------------

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

  Stream.Write(&inputStream, &_KeyArgs, sizeof(KeyEventArgs));
}



extern void KShell_NotifyKeyPress(KeyEventArgs eventArgs);

void _HandleInput(void) {
  KeyEventArgs keyArgs = { };

  while (Stream.Read(&inputStream, &keyArgs, sizeof(KeyEventArgs))) {
    // Call handler in shell
    KShell_NotifyKeyPress(keyArgs);
  }
}



// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------

import(Renderer);

extern void KShell_Initialize(VgaConfig *config, HeapArea *heap);
extern void KShell_DrawLayout(VgaConfig *config);

VgaConfig vgaConfig;




void KernelMain() {
  _InitializeHeap();
  Interrupt.SetUpAll(_Kernel_Idt, &_Kernel_IdtDescriptor);

  vgaConfig = (VgaConfig) {
    .Resolution = (Vector2d) {
      .X = 640,
      .Y = 480
    },
    .PlaneCount = 4,
    .Backbuffer = Heap.Allocate(_Kernel_DynMemory, (640 / 8) * 480 * 4),
    .ScreenBuffer = (U8*)0xa0000
  };

  KShell_Initialize(&vgaConfig, _Kernel_DynMemory);

  for (;;) {
    _ProcessKeyboardInput();
    _HandleInput();

    KShell_DrawLayout(&vgaConfig);
    Renderer.Refresh(&vgaConfig);
    
   
    __asm__ __volatile__("hlt");
  }
}


