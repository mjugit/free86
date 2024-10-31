#include "../include/libint.h"


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
      shift_pressed = 1;  // Shift-Taste gedrückt
      return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
      shift_pressed = 0;  // Shift-Taste losgelassen
      return;
    }

    // Ignoriere Break Codes
    if (scancode & 0x80)
      return;
    

  char character = shift_pressed
    ? german_uppercase[scancode]
    : german_lowercase[scancode];
  
  if (!character)
    return;
  
  vga_draw_char(char_pos, 0, character, Red);
  
  char_pos += 8;
}
