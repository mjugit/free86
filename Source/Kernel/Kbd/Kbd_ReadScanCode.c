#include "../Include/Kbd.h"

/**
 * @brief Reads a single byte from the specified I/O port.
 *
 * This function performs a low-level read from the given hardware port.
 *
 * @param port The I/O port to read from.
 * @return The byte value read from the port.
 */
static inline uint8_t _Kbd_ReadFromPort(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}


/**
 * @brief Stores the last read scancode to handle extended scancodes.
 */
static uint8_t _LastScanCode = 0;


/**
 * @brief Reads the current scancode from the keyboard's I/O port.
 *
 * This function reads raw scancodes from the keyboard. It handles extended scancodes
 * (prefixed with `0xE0`) by combining them into a 16-bit representation:
 * - Extended scancodes are returned as `0xE000 | scancode`.
 * - Single-byte scancodes are returned as-is.
 * - If the prefix `0xE0` is read, the function returns `0` and waits for the next scancode.
 *
 * @return A 16-bit scancode. Returns `0` if the function is processing an extended prefix.
 */
uint16_t Kbd_ReadScanCode(void) {
  uint8_t portValue = _Kbd_ReadFromPort(0x60);

  // Handle extended scancode prefix
  if (portValue == 0xE0) {
    _LastScanCode = 0xE0;
    // Indicate that the prefix has been read
    return 0;
  }

  // Combine extended prefix with the next scancode
  if (_LastScanCode == 0xE0) {
    // Reset prefix state
    _LastScanCode = 0;
    
    return 0xE000 | portValue;
  } else {
    // Normal scancode
    _LastScanCode = portValue;
    return portValue;
  }
}
