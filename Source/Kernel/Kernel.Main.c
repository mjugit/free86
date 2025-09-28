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
#include "../Modules/Include/Bitmap.h"
#include "../Modules/Include/String.h"
#include "../Modules/Include/Stream.h"

#include "../Modules/Include/Gfx.h"

#include "Include/Interrupt.h"
#include "Include/Keyboard.h"
#include "Include/ColorTheme.h"


use(Bitmap);
use(Stream);
use(Heap);
use(String);
use(Keyboard);
use(GfxCore);
use(GfxDraw);


// These are populated by the second stage bootloader
extern U16 _Kernel_LowMemoryInfoKiB;
extern U16 _Kernel_HighMemoryInfoKiB;
extern U16 _Kernel_EquipmentWord;



// Dynamic memory
static HeapArea* _Kernel_DynMemory;

static void _InitializeHeap(void) {
  void* heapLimitBottom = (void*)0x10000;
  void* heapLimitTop = (void*)0x60000;

  U32 totalHeapSize = heapLimitTop - heapLimitBottom;
  _Kernel_DynMemory = Heap.Initialize(heapLimitBottom, totalHeapSize);
}


// Interrupts
static IdtEntry _Kernel_Idt[256];
static IdtDescriptor _Kernel_IdtDescriptor;


// Equipment
static U16 _Equipment_FloppyDriveCount;
static U16 _Equipment_RS232Count;
static bool _Equipment_HasFpu;

static void _InitializeEquipmentInfo(void) {
  _Equipment_FloppyDriveCount = ((_Kernel_EquipmentWord >> 6) & 0x3) + 1;
  _Equipment_RS232Count	      = (_Kernel_EquipmentWord >> 9) & 0x7;
  _Equipment_HasFpu	      = (_Kernel_EquipmentWord >> 1) & 0x1;
}




KeyCode keyCode = 0;
static KeyModifiers  _KeyModifiers;
static KeyEventArgs _KeyArgs;

static stream inputStream;

static void KeyboardHandler(void) {
  U16 scanCode = Keyboard.ReadScanCode();
  if (!scanCode)
    return;

  bool wasKeyDown = (scanCode & 0xFF) & 0x80;
  U8 keyCode = Keyboard.GetKeyCode(wasKeyDown ? (scanCode - 0x80) : scanCode);
  Keyboard.UpdateModifiers(keyCode, wasKeyDown, &_KeyModifiers);

  _KeyArgs = (KeyEventArgs) {
    .KeyCode = keyCode,
    .WasKeyPress = wasKeyDown,
    .Modifiers = &_KeyModifiers
  };

  char character = Keyboard.GetChar(keyCode, _KeyModifiers);
  if (character && _KeyArgs.WasKeyPress)
    Stream.Write(&inputStream, &character, 1);
}

/* Naked attribute: no prolog/epilog */
__attribute__((naked))
void IrqKeyboard_Handler(void) {
    PUSHA

    KeyboardHandler();
    
    EOI
}


U32 uptimeSeconds = 0;
U16 timerCounter = 0;
__attribute__((naked))
void IrqTimer_Handler(void) {
  PUSHA

  timerCounter = timerCounter + 1;
  if (timerCounter == 100) {
    uptimeSeconds++;
    timerCounter = 0;
  }
    
  EOI
}

static void InitializeTimer() {
    U16 divisor = 11932; /* ≈ 100 Hz */

    PortWriteByte(0x43, 0x36);

    PortWriteByte(0x40, (U8)(divisor & 0xFF));
    PortWriteByte(0x40, (U8)(divisor >> 8));

    U8 timerHandlerFlags = Idt_EncodeFlags(IdtGateTypeInt32, IdtPrivilegeKernel, true);
    Idt_SetGate(&_Kernel_Idt[0x20], (U32)IrqTimer_Handler, 0x08,  timerHandlerFlags);
}

static void InitializeKeyboard() {
  U8 keyboardHandlerFlags = Idt_EncodeFlags(IdtGateTypeInt32, IdtPrivilegeKernel, true);
  Idt_SetGate(&_Kernel_Idt[0x21], (U32)IrqKeyboard_Handler, 0x08, keyboardHandlerFlags);
}

/* Minimal interrupt init */
static void _InitializeInterrupts() {
    DisableInterrupts();

    /* Remap PIC */
    PortWriteByte(0x20, 0x11);
    PortWriteByte(0xA0, 0x11);
    PortWriteByte(0x21, 0x20); /* Master offset = 32 */
    PortWriteByte(0xA1, 0x28); /* Slave offset = 40 */
    PortWriteByte(0x21, 0x04);
    PortWriteByte(0xA1, 0x02);

    PortWriteByte(0x21, 0x01);
    PortWriteByte(0xA1, 0x01);

    PortWriteByte(0x21, 0xfc);
    PortWriteByte(0xA1, 0xFF);


    
    InitializeTimer();
    InitializeKeyboard();
    
    Idt_Load(_Kernel_Idt, &_Kernel_IdtDescriptor);

    EnableInterrupts();
}
 

void KernelMain() {
  _InitializeHeap();
  _InitializeInterrupts();
  _InitializeEquipmentInfo();


  if (GfxCore.Initialize(640, 480, 4, _Kernel_DynMemory))
    GfxCore.Refresh();


  char textBuff[100] = { };
  
  while (1) {
    GfxDraw.FilledRect(0, 0, 639, 479, 0);

    GfxDraw.String(10, 10, "Hello, world!", 3, Sans);
    GfxDraw.String(10, 20, "Hello, world!", 3, SansBold);
    GfxDraw.String(10, 30, "Hello, world!", 3, SansItalic);
    GfxDraw.String(10, 40, "Hello, world!", 3, Serif);
    GfxDraw.String(10, 50, "Hello, world!", 3, SerifBold);
    GfxDraw.String(10, 60, "Hello, world!", 3, Courier);
    
    
    
    GfxCore.Refresh();
    
    __asm__ __volatile__("hlt");
  }
}

