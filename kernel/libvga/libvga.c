#include "../include/libvga.h"

static volatile uint8_t *_video_memory = (uint8_t*) VIDEO_MEMORY_ADDRESS;


inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}


void vga_set_plane_mask(uint8_t plane_mask) {
  const uint8_t PLANE_MASK_INDEX = 0x02;
  
  outb(VGA_SEQUENCER_INDEX, PLANE_MASK_INDEX);
  outb(VGA_SEQUENCER_DATA, plane_mask);
}

void vga_set_bit_mask(uint8_t bit_mask) {
  const uint8_t BITMASK_INDEX = 0x08;

  outb(VGA_GRAPHICS_INDEX, BITMASK_INDEX);
  outb(VGA_GRAPHICS_DATA, bit_mask);
}



void vga_draw_pixel(uint16_t x, uint16_t y, color_t color) {
  uint16_t bytes_per_row = VGA_SCREEN_X / 8;
  uint16_t byte_offset = (y * bytes_per_row) + (x / 8);

  uint8_t bit_position = x % 8;
  uint8_t bit_mask = 1 << (7 - bit_position);

  vga_set_plane_mask(color);
  vga_set_bit_mask(bit_mask);

  uint8_t* effective_address = (uint8_t*)_video_memory + byte_offset;
  *effective_address |= bit_mask;
}

void vga_draw_rect(uint16_t startx, uint16_t starty, uint16_t sizex, uint16_t sizey, color_t color) {
  for (uint16_t posy = starty; posy < starty + sizey; posy++)
    for (uint16_t posx = startx; posx < startx + sizex; posx++)
      vga_draw_pixel(posx, posy, color);
}

void vga_draw_char(uint16_t startx, uint16_t starty, const char character, color_t color) {
    char *bitmap = monospace_font[(unsigned int)character];
  
  for (short row = 0; row < 8; row++) {
    short current_row = starty + row + 1;
    char bitmap_row = bitmap[row];

    for (short col = 0; col < 7; col++) {
      short current_col = startx + col + 1;
      char bitmask = 1 << (6 - col);
      
      if (bitmap_row & bitmask)
	vga_draw_pixel(current_col, current_row, color);
    }
  }
}

void vga_set_palette(color_t color, uint8_t palette_index) {
  // REMARK:
  // The inb() calls below are used to enable the VGA controller before writing.

  // Redefine the color
  inb(VGA_STATUS);
  outb(VGA_ATTRIBUTE_INDEX, (color & 0x1f));
  outb(VGA_ATTRIBUTE_INDEX, (palette_index & 0xff));

  // Set video enable bit
  inb(VGA_STATUS);
  outb(VGA_ATTRIBUTE_INDEX, 0x20);
}

void vga_redefine_palette(uint8_t palette_index, uint8_t red, uint8_t green, uint8_t blue) {
  outb(VGA_DAC_INDEX, palette_index);
  outb(VGA_DAC_DATA, red);
  outb(VGA_DAC_DATA, green);
  outb(VGA_DAC_DATA, blue);
}

void vga_load_palette(void) {
  vga_redefine_palette(0, 15, 16, 20);
  vga_set_palette(Black, 0);
  
  vga_redefine_palette(1, 45, 22, 24);
  vga_set_palette(Red, 1);
  
  vga_redefine_palette(2, 38, 45, 32);
  vga_set_palette(Green, 2);
  
  vga_redefine_palette(3, 56, 48, 32);
  vga_set_palette(Yellow, 3);
  
  vga_redefine_palette(4, 29, 37, 45);
  vga_set_palette(Blue, 4);
  
  vga_redefine_palette(5, 42, 33, 40);
  vga_set_palette(Purple, 5);
  
  vga_redefine_palette(6, 31, 45, 49);
  vga_set_palette(Cyan, 6);
  
  vga_redefine_palette(7, 54, 55, 57);
  vga_set_palette(LightGray, 7);
  
  vga_redefine_palette(8, 18, 21, 26);
  vga_set_palette(DarkGray, 8);
  
  vga_redefine_palette(9, 53, 31, 33);
  vga_set_palette(LightRed, 9);
  
  vga_redefine_palette(10, 47, 54, 41);
  vga_set_palette(LightGreen, 10);
  
  vga_redefine_palette(11, 63, 57, 41);
  vga_set_palette(LightYellow, 11);
  
  vga_redefine_palette(12, 38, 47, 54);
  vga_set_palette(LightBlue, 12);
  
  vga_redefine_palette(13, 51, 44, 49);
  vga_set_palette(LightPurple, 13);
  
  vga_redefine_palette(14, 42, 53, 53);
  vga_set_palette(LightCyan, 14);
  
  vga_redefine_palette(15, 63, 63, 63);
  vga_set_palette(White, 15);
}




