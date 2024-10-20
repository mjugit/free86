#include <stdint.h>

void kernel_main(void) {
  volatile uint8_t* video_memory = (uint8_t*) 0xB8000;

  video_memory[0] = 'A';
  video_memory[1] = 0x04;

  return;
}
