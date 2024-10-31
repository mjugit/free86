#include <stdint.h>
#include <stdarg.h>


#include "include/libvga.h"
#include "include/string.h"
#include "include/libint.h"


#define KERNEL_CODE_SEGMENT 0x08
#define IDT_ENTRIES 256

extern char *_system_memory_map;

void print_mem_map_entry(unsigned int index, int startx, int starty) {

  char *base_address = _system_memory_map + (index * 16);
  
  char buff[100];
  uint32_t *base_address_ptr = (uint32_t*)base_address;
  string_format(buff, "Base Address (uint32_t) = %x", *base_address_ptr);
  vga_draw_string(startx, starty, buff, White);

  uint32_t *length_ptr = (uint32_t*)(base_address + 4);
  string_format(buff, "Length (uint32_t) = %x", *length_ptr);
  vga_draw_string(startx, starty + 10, buff, White);

  uint8_t *type_ptr = (uint8_t*)(base_address + 9);
  string_format(buff, "Type (uint8_t) = %xb", *type_ptr);
  vga_draw_string(startx, starty + 20, buff, White);

}


void kernel_main(void) {
  vga_load_palette();

  int_init_idt();
  int_init_pic(0xfd, 0xff);
  int_enable_interrupts();

  print_mem_map_entry(0, 0, 0);
  print_mem_map_entry(1, 0, 60);
  print_mem_map_entry(2, 0, 120);

    
 
  
  // vga_draw_testimage();

  while (1)
    __asm__ __volatile__("hlt");
 
  return;
}
