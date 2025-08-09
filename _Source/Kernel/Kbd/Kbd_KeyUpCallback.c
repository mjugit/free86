#include "../Include/Kbd.h"

/**
 * @brief Static variable to store the callback function for key up events.
 */
static KeyEventHandler _KeyUpCallback;


/**
 * @brief Registers a callback function for key up events.
 *
 * This function allows the user to specify a custom callback that will
 * be invoked whenever a key up event occurs.
 *
 * @param callback A function pointer of type `KeyEventHandler` to handle key up events.
 */
void Kbd_SetKeyUpCallback(KeyEventHandler callback) {
    _KeyUpCallback = callback;
}


/**
 * @brief Retrieves the currently registered key up callback.
 *
 * This function returns the previously registered callback function for
 * key up events. If no callback is set, it returns `NULL`.
 *
 * @return The currently registered `KeyEventHandler` for key up events.
 */
KeyEventHandler Kbd_GetKeyUpCallback(void) {
    return _KeyUpCallback;
}
