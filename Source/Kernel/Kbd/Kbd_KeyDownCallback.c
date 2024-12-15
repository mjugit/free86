#include "../Include/Kbd.h"

/**
 * @brief Static variable to store the callback function for key down events.
 */
static KeyEventHandler _KeyDownCallback;


/**
 * @brief Registers a callback function for key down events.
 *
 * This function allows the user to specify a custom callback that will
 * be invoked whenever a key down event occurs.
 *
 * @param callback A function pointer of type `KeyEventHandler` to handle key down events.
 */
void Kbd_SetKeyDownCallback(KeyEventHandler callback) {
    _KeyDownCallback = callback;
}


/**
 * @brief Retrieves the currently registered key down callback.
 *
 * This function returns the previously registered callback function for
 * key down events. If no callback is set, it returns `NULL`.
 *
 * @return The currently registered `KeyEventHandler` for key down events.
 */
KeyEventHandler Kbd_GetKeyDownCallback(void) {
    return _KeyDownCallback;
}
