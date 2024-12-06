#include "../Include/Kbd.h"

static inline uint8_t ReadFromPort(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static uint8_t _LastScanCode = 0;

uint16_t Kbd_ReadScanCode(void) {
  uint8_t portValue = ReadFromPort(0x60);

  if (portValue == 0xe0) {
    _LastScanCode = 0xe0;
    return 0;
  }

  if (_LastScanCode == 0xe0) {
    _LastScanCode = 0;
    return 0xe000 | portValue;
  } else {
    _LastScanCode = portValue;
    return portValue;
  }
}
