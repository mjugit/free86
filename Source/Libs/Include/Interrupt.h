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

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "CoreTypes.h"
#include "HardwareIO.h"


typedef struct {
  U16 BaseLow;           // Lower 16 bits of the handler's base address.
  U16 SegmentSelector;   // Selector for the code segment.
  U8 __ZeroReserved;     // Reserved field, must be zero.
  U8 Flags;              // Flags defining the type and attributes of the entry.
  U16 BaseHigh;          // Upper 16 bits of the handler's base address.
} IdtEntry;


typedef struct {
  U16 Limit;  // Size of the IDT (in bytes) minus one.
  U32 Base;   // Base address of the IDT.
} IdtDescriptor;


struct InterruptFrame {
    U32 Edi;
    U32 Esi;
    U32 Ebp;
    U32 EspDummy;   // Value of ESP before pusha; not the real ESP
    U32 Ebx;
    U32 Edx;
    U32 Ecx;
    U32 Eax;

    // Interrupt number and optional error code
    U32 InterruptNumber;
    U32 ErrorCode;

    // Pushed automatically by CPU on interrupt/exception
    U32 Eip;
    U32 Cs;
    U32 Eflags;

    // Only pushed if privilege level change occurred
    U32 UserEsp;
    U32 UserSs;
};


// Namespace for IDT Setup
struct __Interrupt_Idt_Namespace {
  void (*LoadDescriptorTable)(void);
  void (*SetGate)(U8 vector, U32 base, U16 selector, U8 flags);
  void (*ClearGate)(U8 vector);
};


// Namespace for PIC management
struct __Interrupt_Pic_Namespace {
  const U16 MasterCommandPort;
  const U16 MasterDataPort;
  const U16 SlaveCommandPort;
  const U16 SlaveDataPort;

  
  void (*Remap)(U8 masterOffset, U8 slaveOffset);
  void (*SetFullMask)(U16 mask);
  U16 (*GetFullMask)(void);
  void (*MaskRequest)(U8 irq);
  void (*UnmaskRequest)(U8 irq);
  void (*SendEndOfInterrupt)(U8 irq);
};


// Namespace for hardware interrupts
struct __Interrupt_Irq_Namespace {
  void (*Register)(U8 irq, void (*handler)(struct InterruptFrame *frame));
  void (*Unregister)(U8 irq);
  void (*Enable)(U8 irq);
  void (*Disable)(U8 irq);
  bool (*IsEnabled)(U8 irq);
  void (*Dispatch)(struct InterruptFrame *frame);
};


struct __Interrupt_Namespace {
  struct __Interrupt_Idt_Namespace Idt;
  struct __Interrupt_Pic_Namespace Pic;
  struct __Interrupt_Irq_Namespace Irq;
};



extern const struct __Interrupt_Namespace Interrupt;

#endif
