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


#define TOTAL_IRQ_LINES 16


static void (*IrqHandlers[TOTAL_IRQ_LINES])(struct InterruptFrame* frame) = { 0 };


// Registers a handler for the given IRQ
void Interrupt_IrqRegister(U8 irq, void (*handler)(struct InterruptFrame *frame)) {
  if (irq >= TOTAL_IRQ_LINES)
    return;
	
  IrqHandlers[irq] = handler;
}


// Unregisters the handler for the given IRQ
void Interrupt_IrqUnregister(U8 irq) {
  if (irq >= TOTAL_IRQ_LINES)
    return;
    
  IrqHandlers[irq] = 0;
}


void Interrupt_IrqDispatch(struct InterruptFrame* frame) {
  U8 irq = (U8)(frame->InterruptNumber - 0x20); // Assuming PIC master offset = 0x20
  if (irq < 16 && IrqHandlers[irq])
    IrqHandlers[irq](frame);
    
  Interrupt.Pic.SendEndOfInterrupt(irq);
}
