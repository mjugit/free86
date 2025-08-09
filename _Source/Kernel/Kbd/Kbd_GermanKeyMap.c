#include "../Include/Kbd.h"

/**
 * @brief A static German keyboard layout mapping.
 *
 * This mapping defines how keycodes correspond to characters on a German keyboard,
 * including variations for Shift, Alt, and Ctrl modifiers. The `KeyMapEntry` structure
 * supports up to four different mappings for each key:
 * - Normal: The default character.
 * - Shift: The character produced when the Shift modifier is active.
 * - Ctrl: The character produced when the Ctrl modifier is active.
 * - Alt: The character produced when the Alt modifier is active.
 */
static const KeyMap _Kbd_GermanKeyMap = {
  [KEY_1] = { '1', '!', 0, 0 },
  [KEY_2] = { '2', '"', 0, 0 },
  [KEY_3] = { '3', 0, 0, 0 },
  [KEY_4] = { '4', '$', 0, 0 },
  [KEY_5] = { '5', '%', 0, 0 },
  [KEY_6] = { '6', '&', 0, 0 },
  [KEY_7] = { '7', '/', 0, '{' },
  [KEY_8] = { '8', '(', 0, '[' },
  [KEY_9] = { '9', ')', 0, ']' },
  [KEY_0] = { '0', '=', 0, '}' },
  [KEY_A] = { 'a', 'A', 0, 0 },
  [KEY_B] = { 'b', 'B', 0, 0 },
  [KEY_C] = { 'c', 'C', 0, 0 },
  [KEY_D] = { 'd', 'D', 0, 0 },
  [KEY_E] = { 'e', 'E', 0, 0 },
  [KEY_F] = { 'f', 'F', 0, 0 },
  [KEY_G] = { 'g', 'G', 0, 0 },
  [KEY_H] = { 'h', 'H', 0, 0 },
  [KEY_I] = { 'i', 'I', 0, 0 },
  [KEY_J] = { 'j', 'J', 0, 0 },
  [KEY_K] = { 'k', 'K', 0, 0 },
  [KEY_L] = { 'l', 'L', 0, 0 },
  [KEY_M] = { 'm', 'M', 0, 0 },
  [KEY_N] = { 'n', 'N', 0, 0 },
  [KEY_O] = { 'o', 'O', 0, 0 },
  [KEY_P] = { 'p', 'P', 0, 0 },
  [KEY_Q] = { 'q', 'Q', 0, '@' },
  [KEY_R] = { 'r', 'R', 0, 0 },
  [KEY_S] = { 's', 'S', 0, 0 },
  [KEY_T] = { 't', 'T', 0, 0 },
  [KEY_U] = { 'u', 'U', 0, 0 },
  [KEY_V] = { 'v', 'V', 0, 0 },
  [KEY_W] = { 'w', 'W', 0, 0 },
  [KEY_X] = { 'x', 'X', 0, 0 },
  [KEY_Y] = { 'z', 'Z', 0, 0 },
  [KEY_Z] = { 'y', 'Y', 0, 0 },
  [KEY_SPACE] = { ' ', ' ', ' ', ' ' },
  [KEY_PERIOD] = { '.', ':', 0, 0 },
  [KEY_COMMA] = { ',', ';', 0, 0 },
  [KEY_SLASH] = { '-', '_', 0, 0 },
  [KEY_SEMICOLON] = { 'o', 'O', 0, 0},
  [KEY_GRAVE] = { 'a', 'A', 0, 0 },
  [KEY_BACKSLASH] = { '#', '\'', 0, 0 },
  [KEY_RBRACKET] = { '+', '*', 0, '~' },
  [KEY_LBRACKET] = { 'u', 'U', 0, 0 },
  [KEY_MINUS] = { '_', '?', 0, '\\'}
};


/**
 * @brief Gets the character for a keycode and modifier combination.
 *
 * This function retrieves the character from the German key map based on the given
 * `KeyCode` and the active modifiers. It prioritizes modifiers in the following order:
 * - Shift
 * - Alt
 * - Ctrl
 *
 * If no modifiers are active, the `Normal` entry from the map is returned.
 *
 * @param keyCode The keycode of the key to look up.
 * @param modifiers The current active key modifiers (Shift, Ctrl, Alt).
 * @return The character corresponding to the given keycode and modifiers, or `0` if no match is found.
 */
char Kbd_GetGermanKeyMapChar(KeyCode keyCode, KeyModifiers modifiers) {
  KeyMapEntry entry = _Kbd_GermanKeyMap[keyCode];
  if (!modifiers)
    return entry.Normal;
  
  if (modifiers & Shift)
    return entry.Shift;
  if (modifiers & Alt)
    return entry.Alt;
  if (modifiers & Ctrl)
    return entry.Ctrl;

  return 0;
}
