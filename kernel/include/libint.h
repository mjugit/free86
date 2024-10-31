#ifndef _LIBINT_H_
#define _LIBINT_H_

#include <stdint.h>
#include "libvga.h"

#define LIBINT_KERNEL_CODE_SEGMENT 0x08
#define LIBINT_IDT_ENTRIES 256

// Master PIC ports
#define MASTER_PIC_CONTROL_PORT 0x20
#define MASTER_PIC_DATA_PORT 0x21

// Slave PIC ports
#define SLAVE_PIC_CONTROL_PORT 0xa0
#define SLAVE_PIC_DATA_PORT 0xa1

struct IdtEntry {
  uint16_t base_low;
  uint16_t segment_selector;
  uint8_t _always_zero;
  uint8_t flags;
  uint16_t base_high;
} __attribute__((packed));


struct IdtDescriptor {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));


extern void int_set_idt_gate(int int_number, void(*service_routine), uint8_t flags);
extern void int_load_idt(void);
extern void int_enable_interrupts(void);
extern void int_disable_interrupts(void);
extern void int_init_pic(uint8_t master_pic_mask, uint8_t slave_pic_mask) ;
extern void int_init_idt(void);


extern void isr_keyboard(void);
extern void keyboard_handler(void);

#endif
