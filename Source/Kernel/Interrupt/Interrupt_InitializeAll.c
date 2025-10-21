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


use(Interrupt);


// PIC Offsets

#define MASTER_PIC_OFFSET            0x20  // 32
#define SLAVE_PIC_OFFSET             0x28  // 40

// Hardware Interrupt Vector Numbers (after PIC remap)

#define IRQ_PIT_TIMER                0x20  // IRQ0 - Programmable Interval Timer
#define IRQ_KEYBOARD                 0x21  // IRQ1 - Keyboard Controller
#define IRQ_CASCADE                  0x22  // IRQ2 - Cascade (connection to slave PIC)
#define IRQ_SERIAL_COM2              0x23  // IRQ3 - Serial Port COM2/COM4
#define IRQ_SERIAL_COM1              0x24  // IRQ4 - Serial Port COM1/COM3
#define IRQ_LPT2                     0x25  // IRQ5 - LPT2 / Sound Card (legacy)
#define IRQ_FLOPPY_DISK              0x26  // IRQ6 - Floppy Disk Controller
#define IRQ_LPT1_SPURIOUS            0x27  // IRQ7 - LPT1 / Spurious Interrupt

#define IRQ_CMOS_RTC                 0x28  // IRQ8 - Real Time Clock
#define IRQ_FREE1                    0x29  // IRQ9 - Available / ACPI / Redirected IRQ2
#define IRQ_FREE2                    0x2A  // IRQ10 - Available / Network
#define IRQ_FREE3                    0x2B  // IRQ11 - Available / SCSI / USB
#define IRQ_MOUSE                    0x2C  // IRQ12 - PS/2 Mouse
#define IRQ_FPU_COPROCESSOR          0x2D  // IRQ13 - FPU / Coprocessor / Math Fault
#define IRQ_PRIMARY_ATA              0x2E  // IRQ14 - Primary ATA Hard Disk
#define IRQ_SECONDARY_ATA            0x2F  // IRQ15 - Secondary ATA Hard Disk



// CPU Reserved Exception Vector Numbers (0–31)

#define EXC_DIVIDE_ERROR             0x00  // Divide Error (#DE)
#define EXC_DEBUG                    0x01  // Debug Exception (#DB)
#define EXC_NMI_INTERRUPT            0x02  // Non-Maskable Interrupt (NMI)
#define EXC_BREAKPOINT               0x03  // Breakpoint (#BP)
#define EXC_OVERFLOW                 0x04  // Overflow (#OF)
#define EXC_BOUND_RANGE_EXCEEDED     0x05  // BOUND Range Exceeded (#BR)
#define EXC_INVALID_OPCODE           0x06  // Invalid Opcode (#UD)
#define EXC_DEVICE_NOT_AVAILABLE     0x07  // Device Not Available (#NM)
#define EXC_DOUBLE_FAULT             0x08  // Double Fault (#DF)
#define EXC_COPROCESSOR_SEG_OVERRUN  0x09  // Coprocessor Segment Overrun (reserved after 486)
#define EXC_INVALID_TSS              0x0A  // Invalid TSS (#TS)
#define EXC_SEGMENT_NOT_PRESENT      0x0B  // Segment Not Present (#NP)
#define EXC_STACK_SEGMENT_FAULT      0x0C  // Stack-Segment Fault (#SS)
#define EXC_GENERAL_PROTECTION_FAULT 0x0D  // General Protection Fault (#GP)
#define EXC_PAGE_FAULT               0x0E  // Page Fault (#PF)
#define EXC_RESERVED_0F              0x0F  // Reserved
#define EXC_X87_FLOATING_POINT       0x10  // x87 FPU Floating-Point Error (#MF)
#define EXC_ALIGNMENT_CHECK          0x11  // Alignment Check (#AC)
#define EXC_MACHINE_CHECK            0x12  // Machine Check (#MC)
#define EXC_SIMD_FLOATING_POINT      0x13  // SIMD Floating-Point Exception (#XM/#XF)
#define EXC_VIRTUALIZATION_EXCEPTION 0x14  // Virtualization Exception (#VE)
#define EXC_CONTROL_PROTECTION       0x15  // Control Protection Exception (#CP)
#define EXC_RESERVED_16              0x16  // Reserved
#define EXC_RESERVED_17              0x17  // Reserved
#define EXC_RESERVED_18              0x18  // Reserved
#define EXC_RESERVED_19              0x19  // Reserved
#define EXC_RESERVED_1A              0x1A  // Reserved
#define EXC_RESERVED_1B              0x1B  // Reserved
#define EXC_RESERVED_1C              0x1C  // Reserved
#define EXC_RESERVED_1D              0x1D  // Reserved
#define EXC_RESERVED_1E              0x1E  // Reserved
#define EXC_SECURITY_EXCEPTION       0x1F  // Security Exception (#SX)


#define KERNEL_SEG 0x08

// Interrupt Service Routines
extern void _Isr_Timer(void);
extern void _Isr_Keyboard(void);


void _Interrupt_SetUpAllImplementation(IdtEntry* idt, IdtDescriptor* descriptor) {
  DisableInterrupts();

  /*
   * Since the interrupt vectors 0 - 31 are reserved for CPU exceptions,
   * the PICs are remapped to vectors 32 - 48.
   */

  Interrupt.Pic.Remap(MASTER_PIC_OFFSET, SLAVE_PIC_OFFSET);
  Interrupt.Pic.SetMask(0xff);

  /*
   * Interrupt service routines are configured here
   */

  Interrupt.Idt.Initialize(idt, descriptor);

  /*
   * CPU interrupts
   */
  

  /*
   * Hardware interrupts
   */
  
  U32 isrTimer = (U32)_Isr_Timer;
  Interrupt.Idt.SetEntry(idt, IRQ_PIT_TIMER, isrTimer, KERNEL_SEG, IdtGateTypeInt32, IdtPrivilegeKernel);
  Interrupt.Idt.EnableEntry(idt, IRQ_PIT_TIMER);
  
  U32 isrKeyboard = (U32)_Isr_Keyboard;
  Interrupt.Idt.SetEntry(idt, IRQ_KEYBOARD, isrKeyboard, KERNEL_SEG, IdtGateTypeInt32, IdtPrivilegeKernel);
  Interrupt.Idt.EnableEntry(idt, IRQ_KEYBOARD);

  Interrupt.Idt.Load(idt, descriptor);
  EnableInterrupts();
}



#undef MASTER_PIC_OFFSET
#undef SLAVE_PIC_OFFSET
