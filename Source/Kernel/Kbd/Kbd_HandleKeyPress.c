#include "../Include/Kbd.h"

/**
 * @brief Stores the current state of active key modifiers.
 *
 * This variable maintains the current state of key modifiers (Shift, Ctrl, Alt),
 * allowing the system to keep track of changes as keys are pressed or released.
 */
static KeyModifiers _Kbd_CurrentModifiers;


/**
 * @brief Handles a key press or release event from the keyboard.
 *
 * This function reads the next scancode from the keyboard, determines whether the
 * associated key was pressed or released, and updates the modifier state accordingly.
 * It then constructs a `KeyEventArgs` structure and invokes the appropriate callback
 * (key down or key up) if one is registered.
 */
void Kbd_HandleKeyPress(void) {
  uint16_t scanCode = Kbd_ReadScanCode();
  if (!scanCode)
    return;
  
  bool wasKeyDown = Kbd_CheckIfBreakCode(scanCode);
  uint8_t keyCode = Kbd_GetKeyCode(wasKeyDown ? (scanCode - 0x80) : scanCode);
  Kbd_UpdateKeyModifiers(keyCode, wasKeyDown, &_Kbd_CurrentModifiers);

  KeyEventArgs eventArgs = {
    .KeyCode = keyCode,
    .KeyDown = wasKeyDown,
    .Modifiers = &_Kbd_CurrentModifiers
  };
  
  KeyEventHandler eventHandler = wasKeyDown
    ? Kbd_GetKeyDownCallback()
    : Kbd_GetKeyUpCallback();

  if (eventHandler)
    eventHandler(eventArgs);
}
