#include "../Include/Kbd.h"

bool Kbd_CheckIfBreakCode(uint16_t scanCode) {
  return (scanCode & 0xff) & 0x80;
}
