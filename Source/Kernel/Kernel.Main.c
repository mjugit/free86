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
#include "Include/ColorTheme.h"


use(Bitmap);
use(Heap);
use(String);


// These are populated by the second stage bootloader
extern U16 _Kernel_LowMemoryInfoKiB;
extern U16 _Kernel_HighMemoryInfoKiB;
extern U16 _Kernel_EquipmentWord;




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




// Colors

void _LoadColorPalette(void) {
  Gfx.Core.SetPaletteColor(0, RgbColorSlateBright);
  Gfx.Core.SetPaletteColor(1, RgbColorSlateNormal);
  Gfx.Core.SetPaletteColor(2, RgbColorSlateDark);

  Gfx.Core.SetPaletteColor(3, RgbColorButterBright);
  Gfx.Core.SetPaletteColor(4, RgbColorButterNormal);
  Gfx.Core.SetPaletteColor(5, RgbColorButterDark);

  Gfx.Core.SetPaletteColor(6, RgbColorCameleonBright);
  Gfx.Core.SetPaletteColor(7, RgbColorCameleonNormal);
  Gfx.Core.SetPaletteColor(8, RgbColorCameleonDark);
  
  Gfx.Core.SetPaletteColor(9, RgbColorOrangeBright);
  Gfx.Core.SetPaletteColor(10, RgbColorOrangeNormal);
  Gfx.Core.SetPaletteColor(11, RgbColorOrangeDark);
  
  Gfx.Core.SetPaletteColor(12, RgbColorChocolateBright);
  Gfx.Core.SetPaletteColor(13, RgbColorChocolateNormal);
  Gfx.Core.SetPaletteColor(14, RgbColorChocolateDark);
  
  Gfx.Core.SetPaletteColor(15, RgbColorSkyBlueBright);
  Gfx.Core.SetPaletteColor(16, RgbColorSkyBlueNormal);
  Gfx.Core.SetPaletteColor(17, RgbColorSkyBlueDark);
  
  Gfx.Core.SetPaletteColor(18, RgbColorPlumBright);
  Gfx.Core.SetPaletteColor(19, RgbColorPlumNormal);
  Gfx.Core.SetPaletteColor(20, RgbColorPlumDark);
  
  Gfx.Core.SetPaletteColor(21, RgbColorScarletRedBright);
  Gfx.Core.SetPaletteColor(22, RgbColorScarletRedNormal);
  Gfx.Core.SetPaletteColor(23, RgbColorScarletRedDark);
  
  Gfx.Core.SetPaletteColor(24, RgbColorAluminiumBright);
  Gfx.Core.SetPaletteColor(25, RgbColorAluminiumNormal);
  Gfx.Core.SetPaletteColor(26, RgbColorAluminiumDark);
}

void _LoadColorTheme(void) {
  Gfx.Core.UsePaletteColor(0, 2); // Black
  Gfx.Core.UsePaletteColor(1, 4); // Yellow
  Gfx.Core.UsePaletteColor(2, 7); // Green
  Gfx.Core.UsePaletteColor(3, 10); // Orange
  Gfx.Core.UsePaletteColor(4, 13); // Brown
  Gfx.Core.UsePaletteColor(5, 16); // Blue
  Gfx.Core.UsePaletteColor(6, 19); // Purple
  Gfx.Core.UsePaletteColor(7, 21); // Red
  Gfx.Core.UsePaletteColor(8, 25); // Grey

  // Colors 9 - f are free at the moment
}




/* extern void IrqKeyboard_Stub(void); */

ScanCode scanCode = 0;
KeyCode keyCode = 0;
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
    keyCode = extended ? KeyCodeA : GermanKeyMap[scanCode];

    if (!isRelease) {
      ;
    }
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

  _LoadColorPalette();
  _LoadColorTheme();

  if (Gfx.Core.Initialize(640, 480, 4, _Kernel_DynMemory))
    Gfx.Core.RefreshFromBackBuffer();
  
  Gfx.Core.RefreshFromBackBuffer();
    
 
  while (1) {
    Gfx.Draw.FilledRect(0, 15, 639, 464, 5);
    Gfx.Draw.FilledRect(0, 0, 639, 14, 1);
    
    char headerBarTextBuffer[100] = { };
    String.Format(headerBarTextBuffer,
		  "Heap: %d free, %d used\0", 
		  Heap.GetBytesFree(_Kernel_DynMemory),
		  Heap.GetBytesUsed(_Kernel_DynMemory));
    Gfx.Draw.String(10, 3, headerBarTextBuffer, 0);
    String.Format(headerBarTextBuffer, "Uptime: %d seconds", uptimeSeconds);
    Gfx.Draw.String(320, 3, headerBarTextBuffer, 0);

    
    char scanCodeText[100] = { };
    String.Format(scanCodeText, "Scancode   = %x, Keycode = %x\0", scanCode, keyCode);
    Gfx.Draw.String(10, 20, scanCodeText, 7);
    

    char memInfoText[100] = { };
    String.Format(memInfoText, "Low memory    = %d bytes, (%d KiB)", _Kernel_LowMemoryInfoKiB * 1024, _Kernel_LowMemoryInfoKiB);
    Gfx.Draw.String(10, 32, memInfoText, 8);
    String.Format(memInfoText, "High Memory   = %d bytes, (%d KiB)", _Kernel_HighMemoryInfoKiB * 1024, _Kernel_HighMemoryInfoKiB);
    Gfx.Draw.String(10, 40, memInfoText, 8);

    char equipment[100] = { };
    U16 floppyDriveCount = (_Kernel_EquipmentWord >> 6) & 0x03;
    String.Format(equipment,   "Floppy Drives = %d", floppyDriveCount + 1);
    Gfx.Draw.String(10, 56, equipment, 8);
    U16 rs232Count = (_Kernel_EquipmentWord >> 9) & 0x07;
    String.Format(equipment,   "RS232 Interf. = %d", rs232Count);
    Gfx.Draw.String(10, 64, equipment, 8);
    U16 hasFpu = (_Kernel_EquipmentWord >> 1) & 0x01;
    String.Format(equipment,   "Has FPU       = %d", hasFpu);
    Gfx.Draw.String(10, 72, equipment, 8);
    
    
    Gfx.Core.RefreshFromBackBuffer();
    
    __asm__ __volatile__("hlt");
  }
}

