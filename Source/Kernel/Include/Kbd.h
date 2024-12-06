#ifndef _KBD_H_
#define _KBD_H_

#include <stdint.h>

#include "../../StandardLib/Include/CoreTypes.h"

/** @defgroup Scancodes IBM AT Keyboard Scancodes
 *  @{
 */

/** Scancode for the 'Esc' key. */
#define SCANCODE_ESC 0x01
/** Scancode for the '1' key. */
#define SCANCODE_1 0x02
/** Scancode for the '2' key. */
#define SCANCODE_2 0x03
/** Scancode for the '3' key. */
#define SCANCODE_3 0x04
/** Scancode for the '4' key. */
#define SCANCODE_4 0x05
/** Scancode for the '5' key. */
#define SCANCODE_5 0x06
/** Scancode for the '6' key. */
#define SCANCODE_6 0x07
/** Scancode for the '7' key. */
#define SCANCODE_7 0x08
/** Scancode for the '8' key. */
#define SCANCODE_8 0x09
/** Scancode for the '9' key. */
#define SCANCODE_9 0x0A
/** Scancode for the '0' key. */
#define SCANCODE_0 0x0B
/** Scancode for the '-' key. */
#define SCANCODE_MINUS 0x0C
/** Scancode for the '=' key. */
#define SCANCODE_EQUALS 0x0D
/** Scancode for the 'Backspace' key. */
#define SCANCODE_BACKSPACE 0x0E
/** Scancode for the 'Tab' key. */
#define SCANCODE_TAB 0x0F
/** Scancode for the 'Q' key. */
#define SCANCODE_Q 0x10
/** Scancode for the 'W' key. */
#define SCANCODE_W 0x11
/** Scancode for the 'E' key. */
#define SCANCODE_E 0x12
/** Scancode for the 'R' key. */
#define SCANCODE_R 0x13
/** Scancode for the 'T' key. */
#define SCANCODE_T 0x14
/** Scancode for the 'Y' key. */
#define SCANCODE_Y 0x15
/** Scancode for the 'U' key. */
#define SCANCODE_U 0x16
/** Scancode for the 'I' key. */
#define SCANCODE_I 0x17
/** Scancode for the 'O' key. */
#define SCANCODE_O 0x18
/** Scancode for the 'P' key. */
#define SCANCODE_P 0x19
/** Scancode for the '[' key. */
#define SCANCODE_LBRACKET 0x1A
/** Scancode for the ']' key. */
#define SCANCODE_RBRACKET 0x1B
/** Scancode for the 'Enter' key. */
#define SCANCODE_ENTER 0x1C
/** Scancode for the 'Left Control' key. */
#define SCANCODE_LCTRL 0x1D
/** Scancode for the 'A' key. */
#define SCANCODE_A 0x1E
/** Scancode for the 'S' key. */
#define SCANCODE_S 0x1F
/** Scancode for the 'D' key. */
#define SCANCODE_D 0x20
/** Scancode for the 'F' key. */
#define SCANCODE_F 0x21
/** Scancode for the 'G' key. */
#define SCANCODE_G 0x22
/** Scancode for the 'H' key. */
#define SCANCODE_H 0x23
/** Scancode for the 'J' key. */
#define SCANCODE_J 0x24
/** Scancode for the 'K' key. */
#define SCANCODE_K 0x25
/** Scancode for the 'L' key. */
#define SCANCODE_L 0x26
/** Scancode for the ';' key. */
#define SCANCODE_SEMICOLON 0x27
/** Scancode for the '\'' key. */
#define SCANCODE_APOSTROPHE 0x28
/** Scancode for the '`' key. */
#define SCANCODE_GRAVE 0x29
/** Scancode for the 'Left Shift' key. */
#define SCANCODE_LSHIFT 0x2A
/** Scancode for the '\\' key. */
#define SCANCODE_BACKSLASH 0x2B
/** Scancode for the 'Z' key. */
#define SCANCODE_Z 0x2C
/** Scancode for the 'X' key. */
#define SCANCODE_X 0x2D
/** Scancode for the 'C' key. */
#define SCANCODE_C 0x2E
/** Scancode for the 'V' key. */
#define SCANCODE_V 0x2F
/** Scancode for the 'B' key. */
#define SCANCODE_B 0x30
/** Scancode for the 'N' key. */
#define SCANCODE_N 0x31
/** Scancode for the 'M' key. */
#define SCANCODE_M 0x32
/** Scancode for the ',' key. */
#define SCANCODE_COMMA 0x33
/** Scancode for the '.' key. */
#define SCANCODE_PERIOD 0x34
/** Scancode for the '/' key. */
#define SCANCODE_SLASH 0x35
/** Scancode for the 'Right Shift' key. */
#define SCANCODE_RSHIFT 0x36
/** Scancode for the '*' key on the numeric keypad. */
#define SCANCODE_KP_ASTERISK 0x37
/** Scancode for the 'Left Alt' key. */
#define SCANCODE_LALT 0x38
/** Scancode for the 'Spacebar'. */
#define SCANCODE_SPACE 0x39
/** Scancode for the 'Caps Lock' key. */
#define SCANCODE_CAPSLOCK 0x3A
/** Scancode for the 'F1' key. */
#define SCANCODE_F1 0x3B
/** Scancode for the 'F2' key. */
#define SCANCODE_F2 0x3C
/** Scancode for the 'F3' key. */
#define SCANCODE_F3 0x3D
/** Scancode for the 'F4' key. */
#define SCANCODE_F4 0x3E
/** Scancode for the 'F5' key. */
#define SCANCODE_F5 0x3F
/** Scancode for the 'F6' key. */
#define SCANCODE_F6 0x40
/** Scancode for the 'F7' key. */
#define SCANCODE_F7 0x41
/** Scancode for the 'F8' key. */
#define SCANCODE_F8 0x42
/** Scancode for the 'F9' key. */
#define SCANCODE_F9 0x43
/** Scancode for the 'F10' key. */
#define SCANCODE_F10 0x44
/** Scancode for the 'Num Lock' key. */
#define SCANCODE_NUMLOCK 0x45
/** Scancode for the 'Scroll Lock' key. */
#define SCANCODE_SCROLLLOCK 0x46
/** Scancode for the '7' key on the numeric keypad. */
#define SCANCODE_KP_7 0x47
/** Scancode for the '8' key on the numeric keypad. */
#define SCANCODE_KP_8 0x48
/** Scancode for the '9' key on the numeric keypad. */
#define SCANCODE_KP_9 0x49
/** Scancode for the '-' key on the numeric keypad. */
#define SCANCODE_KP_MINUS 0x4A
/** Scancode for the '4' key on the numeric keypad. */
#define SCANCODE_KP_4 0x4B
/** Scancode for the '5' key on the numeric keypad. */
#define SCANCODE_KP_5 0x4C
/** Scancode for the '6' key on the numeric keypad. */
#define SCANCODE_KP_6 0x4D
/** Scancode for the '+' key on the numeric keypad. */
#define SCANCODE_KP_PLUS 0x4E
/** Scancode for the '1' key on the numeric keypad. */
#define SCANCODE_KP_1 0x4F
/** Scancode for the '2' key on the numeric keypad. */
#define SCANCODE_KP_2 0x50
/** Scancode for the '3' key on the numeric keypad. */
#define SCANCODE_KP_3 0x51
/** Scancode for the '0' key on the numeric keypad. */
#define SCANCODE_KP_0 0x52
/** Scancode for the '.' key on the numeric keypad. */
#define SCANCODE_KP_PERIOD 0x53
/** Scancode for the 'F11' key. */
#define SCANCODE_F11 0x57
/** Scancode for the 'F12' key. */
#define SCANCODE_F12 0x58

/** Extended Scancodes (E0 prefix). */
/** Scancode for the 'Right Control' key. */
#define SCANCODE_RCTRL 0xE01D
/** Scancode for the 'Right Alt' (AltGr) key. */
#define SCANCODE_RALT 0xE038
/** Scancode for the 'Home' key. */
#define SCANCODE_HOME 0xE047
/** Scancode for the 'Up Arrow' key. */
#define SCANCODE_UP 0xE048
/** Scancode for the 'Page Up' key. */
#define SCANCODE_PAGEUP 0xE049
/** Scancode for the 'Left Arrow' key. */
#define SCANCODE_LEFT 0xE04B
/** Scancode for the 'Right Arrow' key. */
#define SCANCODE_RIGHT 0xE04D
/** Scancode for the 'End' key. */
#define SCANCODE_END 0xE04F
/** Scancode for the 'Down Arrow' key. */
#define SCANCODE_DOWN 0xE050
/** Scancode for the 'Page Down' key. */
#define SCANCODE_PAGEDOWN 0xE051
/** Scancode for the 'Insert' key. */
#define SCANCODE_INSERT 0xE052
/** Scancode for the 'Delete' key. */
#define SCANCODE_DELETE 0xE053


#define BREAK_CODE(scanCode) ((scanCode) + 0x80)

/** @} */ // End of Scancodes group




/** @defgroup Keycodes Abstract Keycodes
 *  @{
 */

/** No key pressed (reserved for special use). */
#define KEY_NONE 0x00
/** Error state or undefined key. */
#define KEY_ERROR 0x01
/** Reserved for future use. */
#define KEY_RESERVED 0x02
/** Another reserved keycode. */
#define KEY_SYSTEM 0x03

/** Keycode for the 'A' key. */
#define KEY_A 0x04
/** Keycode for the 'B' key. */
#define KEY_B 0x05
/** Keycode for the 'C' key. */
#define KEY_C 0x06
/** Keycode for the 'D' key. */
#define KEY_D 0x07
/** Keycode for the 'E' key. */
#define KEY_E 0x08
/** Keycode for the 'F' key. */
#define KEY_F 0x09
/** Keycode for the 'G' key. */
#define KEY_G 0x0A
/** Keycode for the 'H' key. */
#define KEY_H 0x0B
/** Keycode for the 'I' key. */
#define KEY_I 0x0C
/** Keycode for the 'J' key. */
#define KEY_J 0x0D
/** Keycode for the 'K' key. */
#define KEY_K 0x0E
/** Keycode for the 'L' key. */
#define KEY_L 0x0F
/** Keycode for the 'M' key. */
#define KEY_M 0x10
/** Keycode for the 'N' key. */
#define KEY_N 0x11
/** Keycode for the 'O' key. */
#define KEY_O 0x12
/** Keycode for the 'P' key. */
#define KEY_P 0x13
/** Keycode for the 'Q' key. */
#define KEY_Q 0x14
/** Keycode for the 'R' key. */
#define KEY_R 0x15
/** Keycode for the 'S' key. */
#define KEY_S 0x16
/** Keycode for the 'T' key. */
#define KEY_T 0x17
/** Keycode for the 'U' key. */
#define KEY_U 0x18
/** Keycode for the 'V' key. */
#define KEY_V 0x19
/** Keycode for the 'W' key. */
#define KEY_W 0x1A
/** Keycode for the 'X' key. */
#define KEY_X 0x1B
/** Keycode for the 'Y' key. */
#define KEY_Y 0x1C
/** Keycode for the 'Z' key. */
#define KEY_Z 0x1D

/** Keycode for the '1' key. */
#define KEY_1 0x1E
/** Keycode for the '2' key. */
#define KEY_2 0x1F
/** Keycode for the '3' key. */
#define KEY_3 0x20
/** Keycode for the '4' key. */
#define KEY_4 0x21
/** Keycode for the '5' key. */
#define KEY_5 0x22
/** Keycode for the '6' key. */
#define KEY_6 0x23
/** Keycode for the '7' key. */
#define KEY_7 0x24
/** Keycode for the '8' key. */
#define KEY_8 0x25
/** Keycode for the '9' key. */
#define KEY_9 0x26
/** Keycode for the '0' key. */
#define KEY_0 0x27

/** Keycode for the 'Enter' key. */
#define KEY_ENTER 0x28
/** Keycode for the 'Escape' key. */
#define KEY_ESC 0x29
/** Keycode for the 'Backspace' key. */
#define KEY_BACKSPACE 0x2A
/** Keycode for the 'Tab' key. */
#define KEY_TAB 0x2B
/** Keycode for the 'Space' key. */
#define KEY_SPACE 0x2C

/** Keycode for the '-' key. */
#define KEY_MINUS 0x2D
/** Keycode for the '=' key. */
#define KEY_EQUALS 0x2E
/** Keycode for the '[' key. */
#define KEY_LBRACKET 0x2F
/** Keycode for the ']' key. */
#define KEY_RBRACKET 0x30
/** Keycode for the '\\' key. */
#define KEY_BACKSLASH 0x31
/** Keycode for the ';' key. */
#define KEY_SEMICOLON 0x32
/** Keycode for the '\'' key. */
#define KEY_APOSTROPHE 0x33
/** Keycode for the '`' key. */
#define KEY_GRAVE 0x34
/** Keycode for the ',' key. */
#define KEY_COMMA 0x35
/** Keycode for the '.' key. */
#define KEY_PERIOD 0x36
/** Keycode for the '/' key. */
#define KEY_SLASH 0x37
/** Keycode for the 'Caps Lock' key. */
#define KEY_CAPSLOCK 0x38
/** Keycode for the 'F1' key. */
#define KEY_F1 0x39
/** Keycode for the 'F2' key. */
#define KEY_F2 0x3A
/** Keycode for the 'F3' key. */
#define KEY_F3 0x3B
/** Keycode for the 'F4' key. */
#define KEY_F4 0x3C
/** Keycode for the 'F5' key. */
#define KEY_F5 0x3D
/** Keycode for the 'F6' key. */
#define KEY_F6 0x3E
/** Keycode for the 'F7' key. */
#define KEY_F7 0x3F
/** Keycode for the 'F8' key. */
#define KEY_F8 0x40
/** Keycode for the 'F9' key. */
#define KEY_F9 0x41
/** Keycode for the 'F10' key. */
#define KEY_F10 0x42
/** Keycode for the 'F11' key. */
#define KEY_F11 0x43
/** Keycode for the 'F12' key. */
#define KEY_F12 0x44

/** Keycode for the 'Print Screen' key. */
#define KEY_PRINTSCREEN 0x45
/** Keycode for the 'Scroll Lock' key. */
#define KEY_SCROLLLOCK 0x46
/** Keycode for the 'Pause' key. */
#define KEY_PAUSE 0x47
/** Keycode for the 'Insert' key. */
#define KEY_INSERT 0x48
/** Keycode for the 'Home' key. */
#define KEY_HOME 0x49
/** Keycode for the 'Page Up' key. */
#define KEY_PAGEUP 0x4A
/** Keycode for the 'Delete' key. */
#define KEY_DELETE 0x4B
/** Keycode for the 'End' key. */
#define KEY_END 0x4C
/** Keycode for the 'Page Down' key. */
#define KEY_PAGEDOWN 0x4D
/** Keycode for the 'Right Arrow' key. */
#define KEY_RIGHT 0x4E
/** Keycode for the 'Left Arrow' key. */
#define KEY_LEFT 0x4F
/** Keycode for the 'Down Arrow' key. */
#define KEY_DOWN 0x50
/** Keycode for the 'Up Arrow' key. */
#define KEY_UP 0x51

/** Keycode for the 'Num Lock' key. */
#define KEY_NUMLOCK 0x52
/** Keycode for the 'Keypad /' key. */
#define KEY_KP_SLASH 0x53
/** Keycode for the 'Keypad *' key. */
#define KEY_KP_ASTERISK 0x54
/** Keycode for the 'Keypad -' key. */
#define KEY_KP_MINUS 0x55
/** Keycode for the 'Keypad +' key. */
#define KEY_KP_PLUS 0x56
/** Keycode for the 'Keypad Enter' key. */
#define KEY_KP_ENTER 0x57
/** Keycode for the 'Keypad 1' key. */
#define KEY_KP_1 0x58
/** Keycode for the 'Keypad 2' key. */
#define KEY_KP_2 0x59
/** Keycode for the 'Keypad 3' key. */
#define KEY_KP_3 0x5A
/** Keycode for the 'Keypad 4' key. */
#define KEY_KP_4 0x5B
/** Keycode for the 'Keypad 5' key. */
#define KEY_KP_5 0x5C
/** Keycode for the 'Keypad 6' key. */
#define KEY_KP_6 0x5D
/** Keycode for the 'Keypad 7' key. */
#define KEY_KP_7 0x5E
/** Keycode for the 'Keypad 8' key. */
#define KEY_KP_8 0x5F
/** Keycode for the 'Keypad 9' key. */
#define KEY_KP_9 0x60
/** Keycode for the 'Keypad 0' key. */
#define KEY_KP_0 0x61
/** Keycode for the 'Keypad .' key. */
#define KEY_KP_PERIOD 0x62

/** Keycode for the 'Left Control' key. */
#define KEY_LCTRL 0x63
/** Keycode for the 'Left Shift' key. */
#define KEY_LSHIFT 0x64
/** Keycode for the 'Left Alt' key. */
#define KEY_LALT 0x65
/** Keycode for the 'Left GUI/Windows' key. */
#define KEY_LGUI 0x66
/** Keycode for the 'Right Control' key. */
#define KEY_RCTRL 0x67
/** Keycode for the 'Right Shift' key. */
#define KEY_RSHIFT 0x68
/** Keycode for the 'Right Alt' (AltGr) key. */
#define KEY_RALT 0x69
/** Keycode for the 'Right GUI/Windows' key. */
#define KEY_RGUI 0x6A

/** @} */ // End of Keycodes group



/** @brief A numerical identifier representing a specifc key on the keyboard. */
typedef uint8_t KeyCode;


/** @brief Bit flags representing active key modifiers (shift, ctrl, alt). */
typedef enum {
  None	= 0,
  Shift = 1 << 0,
  Ctrl	= 1 << 1,
  Alt	= 1 << 2
} KeyModifiers;


/** @brief Event argument structure for keyboard events. */
typedef struct {
  KeyModifiers *Modifiers;
  KeyCode KeyCode;
  bool KeyDown;
} KeyEventArgs;


typedef struct {
  char Normal;
  char Shift;
  char Ctrl;
  char Alt;
} KeyMapEntry;

typedef KeyMapEntry KeyMap[256];


/** @brief An event handler for keyboard events. */
typedef void (*KeyEventHandler)(KeyEventArgs);


extern void Kbd_SetKeyDownCallback(KeyEventHandler callback);
extern KeyEventHandler Kbd_GetKeyDownCallback(void);
extern void Kbd_SetKeyUpCallback(KeyEventHandler callback);
extern KeyEventHandler Kbd_GetKeyUpCallback(void);
extern void Kbd_UpdateKeyModifiers(KeyCode keyCode, bool keyDown, KeyModifiers *modifiers);
extern uint16_t Kbd_ReadScanCode(void);
extern KeyCode Kbd_GetKeyCode(uint16_t scanCode);
extern bool Kbd_CheckIfBreakCode(uint16_t scanCode);
extern void Kbd_HandleKeyPress(void);
extern char Kbd_GetGermanKeyMapChar(KeyCode keyCode, KeyModifiers modifiers);

#endif
