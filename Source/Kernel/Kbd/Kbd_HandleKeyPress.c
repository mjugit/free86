#include "../Include/Kbd.h"

static KeyModifiers _Kbd_CurrentModifiers;

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
