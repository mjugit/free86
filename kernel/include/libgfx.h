#ifndef _LIBGFX_H_
#define _LIBGFX_H_

#include <stdint.h>
#include "font_mono.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200


typedef uint8_t color8_t;


extern void gfx_draw_rect(color8_t color, short top_x, short top_y, short size_x, short size_y);

extern void gfx_putc(color8_t color, char character, short pos_x, short pos_y);
extern void gfx_putstr(color8_t color, const char *text, short pos_x, short pos_y);


#endif
