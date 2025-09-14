/*
	
  Copyright © 2025 Maximilian Jung

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the “Software”), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
*/


#include "../Include/Keyboard.h"


static const KeyMapEntry _GermanKeyMap[0xff] = {
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


char _Keyboard_GetCharImplementation(KeyCode keyCode, KeyModifiers modifiers) {
  KeyMapEntry keyMapEntry = _GermanKeyMap[keyCode];

  if (!modifiers)
    return keyMapEntry.Normal;

  if (modifiers & Shift)
    return keyMapEntry.Shift;

  if (modifiers & Alt)
    return keyMapEntry.Alt;

  if (modifiers & Ctrl)
    return keyMapEntry.Ctrl;

  return 0;
}
