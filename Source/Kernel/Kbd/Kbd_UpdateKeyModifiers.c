#include "../Include/Kbd.h"

void Kbd_UpdateKeyModifiers(KeyCode keyCode, bool keyDown, KeyModifiers *modifiers) {
    switch (keyCode) {
    case KEY_LCTRL:
    case KEY_RCTRL:
      if (!keyDown)
	*modifiers |= Ctrl;
      else
	*modifiers &= ~Ctrl;
      return;

    case KEY_LALT:
    case KEY_RALT:
      if (!keyDown)
	*modifiers |= Alt;
      else
	*modifiers &= ~Alt;
      return;;

    case KEY_LSHIFT:
    case KEY_RSHIFT:
      if (!keyDown)
	*modifiers |= Shift;
      else
	*modifiers &= ~Shift;
      return;;
    }
}
