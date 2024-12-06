#include "../Include/Kbd.h"

static KeyEventHandler _KeyDownCallback;

void Kbd_SetKeyDownCallback(KeyEventHandler callback) {
  _KeyDownCallback = callback;
}

KeyEventHandler Kbd_GetKeyDownCallback(void) {
  return _KeyDownCallback;
}
