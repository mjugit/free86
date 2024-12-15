#include "../Include/Interrupt.h"

/**
 * @brief Array of IDT entries.
 *
 * This array represents the Interrupt Descriptor Table (IDT), which contains
 * `IDT_ENTRY_COUNT` entries. Each entry is an instance of the `IdtEntry` structure,
 * specifying the interrupt handler for a specific interrupt or exception.
 */
struct IdtEntry Idt[IDT_ENTRY_COUNT];


/**
 * @brief Descriptor for the Interrupt Descriptor Table (IDT).
 *
 * The `IdtPointer` structure is used to provide the processor with the base
 * address and size of the IDT. It is loaded into the IDTR (Interrupt Descriptor
 * Table Register) using the `lidt` instruction during initialization.
 */
struct IdtDescriptor IdtPointer;




/**
 * @brief Helper method for writing a byte to a specific port.
 *
 * This inline function writes a single byte to the specified I/O port.
 * It uses the `outb` assembly instruction to perform the operation.
 *
 * @param port The I/O port to write to.
 * @param value The byte value to write.
 */
static inline void _INT_WriteByte(uint16_t port, uint8_t value) {
    __asm__ volatile ("_INT_WriteByte %1, %0" : : "dN" (port), "a" (value));
}


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
void INT_SetIdtGate(int int_number, void(*service_routine), uint8_t flags) {
  uint32_t raw_ptr = (uint32_t)service_routine;

  Idt[int_number].BaseLow = raw_ptr & 0xffff;
  Idt[int_number].SegmentSelector = KERNEL_CS;
  Idt[int_number]._AlwaysZero = 0;
  Idt[int_number].Flags = flags;
  Idt[int_number].BaseHigh = (raw_ptr >> 16) & 0xffff;
}


/**
 * @brief Loads the configured Interrupt Descriptor Table (IDT) into the CPU.
 *
 * This function sets up the `IdtPointer` structure with the base address and size
 * of the IDT and then loads it into the IDTR (Interrupt Descriptor Table Register)
 * using the `lidt` instruction.
 */
void INT_LoadIDT(void) {
  IdtPointer.Limit = sizeof(Idt) - 1;
  IdtPointer.Base = (uint32_t)&Idt;

  __asm__ __volatile__("lidt (%0)" : : "r"(&IdtPointer));
}


/**
 * @brief Enables hardware interrupts by setting the interrupt flag.
 *
 * This function uses the `sti` assembly instruction to enable interrupts
 * globally, allowing the CPU to process external and internal interrupts.
 */
void INT_EnableInterrupts(void) {
    __asm__ __volatile__("sti");
}


/**
 * @brief Disables hardware interrupts by clearing the interrupt flag.
 *
 * This function uses the `cli` assembly instruction to disable interrupts
 * globally, preventing the CPU from processing external and internal interrupts.
 */
void INT_DisableInterrupts(void) {
    __asm__ __volatile__("cli");  
} 


/**
 * @brief Initializes the master Programmable Interrupt Controller (PIC).
 *
 * This function reinitializes the master PIC in cascade mode and configures
 * its interrupt offsets and communication with the slave PIC.
 *
 * Steps:
 * - Sends the initialization command (0x11) to the master PIC control port.
 * - Sets the interrupt vector offset for the master PIC to 32–39.
 * - Configures the master PIC to use IRQ 2 to communicate with the slave PIC.
 * - Sets the PIC to 8086/88 (MCS-80/85) operation mode.
 */
static void INT_InitializeMasterPIC(void) {
    const uint8_t INIT_COMMAND = 0x11;        // Initialization command for PIC
    _INT_WriteByte(MASTER_PIC_CONTROL_PORT, INIT_COMMAND);

    const uint8_t MASTER_PIC_OFFSET = 32;    // Interrupt vector offset for master PIC
    _INT_WriteByte(MASTER_PIC_DATA_PORT, MASTER_PIC_OFFSET);

    _INT_WriteByte(MASTER_PIC_DATA_PORT, 0x04); // IRQ 2 used for communication with slave PIC
    _INT_WriteByte(MASTER_PIC_DATA_PORT, 0x01); // Set 8086 operation mode
}


/**
 * @brief Initializes the slave Programmable Interrupt Controller (PIC).
 *
 * This function reinitializes the slave PIC in cascade mode and configures
 * its interrupt offsets and communication with the master PIC.
 *
 * Steps:
 * - Sends the initialization command (0x11) to the slave PIC control port.
 * - Sets the interrupt vector offset for the slave PIC to 40–47.
 * - Configures the slave PIC to communicate with the master PIC via IRQ 2.
 * - Sets the PIC to 8086/88 (MCS-80/85) operation mode.
 */
static void INT_InitializeSlavePIC(void) {
    const uint8_t INIT_COMMAND = 0x11;       // Initialization command for PIC
    _INT_WriteByte(SLAVE_PIC_CONTROL_PORT, INIT_COMMAND);

    const uint8_t SLAVE_PIC_OFFSET = 40;    // Interrupt vector offset for slave PIC
    _INT_WriteByte(SLAVE_PIC_DATA_PORT, SLAVE_PIC_OFFSET);

    _INT_WriteByte(SLAVE_PIC_DATA_PORT, 0x02); // IRQ 2 used on master PIC
    _INT_WriteByte(SLAVE_PIC_DATA_PORT, 0x01); // Set 8086 operation mode
}


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
void INT_InitializePIC(uint8_t master_pic_mask, uint8_t slave_pic_mask) {
    INT_InitializeMasterPIC();
    INT_InitializeSlavePIC();

    _INT_WriteByte(MASTER_PIC_DATA_PORT, master_pic_mask); // Apply master PIC mask
    _INT_WriteByte(SLAVE_PIC_DATA_PORT, slave_pic_mask);   // Apply slave PIC mask
}


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
void INT_InitializeIDT(void) {
    INT_SetIdtGate(33, ISR_Keyboard, 0x8e); // Assign ISR for keyboard interrupt
    INT_LoadIDT();                          // Load the IDT
}
