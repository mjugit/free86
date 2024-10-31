#include "../include/libint.h"

struct IdtEntry Idt[LIBINT_IDT_ENTRIES];
struct IdtDescriptor IdtPointer;

// Helper method for writing to a port
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Helper method for reading from a port
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Set the service routine for a specific interrupt
void int_set_idt_gate(int int_number, void(*service_routine), uint8_t flags) {
  uint32_t raw_ptr = (uint32_t)service_routine;

  Idt[int_number].base_low = raw_ptr & 0xffff;
  Idt[int_number].segment_selector = LIBINT_KERNEL_CODE_SEGMENT;
  Idt[int_number]._always_zero = 0;
  Idt[int_number].flags = flags;
  Idt[int_number].base_high = (raw_ptr >> 16) & 0xffff;
}

// Load the configured IDT
void int_load_idt(void) {
  IdtPointer.limit = sizeof(Idt) - 1;
  IdtPointer.base = (uint32_t)&Idt;

  __asm__ __volatile__("lidt (%0)" : : "r"(&IdtPointer));
}

// Set the interrupt enable flag
void int_enable_interrupts(void) {
    __asm__ __volatile__("sti");
}

// Clear the interrupt enable flag
void int_disable_interrupts(void) {
    __asm__ __volatile__("cli");  
} 

// Initialize the master PIC
static void int_init_master_pic(void) {
  // Reinitialize in cascade mode
  const uint8_t INIT_COMMAND = 0x11;
  outb(MASTER_PIC_CONTROL_PORT, INIT_COMMAND);

  // Assign interrupt numbers 32 - 39
  const uint8_t MASTER_PIC_OFFSET = 32;
  outb(MASTER_PIC_DATA_PORT, MASTER_PIC_OFFSET);
  // Use IRQ 2 for slave controller
  outb(MASTER_PIC_DATA_PORT, 0x04);
  // Use 8086 operation mode
  outb(MASTER_PIC_DATA_PORT, 0x01);
}

// Initialize the slave PIC
static void int_init_slave_pic(void) {
  // Reinitialize in cascade mode
  const uint8_t INIT_COMMAND = 0x11;
  outb(SLAVE_PIC_CONTROL_PORT, INIT_COMMAND);

  // Assign interrupt numbers 40 - 47
  const uint8_t SLAVE_PIC_OFFSET = 40;
  outb(SLAVE_PIC_DATA_PORT, SLAVE_PIC_OFFSET);
  // Use IRQ 2 on master
  outb(SLAVE_PIC_DATA_PORT, 0x02);
  // Use 8086 operation mode
  outb(SLAVE_PIC_DATA_PORT, 0x01);
}

// Init both the master and the slave PIC and set the given IRQ mask
void int_init_pic(uint8_t master_pic_mask, uint8_t slave_pic_mask) {
  int_init_master_pic();
  int_init_slave_pic();

  outb(MASTER_PIC_DATA_PORT, master_pic_mask);
  outb(SLAVE_PIC_DATA_PORT, slave_pic_mask);
}



void int_init_idt(void) {
  int_set_idt_gate(33, isr_keyboard, 0x8e);
  int_load_idt();
}

