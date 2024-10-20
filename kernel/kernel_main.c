#include <stdint.h>
#include "include/libgfx.h"

void kernel_main(void) {
   volatile uint8_t* video_memory = (uint8_t*) 0xa0000;

   for (int i = 0; i < 480 * 640; i++) {
    video_memory[i] = 0x42;
   }

   gfx_draw_rect(0x1c, 0, 0, SCREEN_WIDTH, 16);

   gfx_draw_rect(0x1e, 1, 1, 1, 12);
   gfx_draw_rect(0x1e, 1, 1, 201, 1);

   gfx_draw_rect(0x1b, 202, 1, 1, 12);
   gfx_draw_rect(0x1b, 2, 12, 200, 1);
   
   gfx_draw_rect(0x1d, 2, 2, 200, 10);


   gfx_putstr(0x00, "This is a test", 2, 2);
   gfx_putstr(0x03, "Hello, world!\0", 10, 20);
  
  //  video_memory[0] = 'A';
  //  video_memory[1] = 0x04;

  return;
}
