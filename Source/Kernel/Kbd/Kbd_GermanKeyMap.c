#include "../Include/Kbd.h"

static const KeyMap _Kbd_GermanKeyMap = {
  [KEY_1] = { '1', '!', 0, 0 },
  [KEY_2] = { '2', '"', 0, 0 },
  [KEY_3] = { '3', '§', 0, 0 },
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
  [KEY_E] = { 'e', 'E', 0, '€' },
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
  [KEY_Y] = { 'y', 'Y', 0, 0 },
  [KEY_Z] = { 'z', 'Z', 0, 0 },
  [KEY_SPACE] = { ' ', ' ', ' ', ' ' },
  [KEY_PERIOD] = { '.', ':', 0, 0 },
  [KEY_COMMA] = { ',', ';', 0, 0 },
  [KEY_MINUS] = { '-', '_', 0, 0 }
};

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
}
