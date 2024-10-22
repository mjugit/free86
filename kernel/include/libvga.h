#ifndef _LIBVGA_H_
#define _LIBVGA_H_

#include <stdint.h>
#include "font_mono.h"

// Sequencer ports
#define VGA_SEQUENCER_INDEX 0x3c4
#define VGA_SEQUENCER_DATA 0x3c5

// Graphics controller ports
#define VGA_GRAPHICS_INDEX 0x3ce
#define VGA_GRAPHICS_DATA 0x3cf

// Attribute controller ports
#define VGA_ATTRIBUTE_INDEX 0x3c0
#define VGA_ATTRIBUTE_DATA 0x3c1

// CRT controller ports
#define VGA_CRT_INDEX 0x3d4
#define VGA_CRT_DATA 0x3d5

// DAC controller ports
#define VGA_DAC_INDEX 0x3c8
#define VGA_DAC_DATA 0x3c9


#define VGA_STATUS 0x3da


// Screen resolution
#define VGA_SCREEN_X 640
#define VGA_SCREEN_Y 480


// Video memory address
#define VIDEO_MEMORY_ADDRESS 0xa0000



typedef enum {
  Black = 0,
  Red = 1,
  Green = 2,
  Yellow = 3,
  Blue = 4,
  Purple = 5,
  Cyan = 6,
  LightGray = 7,
  DarkGray = 8,
  LightRed = 9,
  LightGreen = 10,
  LightYellow = 11,
  LightBlue = 12,
  LightPurple = 13,
  LightCyan = 14,
  White = 15
} color_t;


extern void vga_set_plane_mask(uint8_t plane_mask);
extern void vga_set_bit_mask(uint8_t bit_mask);
extern void vga_draw_pixel(uint16_t x, uint16_t y, color_t color);
extern void vga_draw_rect(uint16_t startx, uint16_t starty, uint16_t sizex, uint16_t sizey, color_t color);
extern void vga_draw_char(uint16_t startx, uint16_t starty, const char character, color_t color);

extern void vga_set_palette(color_t color, uint8_t palette_index);
extern void vga_redefine_palette(uint8_t palette_index, uint8_t red, uint8_t green, uint8_t blue);
extern void vga_load_palette(void);

#endif
