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



static inline U8 Idt_EncodeFlags(IdtGateType gateType, IdtPrivilegeLevel privilege, bool present) {
  U8 flags = 0;

  flags |= (gateType & 0x0f);
  flags |= (0 << 4);
  flags |= ((privilege & 0x3) << 5);
  flags |= ((present & 0x1) << 7);
  
  return flags;
}


static inline void Idt_SetGate(IdtEntry* entry, U32 baseAddress, U16 selector, U8 flags) {
  entry->BaseAddressLow	 = baseAddress & 0xffff;
  entry->Selector	 = selector;
  entry->__AlwaysZero	 = 0;
  entry->Flags		 = flags;
  entry->BaseAddressHigh = (baseAddress >> 16) & 0xffff;
}


extern void Idt_Load(IdtEntry *idt, IdtDescriptor* descriptor);


#endif
