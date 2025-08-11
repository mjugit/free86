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


// IDT namespace members
void Interrupt_LoadDescriptorTable();
void Interrupt_SetGate(U8 vector, U32 base, U16 selector, U8 flags);
void Interrupt_ClearGate(U8 vector);

// PIC namespace members
void Interrupt_PicRemap(U8 masterOffset, U8 slaveOffset);
void Interrupt_SetFullMask(U16 mask);
U16 Interrupt_GetFullMask(void);
void Interrupt_PicMaskRequest(U8 irq);
void Interrupt_PicUnmaskRequest(U8 irq);
void Interrupt_SendEndOfInterrupt(U8 irq);

// IRQ namespace members
void Interrupt_IrqRegister(U8 irq, void (*handler)(struct InterruptFrame *frame));
void Interrupt_IrqUnregister(U8 irq);
void Interrupt_IrqEnable(U8 irq);
void Interrupt_IrqDisable(U8 irq);
bool Interrupt_IrqIsEnabled(U8 irq);
void Interrupt_IrqDispatch(struct InterruptFrame* frame);


const struct __Interrupt_Namespace Interrupt = {
  .Idt = {
    .LoadDescriptorTable = Interrupt_LoadDescriptorTable,
    .SetGate = Interrupt_SetGate,
    .ClearGate = Interrupt_ClearGate
  },
  .Pic = {
    // Ports
    .MasterCommandPort = 0x20,
    .MasterDataPort = 0x21,
    .SlaveCommandPort = 0xa0,
    .SlaveDataPort = 0xa1,

    // Functions
    .Remap = Interrupt_PicRemap,
    .SetFullMask = Interrupt_SetFullMask,
    .GetFullMask = Interrupt_GetFullMask,
    .MaskRequest = Interrupt_PicMaskRequest,
    .UnmaskRequest = Interrupt_PicUnmaskRequest,
    .SendEndOfInterrupt = Interrupt_SendEndOfInterrupt
  },
  .Irq = {
    .Register = Interrupt_IrqRegister,
    .Unregister = Interrupt_IrqUnregister,
    .Enable = Interrupt_IrqEnable,
    .Disable = Interrupt_IrqDisable,
    .IsEnabled = Interrupt_IrqIsEnabled,
    .Dispatch = Interrupt_IrqDispatch
  }
};
