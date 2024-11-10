#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <stdint.h>

#define KERNEL_CS 0x08
#define IDT_ENTRY_COUNT 256

// Master PIC ports
#define MASTER_PIC_CONTROL_PORT 0x20
#define MASTER_PIC_DATA_PORT 0x21

// Slave PIC ports
#define SLAVE_PIC_CONTROL_PORT 0xa0
#define SLAVE_PIC_DATA_PORT 0xa1

struct IdtEntry {
  uint16_t BaseLow;
  uint16_t SegmentSelector;
  uint8_t _AlwaysZero;
  uint8_t Flags;
  uint16_t BaseHigh;
} __attribute__((packed));


struct IdtDescriptor {
  uint16_t Limit;
  uint32_t Base;
} __attribute__((packed));


extern void INT_SetIdtGate(int int_number, void(*service_routine), uint8_t flags);
extern void INT_LoadIDT(void);
extern void INT_EnableInterrupts(void);
extern void INT_DisableInterrupts(void);
extern void INT_InitializePIC(uint8_t master_pic_mask, uint8_t slave_pic_mask) ;
extern void INT_InitializeIDT(void);


extern void ISR_Keyboard(void);
extern void keyboard_handler(void);


#endif
