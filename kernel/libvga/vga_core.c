#include "../include/libvga.h"

// A pointer to the video memory
static volatile uint8_t *_video_memory = (uint8_t*) VIDEO_MEMORY_ADDRESS;

// Helper method for writing to a port
inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Helper method for reading from a port
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}


// Set the plane mask to the given value
void vga_set_plane_mask(uint8_t plane_mask) {
  const uint8_t PLANE_MASK_INDEX = 0x02;
  
  outb(VGA_SEQUENCER_INDEX, PLANE_MASK_INDEX);
  outb(VGA_SEQUENCER_DATA, plane_mask);
}

// Set the bit mask to the given value 
void vga_set_bit_mask(uint8_t bit_mask) {
  const uint8_t BITMASK_INDEX = 0x08;

  outb(VGA_GRAPHICS_INDEX, BITMASK_INDEX);
  outb(VGA_GRAPHICS_DATA, bit_mask);
}


// Draw a single pixel at the coordinates in a specific color
void vga_draw_pixel(uint16_t x, uint16_t y, color_t color) {
  uint16_t bytes_per_row = VGA_SCREEN_X / 8;
  uint16_t byte_offset = (y * bytes_per_row) + (x / 8);
  uint8_t bit_position = x % 8;
  uint8_t bit_mask = 1 << (7 - bit_position);

  vga_set_plane_mask(0xf);
  vga_set_bit_mask(bit_mask);
  _video_memory[byte_offset] &= ~bit_mask;
  
  vga_set_plane_mask(color);
  _video_memory[byte_offset] |= bit_mask;
}


// Draw a rectangle in a specific color
void vga_draw_rect(uint16_t startx, uint16_t starty, uint16_t sizex, uint16_t sizey, color_t color) {
  for (uint16_t posy = starty; posy < starty + sizey; posy++)
    for (uint16_t posx = startx; posx < startx + sizex; posx++)
      vga_draw_pixel(posx, posy, color);
}

// Draw a character in a specific color
void vga_draw_char(uint16_t startx, uint16_t starty, const char character, color_t color) {
  uint8_t *bitmap = monospace_font[(uint8_t)character];
  
  for (uint16_t row = 0; row < 8; row++) {
    uint16_t current_row = starty + row + 1;
    uint8_t bitmap_row = bitmap[row];

    for (uint16_t col = 0; col < 7; col++) {
      uint16_t current_col = startx + col + 1;
      uint8_t bitmask = 1 << col;
      
      if (bitmap_row & bitmask)
	vga_draw_pixel(current_col, current_row, color);
    }
  }
}

// Draw a string in a specific color
void vga_draw_string(uint16_t  startx, uint16_t starty, const char *text, color_t color) {
  for (short index = 0; text[index]; index++)
    vga_draw_char(startx + (index * CHAR_WIDTH), starty, text[index], color);
}

// Draw a horizontal line
void vga_draw_hline(uint16_t startx, uint16_t starty, uint16_t sizex, color_t color) {
  for (uint16_t posx = startx; posx < startx + sizex; posx++)
    vga_draw_pixel(posx, starty, color);
}

// Draw a vertical line
void vga_draw_vline(uint16_t startx, uint16_t starty, uint16_t sizey, color_t color) {
  for (uint16_t posy = starty; posy < starty + sizey; posy++)
    vga_draw_pixel(startx, posy, color);
}


// Update the color mapping between the 16 color palette and the 256 colors to choose from
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

// Redefine a color in the global palette
// This method modifies the DAC of the monitor, so the changes are global!
void vga_redefine_palette(uint8_t palette_index, uint8_t red, uint8_t green, uint8_t blue) {
  outb(VGA_DAC_INDEX, palette_index);
  outb(VGA_DAC_DATA, red);
  outb(VGA_DAC_DATA, green);
  outb(VGA_DAC_DATA, blue);
}


// Load and configure the default system colors.
void vga_load_palette(void) {
  vga_redefine_palette(0, 9, 10, 11);
  vga_set_palette(Black, 0);
  
  vga_redefine_palette(1, 40, 16, 16);
  vga_set_palette(Red, 1);
  
  vga_redefine_palette(2, 35, 37, 15);
  vga_set_palette(Green, 2);
  
  vga_redefine_palette(3, 55, 36, 23);
  vga_set_palette(Yellow, 3);
  
  vga_redefine_palette(4, 23, 32, 39);
  vga_set_palette(Blue, 4);
  
  vga_redefine_palette(5, 33, 25, 35);
  vga_set_palette(Purple, 5);
  
  vga_redefine_palette(6, 23, 35, 33);
  vga_set_palette(Cyan, 6);
  
  vga_redefine_palette(7, 28, 30, 32);
  vga_set_palette(LightGray, 7);
  
  vga_redefine_palette(8, 13, 14, 16);
  vga_set_palette(DarkGray, 8);
  
  vga_redefine_palette(9, 50, 25, 25);
  vga_set_palette(LightRed, 9);
  
  vga_redefine_palette(10, 45, 47, 25);
  vga_set_palette(LightGreen, 10);
  
  vga_redefine_palette(11, 60, 49, 29);
  vga_set_palette(LightYellow, 11);
  
  vga_redefine_palette(12, 32, 40, 47);
  vga_set_palette(LightBlue, 12);
  
  vga_redefine_palette(13, 44, 37, 46);
  vga_set_palette(LightPurple, 13);
  
  vga_redefine_palette(14, 34, 47, 45);
  vga_set_palette(LightCyan, 14);
  
  vga_redefine_palette(15, 49, 50, 49);
  vga_set_palette(White, 15);
}

void vga_draw_testimage(void) {
  color_t color = 0;
  int posx = 100;
  int posy = 50;
  
  for (int row = 0; row < 2; row++) {
    for (int box = 0; box < 8; box++) {

      vga_draw_rect(posx, posy, 45, 45, color);
      posx += 50;
      color++;
    }
    posx = 100;
    posy += 50;
  }

  for (color_t textcolor = 0; textcolor < 16; textcolor++) {
    vga_draw_string(50, 200 + (textcolor * 14), "ABCDEFGHIJKLMNOPQRSTUVWXYZ\0", textcolor);
    vga_draw_string(300, 200 + (textcolor * 14), "abcdefghijklmnopqrstuvwxyz\0", textcolor);
  }
} 
