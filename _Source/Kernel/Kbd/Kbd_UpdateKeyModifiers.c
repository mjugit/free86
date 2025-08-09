#include "../Include/Kbd.h"

/**
 * @brief Updates the key modifiers based on the provided key event.
 *
 * This function modifies the `KeyModifiers` flags to reflect the current state
 * (pressed or released) of modifier keys such as Control, Alt, and Shift.
 *
 * @param keyCode The `KeyCode` of the key involved in the event.
 * @param keyDown `true` if the key is pressed; `false` if it is released.
 * @param modifiers A pointer to the `KeyModifiers` structure to update.
 */
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
