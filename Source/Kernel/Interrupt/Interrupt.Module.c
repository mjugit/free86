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


extern void _Idt_InitializeImplementation(IdtEntry* idt, IdtDescriptor* descriptor);
extern void _Idt_LoadImplementation(IdtEntry* idt, IdtDescriptor* descriptor);
extern void _Idt_SetEntryImplementation(
				 IdtEntry* idt,
				 U8 index,
				 U32 handlerAddress,
				 U16 selector,
				 IdtGateType gateType,
				 IdtPrivilegeLevel privelegeLevel);
extern void _Idt_EnableEntryImplementation(IdtEntry* idt, U8 index);
extern void _Idt_DisableEntryImplementation(IdtEntry* idt, U8 index);


members(Idt) {
    .Initialize = _Idt_InitializeImplementation,
    .Load = _Idt_LoadImplementation,
    .SetEntry = _Idt_SetEntryImplementation,
    .EnableEntry = _Idt_EnableEntryImplementation,
    .DisableEntry = _Idt_DisableEntryImplementation
};




extern void _Pic_RemapImplementation(U8 masterOffset, U8 slaveOffset);
extern void _Pic_SendEoiImplementation(U8 irqNumber);
extern void _Pic_SetFullMaskImplementation(U16 mask);
extern void _Pic_SetMaskImplementation(U8 irqLine);
extern void _Pic_ClearMaskImplementation(U8 irqLine);
extern U16 _Pic_GetMaskImplementation(void);


members(Pic) {
    .Remap = _Pic_RemapImplementation,
    .SendEoi = _Pic_SendEoiImplementation,
    .SetFullMask = _Pic_SetFullMaskImplementation,
    .SetMask = _Pic_SetMaskImplementation,
    .ClearMask = _Pic_ClearMaskImplementation,
    .GetMask = _Pic_GetMaskImplementation
};



extern void _Interrupt_SetUpAllImplementation(IdtEntry* idt, IdtDescriptor* descriptor);

members(Interrupt) {
    .Idt = Idt,
    .Pic = Pic,
    .SetUpAll = _Interrupt_SetUpAllImplementation
};
