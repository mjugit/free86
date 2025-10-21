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


#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "HardwareIO.h"


// The total amount of IDT entries
#define IDT_SIZE 256





typedef struct IdtEntry {
  U16 BaseAddressLow;
  U16 Selector;
  U8 __AlwaysZero;
  U8 Flags;
  U16 BaseAddressHigh;
} __attribute__((packed)) IdtEntry;


typedef struct IdtDescriptor {
  U16 Limit;
  U32 BaseAddress;
} __attribute__((packed)) IdtDescriptor;



typedef enum {
  IdtPrivilegeKernel = 0,
  IdtPrivilegeRing1 = 1,
  IdtPrivilegeRing2 = 2,
  IdtPrivilegeUser = 3
} IdtPrivilegeLevel;


typedef enum {
  IdtGateTypeTask16 = 0x5,
  IdtGateTypeInt16 = 0x6,
  IdtGateTypeTrap16 = 0x7,
  IdtGateTypeInt32 = 0xe,
  IdtGateTypeTrap32 = 0xf
} IdtGateType;


// Encode the IDT flags field
__attribute__((always_inline, unused))
static inline U8 _Idt_EncodeFlags(IdtGateType gateType, IdtPrivilegeLevel privilege, bool present) {
  U8 flags = 0;

  flags |= (gateType & 0x0f);
  flags |= (0 << 4);
  flags |= ((privilege & 0x3) << 5);
  flags |= ((present & 0x1) << 7);
  
  return flags;
}


// Set an IDT gate
__attribute__((always_inline, unused))
static inline void _Idt_SetGate(IdtEntry* entry, U32 baseAddress, U16 selector, U8 flags) {
  entry->BaseAddressLow	 = baseAddress & 0xffff;
  entry->Selector	 = selector;
  entry->__AlwaysZero	 = 0;
  entry->Flags		 = flags;
  entry->BaseAddressHigh = (baseAddress >> 16) & 0xffff;
}





// Invoke pusha
#define PUSHA	__asm__ __volatile__("pusha\n");


// Send end of interrupt to PIC
#define EOI			        \
  __asm__ __volatile__(			        \
		        "movb $0x20, %al\n"	\
		        "outb %al, $0x20\n"	\
		        "popa\n"		\
		        "iret\n"		\
		        );


module(Idt) {
  void (*Initialize)(IdtEntry* idt, IdtDescriptor* descriptor);
  void (*Load)(IdtEntry* idt, IdtDescriptor* descriptor);
  void (*SetEntry)(IdtEntry* idt,
		   U8 index,
		   U32 handlerAddress,
		   U16 selector,
		   IdtGateType gateType,
		   IdtPrivilegeLevel privelegeLevel);
  void (*EnableEntry)(IdtEntry* idt, U8 index);
  void (*DisableEntry)(IdtEntry* idt, U8 index);
};



#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0
#define PIC2_DATA 0xa1
#define PIC_EOI 0x20


module(Pic) {
  void (*Remap)(U8 masterOffset, U8 slaveOffset);
  void (*SendEoi)(U8 irqNumber);
  void (*SetFullMask)(U16 mask);
  void (*SetMask)(U8 irqLine);
  void (*ClearMask)(U8 irqLine);
  U16 (*GetMask)(void);
};


module(Interrupt) {
  embed(Idt, Idt);
  embed(Pic, Pic);

  void (*SetUpAll)(IdtEntry* idt, IdtDescriptor* descriptor);
};

#endif
