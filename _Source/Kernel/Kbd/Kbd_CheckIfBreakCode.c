#include "../Include/Kbd.h"

/**
 * @brief Checks if a given scancode represents a break code (key release).
 *
 * Break codes indicate that a key has been released. In the standard
 * keyboard scancode set, a break code is identified when the highest
 * bit (bit 7) of the low byte in the scancode is set (1).
 *
 * @param scanCode The 16-bit scancode to check.
 * @return `true` if the scancode is a break code, `false` otherwise.
 */
bool Kbd_CheckIfBreakCode(uint16_t scanCode) {
    return (scanCode & 0xFF) & 0x80;
}
