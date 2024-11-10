#include "../Include/Interrupt.h"

struct IdtEntry Idt[IDT_ENTRY_COUNT];
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
void INT_SetIdtGate(int int_number, void(*service_routine), uint8_t flags) {
  uint32_t raw_ptr = (uint32_t)service_routine;

  Idt[int_number].BaseLow = raw_ptr & 0xffff;
  Idt[int_number].SegmentSelector = KERNEL_CS;
  Idt[int_number]._AlwaysZero = 0;
  Idt[int_number].Flags = flags;
  Idt[int_number].BaseHigh = (raw_ptr >> 16) & 0xffff;
}

// Load the configured IDT
void INT_LoadIDT(void) {
  IdtPointer.Limit = sizeof(Idt) - 1;
  IdtPointer.Base = (uint32_t)&Idt;

  __asm__ __volatile__("lidt (%0)" : : "r"(&IdtPointer));
}

// Set the interrupt enable flag
void INT_EnableInterrupts(void) {
    __asm__ __volatile__("sti");
}

// Clear the interrupt enable flag
void INT_DisableInterrupts(void) {
    __asm__ __volatile__("cli");  
} 

// Initialize the master PIC
static void INT_InitializeMasterPIC(void) {
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
static void INT_InitializeSlavePIC(void) {
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
void INT_InitializePIC(uint8_t master_pic_mask, uint8_t slave_pic_mask) {
  INT_InitializeMasterPIC();
  INT_InitializeSlavePIC();

  outb(MASTER_PIC_DATA_PORT, master_pic_mask);
  outb(SLAVE_PIC_DATA_PORT, slave_pic_mask);
}

void INT_InitializeIDT(void) {
  INT_SetIdtGate(33, ISR_Keyboard, 0x8e);
  INT_LoadIDT();
}




static char german_lowercase[128]  = {
  0,    0,    '1',  '2',  '3',  '4',  '5',  '6',    // 0x00 - 0x07
  '7',  '8',  '9',  '0',  'ß',  '\'', 0,    '\t',   // 0x08 - 0x0F
  'q',  'w',  'e',  'r',  't',  'z',  'u',  'i',    // 0x10 - 0x17
  'o',  'p',  'u',  '+',  0,    0,    'a',  's',    // 0x18 - 0x1F
  'd',  'f',  'g',  'h',  'j',  'k',  'l',  'o',    // 0x20 - 0x27
  'a',  '#',  0,    '<',  'y',  'x',  'c',  'v',    // 0x28 - 0x2F
  'b',  'n',  'm',  ',',  '.',  '-',  0,    '*',    // 0x30 - 0x37
  0,    ' ',  0,    0,    0,    0,    0,    0,      // 0x38 - 0x3F
  0,    0,    0,    0,    0,    0,    0,    0,      // 0x40 - 0x47
  0,    0,    0,    0,    0,    0,    0,    0,      // 0x48 - 0x4F
  0,    0,    0,    0,    0,    0,    0,    0,      // 0x50 - 0x57
  0,    0,    0,    0,    0,    0,    0,    0       // 0x58 - 0x5F
};

static char german_uppercase[128] = {
    0,    0,    '!',  '"',  '§',  '$',  '%',  '&',    // 0x00 - 0x07
    '/',  '(',  ')',  '=',  '?',  '`',  0,    '\t',   // 0x08 - 0x0F
    'Q',  'W',  'E',  'R',  'T',  'Z',  'U',  'I',    // 0x10 - 0x17
    'O',  'P',  'U',  '*',  0,    0,    'A',  'S',    // 0x18 - 0x1F
    'D',  'F',  'G',  'H',  'J',  'K',  'L',  'O',    // 0x20 - 0x27
    'A',  '\'', 0,    '>',  'Y',  'X',  'C',  'V',    // 0x28 - 0x2F
    'B',  'N',  'M',  ';',  ':',  '_',  0,    '*',    // 0x30 - 0x37
    0,    ' ',  0,    0,    0,    0,    0,    0       // 0x38 - 0x3F
};


int char_pos = 0;
int shift_pressed = 0;

void keyboard_handler(void) {
    unsigned char scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) {
      shift_pressed = 1;
      return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
      shift_pressed = 0;
      return;
    }

    // Ignore break codes
    if (scancode & 0x80)
      return;
    

  char character = shift_pressed
    ? german_uppercase[scancode]
    : german_lowercase[scancode];
  
  if (!character)
    return;
  
  // vga_draw_char(char_pos, 0, character, Red);
  
  char_pos += 8;
}

