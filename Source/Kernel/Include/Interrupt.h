#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <stdint.h>

#include "VgaGraphics.h"

#include "../../StandardLib/Include/CoreTypes.h"
#include "../../StandardLib/Include/Stream.h"


/**
 * @brief Kernel code segment selector for the GDT.
 *
 * This constant represents the segment selector for the kernel code segment
 * in the Global Descriptor Table (GDT). It is used in the `SegmentSelector`
 * field of the `IdtEntry` structure.
 */
#define KERNEL_CS 0x08

/**
 * @brief Number of entries in the Interrupt Descriptor Table (IDT).
 *
 * This constant defines the maximum number of interrupt descriptors
 * that can be stored in the IDT. The value is typically 256 to cover
 * all possible interrupts, including hardware and software exceptions.
 */
#define IDT_ENTRY_COUNT 256

/**
 * @brief Control port address for the master Programmable Interrupt Controller (PIC).
 *
 * The `MASTER_PIC_CONTROL_PORT` is used to send commands to the master PIC.
 */
#define MASTER_PIC_CONTROL_PORT 0x20

/**
 * @brief Data port address for the master Programmable Interrupt Controller (PIC).
 *
 * The `MASTER_PIC_DATA_PORT` is used to read or write data to the master PIC's registers.
 */
#define MASTER_PIC_DATA_PORT 0x21

/**
 * @brief Control port address for the slave Programmable Interrupt Controller (PIC).
 *
 * The `SLAVE_PIC_CONTROL_PORT` is used to send commands to the slave PIC.
 */
#define SLAVE_PIC_CONTROL_PORT 0xA0

/**
 * @brief Data port address for the slave Programmable Interrupt Controller (PIC).
 *
 * The `SLAVE_PIC_DATA_PORT` is used to read or write data to the slave PIC's registers.
 */
#define SLAVE_PIC_DATA_PORT 0xA1




/**
 * @brief Represents an entry in the Interrupt Descriptor Table (IDT).
 *
 * The `IdtEntry` structure defines the format of an IDT entry. Each entry
 * specifies the interrupt handler's base address, the segment selector for
 * the code segment, and other configuration flags.
 *
 * Fields:
 * - `BaseLow`: Lower 16 bits of the interrupt handler's address.
 * - `SegmentSelector`: Selector for the code segment in the GDT or LDT.
 * - `_AlwaysZero`: Must always be zero (reserved field).
 * - `Flags`: Specifies the type and attributes of the IDT entry.
 * - `BaseHigh`: Upper 16 bits of the interrupt handler's address.
 */
struct IdtEntry {
    uint16_t BaseLow;           ///< Lower 16 bits of the handler's base address.
    uint16_t SegmentSelector;   ///< Selector for the code segment.
    uint8_t _AlwaysZero;        ///< Reserved field, must be zero.
    uint8_t Flags;              ///< Flags defining the type and attributes of the entry.
    uint16_t BaseHigh;          ///< Upper 16 bits of the handler's base address.
} __attribute__((packed));


/**
 * @brief Descriptor for the Interrupt Descriptor Table (IDT).
 *
 * The `IdtDescriptor` structure is used to load the IDT into the processor's
 * IDTR (Interrupt Descriptor Table Register). It specifies the base address
 * and size of the IDT.
 *
 * Fields:
 * - `Limit`: Size of the IDT in bytes minus one.
 * - `Base`: Base address of the IDT.
 */
struct IdtDescriptor {
    uint16_t Limit;  ///< Size of the IDT (in bytes) minus one.
    uint32_t Base;   ///< Base address of the IDT.
} __attribute__((packed));




/**
 * @brief Sets an interrupt service routine (ISR) for a specific interrupt.
 *
 * This function configures an entry in the IDT to point to the given service routine.
 * It sets the base address of the ISR, the kernel segment selector, and the flags
 * indicating the type and privilege level of the entry.
 *
 * @param int_number The interrupt number to configure.
 * @param service_routine Pointer to the ISR function.
 * @param flags Flags specifying the type and privilege level of the interrupt.
 */
extern void INT_SetIdtGate(int int_number, void(*service_routine), uint8_t flags);


/**
 * @brief Loads the configured Interrupt Descriptor Table (IDT) into the CPU.
 *
 * This function sets up the `IdtPointer` structure with the base address and size
 * of the IDT and then loads it into the IDTR (Interrupt Descriptor Table Register)
 * using the `lidt` instruction.
 */
extern void INT_LoadIDT(void);


/**
 * @brief Enables hardware interrupts by setting the interrupt flag.
 *
 * This function uses the `sti` assembly instruction to enable interrupts
 * globally, allowing the CPU to process external and internal interrupts.
 */
extern void INT_EnableInterrupts(void);


/**
 * @brief Disables hardware interrupts by clearing the interrupt flag.
 *
 * This function uses the `cli` assembly instruction to disable interrupts
 * globally, preventing the CPU from processing external and internal interrupts.
 */
extern void INT_DisableInterrupts(void);


/**
 * @brief Initializes both the master and slave PICs and sets their IRQ masks.
 *
 * This function initializes the master and slave PICs by reconfiguring their
 * interrupt vector offsets and operational modes. After initialization, the
 * provided IRQ masks are applied to disable specific interrupts.
 *
 * @param master_pic_mask The IRQ mask for the master PIC. Each bit disables a specific IRQ line.
 * @param slave_pic_mask The IRQ mask for the slave PIC. Each bit disables a specific IRQ line.
 */
extern void INT_InitializePIC(uint8_t master_pic_mask, uint8_t slave_pic_mask);


/**
 * @brief Initializes the Interrupt Descriptor Table (IDT).
 *
 * This function sets up the IDT by assigning specific interrupt service routines
 * (ISRs) to the desired interrupt gates. After configuration, the IDT is loaded
 * into the CPU's IDTR (Interrupt Descriptor Table Register).
 *
 * Example:
 * - ISR 33 (interrupt vector for the keyboard) is assigned with flags 0x8e (present, DPL=0).
 */
extern void INT_InitializeIDT(void);


/**
 * @brief External declaration for the keyboard interrupt service routine (ISR).
 *
 * This function is implemented in assembly or elsewhere in the project to handle
 * keyboard interrupts. It is typically linked to interrupt vector 33 in the
 * Interrupt Descriptor Table (IDT).
 */
extern void ISR_Keyboard(void);


#endif
