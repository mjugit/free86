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

#include "../Libs/Include/Gfx.h"

#include "Include/Interrupt.h"
#include "Include/Keyboard.h"

use(Bitmap);
use(Heap);
use(String);

extern U16 _Kernel_LowMemoryInfoKiB;


// Dynamic memory
static HeapMemory _Kernel_DynMemory;

static void _InitializeHeap(void) {
  void* heapLimitBottom = (void*)0x10000;
  void* heapLimitTop = (void*)0x60000;

  U32 totalHeapSize = heapLimitTop - heapLimitBottom;
  _Kernel_DynMemory = Heap.Initialize(heapLimitBottom, totalHeapSize);
}


// Interrupts
static IdtEntry _Kernel_Idt[256];
static IdtDescriptor _Kernel_IdtDescriptor;




/* extern void IrqKeyboard_Stub(void); */

static U16 pixelX = 0;
ScanCode scanCode = 0;
static U8 extended = 0;
static void KeyboardHandler(void) {
      U8 raw = PortReadByte(0x60);

    if (raw == 0xe0 || raw == 0xe1) {
        extended = raw;
        return;
    }

    bool isRelease = (raw & 0x80) != 0;
    U16 code = raw & 0x7f;

    if (extended) {
        code |= (extended << 8);
        extended = 0;
    }

    scanCode = (ScanCode)code;

    if (!isRelease) {
        pixelX++;
    }
}


KeyCode TranslateScanCode(ScanCode scanCode) {
    switch (scanCode) {
        /* Letters */
        case ScanCodeA: return KeyCodeA;
        case ScanCodeB: return KeyCodeB;
        case ScanCodeC: return KeyCodeC;
        case ScanCodeD: return KeyCodeD;
        case ScanCodeE: return KeyCodeE;
        case ScanCodeF: return KeyCodeF;
        case ScanCodeG: return KeyCodeG;
        case ScanCodeH: return KeyCodeH;
        case ScanCodeI: return KeyCodeI;
        case ScanCodeJ: return KeyCodeJ;
        case ScanCodeK: return KeyCodeK;
        case ScanCodeL: return KeyCodeL;
        case ScanCodeM: return KeyCodeM;
        case ScanCodeN: return KeyCodeN;
        case ScanCodeO: return KeyCodeO;
        case ScanCodeP: return KeyCodeP;
        case ScanCodeQ: return KeyCodeQ;
        case ScanCodeR: return KeyCodeR;
        case ScanCodeS: return KeyCodeS;
        case ScanCodeT: return KeyCodeT;
        case ScanCodeU: return KeyCodeU;
        case ScanCodeV: return KeyCodeV;
        case ScanCodeW: return KeyCodeW;
        case ScanCodeX: return KeyCodeX;
        case ScanCodeY: return KeyCodeY;
        case ScanCodeZ: return KeyCodeZ;

        /* Numbers */
        case ScanCode1: return KeyCode1;
        case ScanCode2: return KeyCode2;
        case ScanCode3: return KeyCode3;
        case ScanCode4: return KeyCode4;
        case ScanCode5: return KeyCode5;
        case ScanCode6: return KeyCode6;
        case ScanCode7: return KeyCode7;
        case ScanCode8: return KeyCode8;
        case ScanCode9: return KeyCode9;
        case ScanCode0: return KeyCode0;

        /* Control keys */
        case ScanCodeEnter:     return KeyCodeEnter;
        case ScanCodeEsc:       return KeyCodeEsc;
        case ScanCodeBackspace: return KeyCodeBackspace;
        case ScanCodeTab:       return KeyCodeTab;
        case ScanCodeSpace:     return KeyCodeSpace;

        /* Symbols */
        case ScanCodeMinus:      return KeyCodeMinus;
        case ScanCodeEquals:     return KeyCodeEquals;
        case ScanCodeLBracket:   return KeyCodeLBracket;
        case ScanCodeRBracket:   return KeyCodeRBracket;
        case ScanCodeBackslash:  return KeyCodeBackslash;
        case ScanCodeSemicolon:  return KeyCodeSemicolon;
        case ScanCodeApostrophe: return KeyCodeApostrophe;
        case ScanCodeGrave:      return KeyCodeGrave;
        case ScanCodeComma:      return KeyCodeComma;
        case ScanCodePeriod:     return KeyCodePeriod;
        case ScanCodeSlash:      return KeyCodeSlash;

        /* Modifiers */
        case ScanCodeLShift: return KeyCodeLShift;
        case ScanCodeLCtrl:  return KeyCodeLCtrl;
        case ScanCodeLAlt:   return KeyCodeLAlt;

        /* Function keys */
        case ScanCodeF1:  return KeyCodeF1;
        case ScanCodeF2:  return KeyCodeF2;
        case ScanCodeF3:  return KeyCodeF3;
        case ScanCodeF4:  return KeyCodeF4;
        case ScanCodeF5:  return KeyCodeF5;
        case ScanCodeF6:  return KeyCodeF6;
        case ScanCodeF7:  return KeyCodeF7;
        case ScanCodeF8:  return KeyCodeF8;
        case ScanCodeF9:  return KeyCodeF9;
        case ScanCodeF10: return KeyCodeF10;
        case ScanCodeF11: return KeyCodeF11;
        case ScanCodeF12: return KeyCodeF12;

        /* Extended keys */
        case ScanCodeRCtrl:     return KeyCodeRCtrl;
        case ScanCodeRAlt:      return KeyCodeRAlt;
        case ScanCodeHome:      return KeyCodeHome;
        case ScanCodeUp:        return KeyCodeUp;
        case ScanCodePageUp:    return KeyCodePageUp;
        case ScanCodeLeft:      return KeyCodeLeft;
        case ScanCodeRight:     return KeyCodeRight;
        case ScanCodeEnd:       return KeyCodeEnd;
        case ScanCodeDown:      return KeyCodeDown;
        case ScanCodePageDown:  return KeyCodePageDown;
        case ScanCodeInsert:    return KeyCodeInsert;
        case ScanCodeDelete:    return KeyCodeDelete;

        default:
            return KeyCodeError;
    }
}




/* Naked attribute: no prolog/epilog */
__attribute__((naked))
void IrqKeyboard_Handler(void) {
    __asm__ __volatile__(
        "pusha\n"
    );

    KeyboardHandler();
    
    __asm__ __volatile__(
        /* Send EOI to PIC */
        "movb $0x20, %al\n"
        "outb %al, $0x20\n"
        "popa\n"
        "iret\n"
    );
}


U32 uptimeSeconds = 0;
U16 timerCounter = 0;
__attribute__((naked))
void IrqTimer_Handler(void) {
    __asm__ __volatile__(
        "pusha\n"
    );

    timerCounter = timerCounter + 1;
    if (timerCounter == 100) {
      pixelX++;
      uptimeSeconds++;
      timerCounter = 0;
    }
    
    __asm__ __volatile__(
        /* Send EOI to PIC */
        "movb $0x20, %al\n"
        "outb %al, $0x20\n"
        "popa\n"
        "iret\n"
    );
}

static void InitializeTimer() {
    U16 divisor = 11932; /* ≈ 100 Hz */

    PortWriteByte(0x43, 0x36);

    PortWriteByte(0x40, (U8)(divisor & 0xFF));
    PortWriteByte(0x40, (U8)(divisor >> 8));

    U8 timerHandlerFlags = Idt_EncodeFlags(IdtGateTypeInt32, IdtPrivilegeKernel, true);
    Idt_SetGate(&_Kernel_Idt[0x20], (U32)IrqTimer_Handler, 0x08,  timerHandlerFlags);
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

    /* Mask everything except IRQ1 (keyboard) */
    // PortWriteByte(0x21, 0xFD);
    /* Mask everything except IRQ1 (keyboard) and IRQ0 (timer)*/
    PortWriteByte(0x21, 0xfc);
    PortWriteByte(0xA1, 0xFF);

    InitializeTimer();
    U8 keyboardHandlerFlags = Idt_EncodeFlags(IdtGateTypeInt32, IdtPrivilegeKernel, true);
    Idt_SetGate(&_Kernel_Idt[0x21], (U32)IrqKeyboard_Handler, 0x08, keyboardHandlerFlags);
    Idt_Load(_Kernel_Idt, &_Kernel_IdtDescriptor);

    EnableInterrupts();
}
 

void KernelMain() {
  _InitializeHeap();
  _InitializeInterrupts();

  if (Gfx.Core.Initialize(640, 480, 4, _Kernel_DynMemory))
    Gfx.Core.RefreshFromBackBuffer();
  
  Gfx.Draw.FilledRect(0, 0, 639, 14, 7);

  
  char textBuffer[100] = { };
  Gfx.Core.RefreshFromBackBuffer();
    
 
  while (1) {
    Gfx.Draw.Pixel(pixelX, 479, 4);
    Gfx.Draw.FilledRect(5, 19, 630, 451, 15);

    
    Gfx.Draw.FilledRect(0, 0, 639, 14, 7);
    char timeText[100] = { };
    String.Format(timeText, "Up %d secs\0", uptimeSeconds);

    char memText[100] = { };
    String.Format(memText,
		  "%d free of %d bytes\0",
		  Heap.GetBytesFree(_Kernel_DynMemory),
		  Heap.GetBytesUsed(_Kernel_DynMemory) + Heap.GetBytesFree(_Kernel_DynMemory));
    
    Gfx.Draw.String(10, 3, timeText, 0);
    Gfx.Draw.String(340, 3, memText, 0);

    char scanCodeText[100] = { };
    String.Format(scanCodeText, "%x", scanCode);
    Gfx.Draw.String(10, 20, scanCodeText, 4);
    Gfx.Core.RefreshFromBackBuffer();
    
    __asm__ __volatile__("hlt");
  }
}

