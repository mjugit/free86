#include "../Include/Kbd.h"

static KeyEventHandler _KeyUpCallback;

void Kbd_SetKeyUpCallback(KeyEventHandler callback) {
  _KeyUpCallback = callback;
}

KeyEventHandler Kbd_GetKeyUpCallback(void) {
  return _KeyUpCallback;
}
