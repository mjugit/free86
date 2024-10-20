#include "../include/libgfx.h"



static volatile uint8_t* video_memory = (uint8_t*) 0xa0000;


void gfx_draw_rect(color8_t color, short top_x, short top_y, short size_x, short size_y) {
  for (short row =0; row < size_y; row++) {
    short current_row = top_y + row;

    for (short col = 0; col < size_x; col++) {
      short current_col = top_x + col;

      video_memory[current_row * SCREEN_WIDTH + current_col] = color;
    }
  }
}

void gfx_putc(color8_t color, char character, short pos_x, short pos_y) {
  char *bitmap = monospace_font[(unsigned int)character];
  
  for (short row = 0; row < 8; row++) {
    short current_row = pos_y + row + 1;
    char bitmap_row = bitmap[row];

    for (short col = 0; col < 7; col++) {
      short current_col = pos_x + col + 1;
      char bitmask = 1 << (6 - col);
      
      if (bitmap_row & bitmask)
	video_memory[current_row * SCREEN_WIDTH + current_col] = color;
    }
  }
}

void gfx_putstr(color8_t color, const char *text, short pos_x, short pos_y) {
  short strlen = 0;

  while (text[strlen]) {
    gfx_putc(color, text[strlen], pos_x + strlen * 6, pos_y);
    strlen++;
  }
}





