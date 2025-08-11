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


#include "../Include/Interrupt.h"


#define TOTAL_IDT_ENTRIES 256

static IdtEntry DescriptorTable[TOTAL_IDT_ENTRIES];
static IdtDescriptor DescriptorInstance;


// Loads the IDT into the CPU using lidt
void Interrupt_LoadDescriptorTable() {
  DescriptorInstance.Limit = sizeof(DescriptorTable) - 1;
  // Strange cast, I know - just to mask a compiler warning
  DescriptorInstance.Base = (U32)(unsigned long)&DescriptorTable;
  
  __asm__ volatile ("lidt (%0)" : : "r"(&DescriptorInstance));
}

// Sets a gate in the IDT
void Interrupt_SetGate(U8 vector, U32 base, U16 selector, U8 flags) {
  DescriptorTable[vector].BaseLow = (U16)(base & 0xFFFF);
  DescriptorTable[vector].SegmentSelector = selector;
  DescriptorTable[vector].__ZeroReserved = 0;
  DescriptorTable[vector].Flags = flags;
  DescriptorTable[vector].BaseHigh = (U16)((base >> 16) & 0xFFFF);
}

// Clears a gate in the IDT
void Interrupt_ClearGate(U8 vector) {
  DescriptorTable[vector].BaseLow = 0;
  DescriptorTable[vector].SegmentSelector = 0;
  DescriptorTable[vector].__ZeroReserved = 0;
  DescriptorTable[vector].Flags = 0;
  DescriptorTable[vector].BaseHigh = 0;
}
