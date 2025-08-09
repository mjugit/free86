#ifndef _KBD_H_
#define _KBD_H_

#include <stdint.h>

#include "../../StandardLib/Include/CoreTypes.h"



/**
 * @defgroup Scancodes IBM AT Keyboard Scancodes
 * @brief Defines scancodes for the IBM AT keyboard.
 *
 * This group provides definitions for the IBM AT keyboard's scancodes,
 * including standard scancodes and extended scancodes. Scancodes are
 * used to identify key presses and releases. The `BREAK_CODE` macro
 * can be used to compute the release code (break code) for any key.
 * @{
 */

/**
 * @brief Scancode for the 'Esc' key.
 */
#define SCANCODE_ESC          0x01

/**
 * @brief Scancode for the '1' key.
 */
#define SCANCODE_1            0x02

/**
 * @brief Scancode for the '2' key.
 */
#define SCANCODE_2            0x03

/**
 * @brief Scancode for the '3' key.
 */
#define SCANCODE_3            0x04

/**
 * @brief Scancode for the '4' key.
 */
#define SCANCODE_4            0x05

/**
 * @brief Scancode for the '5' key.
 */
#define SCANCODE_5            0x06

/**
 * @brief Scancode for the '6' key.
 */
#define SCANCODE_6            0x07

/**
 * @brief Scancode for the '7' key.
 */
#define SCANCODE_7            0x08

/**
 * @brief Scancode for the '8' key.
 */
#define SCANCODE_8            0x09

/**
 * @brief Scancode for the '9' key.
 */
#define SCANCODE_9            0x0A

/**
 * @brief Scancode for the '0' key.
 */
#define SCANCODE_0            0x0B

/**
 * @brief Scancode for the '-' key.
 */
#define SCANCODE_MINUS        0x0C

/**
 * @brief Scancode for the '=' key.
 */
#define SCANCODE_EQUALS       0x0D

/**
 * @brief Scancode for the 'Backspace' key.
 */
#define SCANCODE_BACKSPACE    0x0E

/**
 * @brief Scancode for the 'Tab' key.
 */
#define SCANCODE_TAB          0x0F

/**
 * @brief Scancode for the 'Q' key.
 */
#define SCANCODE_Q            0x10

/**
 * @brief Scancode for the 'W' key.
 */
#define SCANCODE_W            0x11

/**
 * @brief Scancode for the 'E' key.
 */
#define SCANCODE_E            0x12

/**
 * @brief Scancode for the 'R' key.
 */
#define SCANCODE_R            0x13

/**
 * @brief Scancode for the 'T' key.
 */
#define SCANCODE_T            0x14

/**
 * @brief Scancode for the 'Y' key.
 */
#define SCANCODE_Y            0x15

/**
 * @brief Scancode for the 'U' key.
 */
#define SCANCODE_U            0x16

/**
 * @brief Scancode for the 'I' key.
 */
#define SCANCODE_I            0x17

/**
 * @brief Scancode for the 'O' key.
 */
#define SCANCODE_O            0x18

/**
 * @brief Scancode for the 'P' key.
 */
#define SCANCODE_P            0x19

/**
 * @brief Scancode for the '[' key.
 */
#define SCANCODE_LBRACKET     0x1A

/**
 * @brief Scancode for the ']' key.
 */
#define SCANCODE_RBRACKET     0x1B

/**
 * @brief Scancode for the 'Enter' key.
 */
#define SCANCODE_ENTER        0x1C

/**
 * @brief Scancode for the 'Left Control' key.
 */
#define SCANCODE_LCTRL        0x1D

/**
 * @brief Scancode for the 'A' key.
 */
#define SCANCODE_A            0x1E

/**
 * @brief Scancode for the 'S' key.
 */
#define SCANCODE_S            0x1F

/**
 * @brief Scancode for the 'D' key.
 */
#define SCANCODE_D            0x20

/**
 * @brief Scancode for the 'F' key.
 */
#define SCANCODE_F            0x21

/**
 * @brief Scancode for the 'G' key.
 */
#define SCANCODE_G            0x22

/**
 * @brief Scancode for the 'H' key.
 */
#define SCANCODE_H            0x23

/**
 * @brief Scancode for the 'J' key.
 */
#define SCANCODE_J            0x24

/**
 * @brief Scancode for the 'K' key.
 */
#define SCANCODE_K            0x25

/**
 * @brief Scancode for the 'L' key.
 */
#define SCANCODE_L            0x26

/**
 * @brief Scancode for the ';' key.
 */
#define SCANCODE_SEMICOLON    0x27

/**
 * @brief Scancode for the '\'' key.
 */
#define SCANCODE_APOSTROPHE   0x28

/**
 * @brief Scancode for the '`' key.
 */
#define SCANCODE_GRAVE        0x29

/**
 * @brief Scancode for the 'Left Shift' key.
 */
#define SCANCODE_LSHIFT       0x2A

/**
 * @brief Scancode for the '\\' key.
 */
#define SCANCODE_BACKSLASH    0x2B

/**
 * @brief Scancode for the 'Z' key.
 */
#define SCANCODE_Z            0x2C

/**
 * @brief Scancode for the 'X' key.
 */
#define SCANCODE_X            0x2D

/**
 * @brief Scancode for the 'C' key.
 */
#define SCANCODE_C            0x2E

/**
 * @brief Scancode for the 'V' key.
 */
#define SCANCODE_V            0x2F

/**
 * @brief Scancode for the 'B' key.
 */
#define SCANCODE_B            0x30

/**
 * @brief Scancode for the 'N' key.
 */
#define SCANCODE_N            0x31

/**
 * @brief Scancode for the 'M' key.
 */
#define SCANCODE_M            0x32

/**
 * @brief Scancode for the ',' key.
 */
#define SCANCODE_COMMA        0x33

/**
 * @brief Scancode for the '.' key.
 */
#define SCANCODE_PERIOD       0x34

/**
 * @brief Scancode for the '/' key.
 */
#define SCANCODE_SLASH        0x35

/**
 * @brief Scancode for the 'Right Shift' key.
 */
#define SCANCODE_RSHIFT       0x36

/**
 * @brief Scancode for the '*' key on the numeric keypad.
 */
#define SCANCODE_KP_ASTERISK  0x37

/**
 * @brief Scancode for the 'Left Alt' key.
 */
#define SCANCODE_LALT         0x38

/**
 * @brief Scancode for the 'Spacebar'.
 */
#define SCANCODE_SPACE        0x39

/**
 * @brief Scancode for the 'Caps Lock' key.
 */
#define SCANCODE_CAPSLOCK     0x3A

/**
 * @brief Scancode for the 'F1' key.
 */
#define SCANCODE_F1           0x3B

/**
 * @brief Scancode for the 'F2' key.
 */
#define SCANCODE_F2           0x3C

/**
 * @brief Scancode for the 'F3' key.
 */
#define SCANCODE_F3           0x3D

/**
 * @brief Scancode for the 'F4' key.
 */
#define SCANCODE_F4           0x3E

/**
 * @brief Scancode for the 'F5' key.
 */
#define SCANCODE_F5           0x3F

/**
 * @brief Scancode for the 'F6' key.
 */
#define SCANCODE_F6           0x40

/**
 * @brief Scancode for the 'F7' key.
 */
#define SCANCODE_F7           0x41

/**
 * @brief Scancode for the 'F8' key.
 */
#define SCANCODE_F8           0x42

/**
 * @brief Scancode for the 'F9' key.
 */
#define SCANCODE_F9           0x43

/**
 * @brief Scancode for the 'F10' key.
 */
#define SCANCODE_F10          0x44

/**
 * @brief Scancode for the 'Num Lock' key.
 */
#define SCANCODE_NUMLOCK      0x45

/**
 * @brief Scancode for the 'Scroll Lock' key.
 */
#define SCANCODE_SCROLLLOCK   0x46

/**
 * @brief Scancode for the '7' key on the numeric keypad.
 */
#define SCANCODE_KP_7         0x47

/**
 * @brief Scancode for the '8' key on the numeric keypad.
 */
#define SCANCODE_KP_8         0x48

/**
 * @brief Scancode for the '9' key on the numeric keypad.
 */
#define SCANCODE_KP_9         0x49

/**
 * @brief Scancode for the '-' key on the numeric keypad.
 */
#define SCANCODE_KP_MINUS     0x4A

/**
 * @brief Scancode for the '4' key on the numeric keypad.
 */
#define SCANCODE_KP_4         0x4B

/**
 * @brief Scancode for the '5' key on the numeric keypad.
 */
#define SCANCODE_KP_5         0x4C

/**
 * @brief Scancode for the '6' key on the numeric keypad.
 */
#define SCANCODE_KP_6         0x4D

/**
 * @brief Scancode for the '+' key on the numeric keypad.
 */
#define SCANCODE_KP_PLUS      0x4E

/**
 * @brief Scancode for the '1' key on the numeric keypad.
 */
#define SCANCODE_KP_1         0x4F

/**
 * @brief Scancode for the '2' key on the numeric keypad.
 */
#define SCANCODE_KP_2         0x50

/**
 * @brief Scancode for the '3' key on the numeric keypad.
 */
#define SCANCODE_KP_3         0x51

/**
 * @brief Scancode for the '0' key on the numeric keypad.
 */
#define SCANCODE_KP_0         0x52

/**
 * @brief Scancode for the '.' key on the numeric keypad.
 */
#define SCANCODE_KP_PERIOD    0x53

/**
 * @brief Scancode for the 'F11' key.
 */
#define SCANCODE_F11          0x57

/**
 * @brief Scancode for the 'F12' key.
 */
#define SCANCODE_F12          0x58

/**
 * @brief Scancode for the 'Right Control' key (extended scancode).
 */
#define SCANCODE_RCTRL        0xE01D

/**
 * @brief Scancode for the 'Right Alt' (AltGr) key (extended scancode).
 */
#define SCANCODE_RALT         0xE038

/**
 * @brief Scancode for the 'Home' key (extended scancode).
 */
#define SCANCODE_HOME         0xE047

/**
 * @brief Scancode for the 'Up Arrow' key (extended scancode).
 */
#define SCANCODE_UP           0xE048

/**
 * @brief Scancode for the 'Page Up' key (extended scancode).
 */
#define SCANCODE_PAGEUP       0xE049

/**
 * @brief Scancode for the 'Left Arrow' key (extended scancode).
 */
#define SCANCODE_LEFT         0xE04B

/**
 * @brief Scancode for the 'Right Arrow' key (extended scancode).
 */
#define SCANCODE_RIGHT        0xE04D

/**
 * @brief Scancode for the 'End' key (extended scancode).
 */
#define SCANCODE_END          0xE04F

/**
 * @brief Scancode for the 'Down Arrow' key (extended scancode).
 */
#define SCANCODE_DOWN         0xE050

/**
 * @brief Scancode for the 'Page Down' key (extended scancode).
 */
#define SCANCODE_PAGEDOWN     0xE051

/**
 * @brief Scancode for the 'Insert' key (extended scancode).
 */
#define SCANCODE_INSERT       0xE052

/**
 * @brief Scancode for the 'Delete' key (extended scancode).
 */
#define SCANCODE_DELETE       0xE053

/**
 * @brief Macro to calculate a key's break code (key release).
 */
#define BREAK_CODE(scanCode)  ((scanCode) + 0x80)

/** @} */ // End of Scancodes group




/**
 * @defgroup Keycodes Abstract Keycodes
 * @brief Defines abstract keycodes for keyboard events.
 *
 * This group provides definitions for abstract keycodes, which are higher-level representations
 * of physical key presses. These keycodes are used to map scancodes to meaningful identifiers
 * and provide cross-platform compatibility.
 * @{
 */

/**
 * @brief No key pressed (reserved for special use).
 */
#define KEY_NONE          0x00

/**
 * @brief Error state or undefined key.
 */
#define KEY_ERROR         0x01

/**
 * @brief Reserved for future use.
 */
#define KEY_RESERVED      0x02

/**
 * @brief Another reserved keycode.
 */
#define KEY_SYSTEM        0x03

/**
 * @brief Keycode for the 'A' key.
 */
#define KEY_A             0x04

/**
 * @brief Keycode for the 'B' key.
 */
#define KEY_B             0x05

/**
 * @brief Keycode for the 'C' key.
 */
#define KEY_C             0x06

/**
 * @brief Keycode for the 'D' key.
 */
#define KEY_D             0x07

/**
 * @brief Keycode for the 'E' key.
 */
#define KEY_E             0x08

/**
 * @brief Keycode for the 'F' key.
 */
#define KEY_F             0x09

/**
 * @brief Keycode for the 'G' key.
 */
#define KEY_G             0x0A

/**
 * @brief Keycode for the 'H' key.
 */
#define KEY_H             0x0B

/**
 * @brief Keycode for the 'I' key.
 */
#define KEY_I             0x0C

/**
 * @brief Keycode for the 'J' key.
 */
#define KEY_J             0x0D

/**
 * @brief Keycode for the 'K' key.
 */
#define KEY_K             0x0E

/**
 * @brief Keycode for the 'L' key.
 */
#define KEY_L             0x0F

/**
 * @brief Keycode for the 'M' key.
 */
#define KEY_M             0x10

/**
 * @brief Keycode for the 'N' key.
 */
#define KEY_N             0x11

/**
 * @brief Keycode for the 'O' key.
 */
#define KEY_O             0x12

/**
 * @brief Keycode for the 'P' key.
 */
#define KEY_P             0x13

/**
 * @brief Keycode for the 'Q' key.
 */
#define KEY_Q             0x14

/**
 * @brief Keycode for the 'R' key.
 */
#define KEY_R             0x15

/**
 * @brief Keycode for the 'S' key.
 */
#define KEY_S             0x16

/**
 * @brief Keycode for the 'T' key.
 */
#define KEY_T             0x17

/**
 * @brief Keycode for the 'U' key.
 */
#define KEY_U             0x18

/**
 * @brief Keycode for the 'V' key.
 */
#define KEY_V             0x19

/**
 * @brief Keycode for the 'W' key.
 */
#define KEY_W             0x1A

/**
 * @brief Keycode for the 'X' key.
 */
#define KEY_X             0x1B

/**
 * @brief Keycode for the 'Y' key.
 */
#define KEY_Y             0x1C

/**
 * @brief Keycode for the 'Z' key.
 */
#define KEY_Z             0x1D

/**
 * @brief Keycode for the '1' key.
 */
#define KEY_1             0x1E

/**
 * @brief Keycode for the '2' key.
 */
#define KEY_2             0x1F

/**
 * @brief Keycode for the '3' key.
 */
#define KEY_3             0x20

/**
 * @brief Keycode for the '4' key.
 */
#define KEY_4             0x21

/**
 * @brief Keycode for the '5' key.
 */
#define KEY_5             0x22

/**
 * @brief Keycode for the '6' key.
 */
#define KEY_6             0x23

/**
 * @brief Keycode for the '7' key.
 */
#define KEY_7             0x24

/**
 * @brief Keycode for the '8' key.
 */
#define KEY_8             0x25

/**
 * @brief Keycode for the '9' key.
 */
#define KEY_9             0x26

/**
 * @brief Keycode for the '0' key.
 */
#define KEY_0             0x27

/**
 * @brief Keycode for the 'Enter' key.
 */
#define KEY_ENTER         0x28

/**
 * @brief Keycode for the 'Escape' key.
 */
#define KEY_ESC           0x29

/**
 * @brief Keycode for the 'Backspace' key.
 */
#define KEY_BACKSPACE     0x2A

/**
 * @brief Keycode for the 'Tab' key.
 */
#define KEY_TAB           0x2B

/**
 * @brief Keycode for the 'Space' key.
 */
#define KEY_SPACE         0x2C

/**
 * @brief Keycode for the '-' key.
 */
#define KEY_MINUS         0x2D

/**
 * @brief Keycode for the '=' key.
 */
#define KEY_EQUALS        0x2E

/**
 * @brief Keycode for the '[' key.
 */
#define KEY_LBRACKET      0x2F

/**
 * @brief Keycode for the ']' key.
 */
#define KEY_RBRACKET      0x30

/**
 * @brief Keycode for the '\\' key.
 */
#define KEY_BACKSLASH     0x31

/**
 * @brief Keycode for the ';' key.
 */
#define KEY_SEMICOLON     0x32

/**
 * @brief Keycode for the '\'' key.
 */
#define KEY_APOSTROPHE    0x33

/**
 * @brief Keycode for the '`' key.
 */
#define KEY_GRAVE         0x34

/**
 * @brief Keycode for the ',' key.
 */
#define KEY_COMMA         0x35

/**
 * @brief Keycode for the '.' key.
 */
#define KEY_PERIOD        0x36

/**
 * @brief Keycode for the '/' key.
 */
#define KEY_SLASH         0x37

/**
 * @brief Keycode for the 'Caps Lock' key.
 */
#define KEY_CAPSLOCK      0x38

/**
 * @brief Keycode for the 'F1' key.
 */
#define KEY_F1            0x39

/**
 * @brief Keycode for the 'F2' key.
 */
#define KEY_F2            0x3A

/**
 * @brief Keycode for the 'F3' key.
 */
#define KEY_F3            0x3B

/**
 * @brief Keycode for the 'F4' key.
 */
#define KEY_F4            0x3C

/**
 * @brief Keycode for the 'F5' key.
 */
#define KEY_F5            0x3D

/**
 * @brief Keycode for the 'F6' key.
 */
#define KEY_F6            0x3E

/**
 * @brief Keycode for the 'F7' key.
 */
#define KEY_F7            0x3F

/**
 * @brief Keycode for the 'F8' key.
 */
#define KEY_F8            0x40

/**
 * @brief Keycode for the 'F9' key.
 */
#define KEY_F9            0x41

/**
 * @brief Keycode for the 'F10' key.
 */
#define KEY_F10           0x42

/**
 * @brief Keycode for the 'F11' key.
 */
#define KEY_F11           0x43

/**
 * @brief Keycode for the 'F12' key.
 */
#define KEY_F12           0x44

/**
 * @brief Keycode for the 'Print Screen' key.
 */
#define KEY_PRINTSCREEN   0x45

/**
 * @brief Keycode for the 'Scroll Lock' key.
 */
#define KEY_SCROLLLOCK    0x46

/**
 * @brief Keycode for the 'Pause' key.
 */
#define KEY_PAUSE         0x47

/**
 * @brief Keycode for the 'Insert' key.
 */
#define KEY_INSERT        0x48

/**
 * @brief Keycode for the 'Home' key.
 */
#define KEY_HOME          0x49

/** 
 * @brief Keycode for the 'Page Up' key. 
 */
#define KEY_PAGEUP        0x4A

/** 
 * @brief Keycode for the 'Delete' key. 
 */
#define KEY_DELETE        0x4B

/** 
 * @brief Keycode for the 'End' key. 
 */
#define KEY_END           0x4C

/** 
 * @brief Keycode for the 'Page Down' key. 
 */
#define KEY_PAGEDOWN      0x4D

/** 
 * @brief Keycode for the 'Right Arrow' key. 
 */
#define KEY_RIGHT         0x4E

/** 
 * @brief Keycode for the 'Left Arrow' key. 
 */
#define KEY_LEFT          0x4F

/** 
 * @brief Keycode for the 'Down Arrow' key. 
 */
#define KEY_DOWN          0x50

/** 
 * @brief Keycode for the 'Up Arrow' key. 
 */
#define KEY_UP            0x51

/** 
 * @brief Keycode for the 'Num Lock' key. 
 */
#define KEY_NUMLOCK       0x52

/** 
 * @brief Keycode for the 'Keypad /' key. 
 */
#define KEY_KP_SLASH      0x53

/** 
 * @brief Keycode for the 'Keypad *' key. 
 */
#define KEY_KP_ASTERISK   0x54

/** 
 * @brief Keycode for the 'Keypad -' key. 
 */
#define KEY_KP_MINUS      0x55

/** 
 * @brief Keycode for the 'Keypad +' key. 
 */
#define KEY_KP_PLUS       0x56

/** 
 * @brief Keycode for the 'Keypad Enter' key. 
 */
#define KEY_KP_ENTER      0x57

/** 
 * @brief Keycode for the 'Keypad 1' key. 
 */
#define KEY_KP_1          0x58

/** 
 * @brief Keycode for the 'Keypad 2' key. 
 */
#define KEY_KP_2          0x59

/** 
 * @brief Keycode for the 'Keypad 3' key. 
 */
#define KEY_KP_3          0x5A

/** 
 * @brief Keycode for the 'Keypad 4' key. 
 */
#define KEY_KP_4          0x5B

/** 
 * @brief Keycode for the 'Keypad 5' key. 
 */
#define KEY_KP_5          0x5C

/** 
 * @brief Keycode for the 'Keypad 6' key. 
 */
#define KEY_KP_6          0x5D

/** 
 * @brief Keycode for the 'Keypad 7' key. 
 */
#define KEY_KP_7          0x5E

/** 
 * @brief Keycode for the 'Keypad 8' key. 
 */
#define KEY_KP_8          0x5F

/** 
 * @brief Keycode for the 'Keypad 9' key. 
 */
#define KEY_KP_9          0x60

/** 
 * @brief Keycode for the 'Keypad 0' key. 
 */
#define KEY_KP_0          0x61

/** 
 * @brief Keycode for the 'Keypad .' key. 
 */
#define KEY_KP_PERIOD     0x62

/** 
 * @brief Keycode for the 'Left Control' key. 
 */
#define KEY_LCTRL         0x63

/** 
 * @brief Keycode for the 'Left Shift' key. 
 */
#define KEY_LSHIFT        0x64

/** 
 * @brief Keycode for the 'Left Alt' key. 
 */
#define KEY_LALT          0x65

/** 
 * @brief Keycode for the 'Left GUI/Windows' key. 
 */
#define KEY_LGUI          0x66

/** 
 * @brief Keycode for the 'Right Control' key. 
 */
#define KEY_RCTRL         0x67

/** 
 * @brief Keycode for the 'Right Shift' key. 
 */
#define KEY_RSHIFT        0x68

/** 
 * @brief Keycode for the 'Right Alt' (AltGr) key. 
 */
#define KEY_RALT          0x69

/** 
 * @brief Keycode for the 'Right GUI/Windows' key. 
 */
#define KEY_RGUI          0x6A

/** @} */ // End of Keycodes group




/**
 * @brief A numerical identifier representing a specific key on the keyboard.
 *
 * `KeyCode` is used to identify a key on the keyboard using its scan code.
 */
typedef uint8_t KeyCode;


/**
 * @brief Bit flags representing active key modifiers (Shift, Ctrl, Alt).
 *
 * This enum defines bitmask values for key modifiers, allowing combinations
 * of active modifier keys to be represented using bitwise operations.
 *
 * - `None` (0): No modifiers are active.
 * - `Shift` (1 << 0): The Shift key is active.
 * - `Ctrl` (1 << 1): The Ctrl key is active.
 * - `Alt` (1 << 2): The Alt key is active.
 */
typedef enum {
    None  = 0,       ///< No modifiers active.
    Shift = 1 << 0,  ///< Shift key active.
    Ctrl  = 1 << 1,  ///< Ctrl key active.
    Alt   = 1 << 2   ///< Alt key active.
} KeyModifiers;


/**
 * @brief Event argument structure for keyboard events.
 *
 * This structure encapsulates the details of a keyboard event, such as the
 * pressed key, whether it was pressed or released, and any active modifiers.
 *
 * - `Modifiers`: A pointer to the active key modifiers (e.g., Shift, Ctrl).
 * - `KeyCode`: The scan code of the key involved in the event.
 * - `KeyDown`: Indicates if the key was pressed (`true`) or released (`false`).
 */
typedef struct {
    KeyModifiers *Modifiers; ///< Pointer to active key modifiers.
    KeyCode KeyCode;         ///< The scan code of the key.
    bool KeyDown;            ///< `true` if the key is pressed; `false` if released.
} KeyEventArgs;


/**
 * @brief Represents the character mapping for a single key.
 *
 * A `KeyMapEntry` provides the characters produced by a keypress, depending on
 * the active modifier keys (Shift, Ctrl, Alt, or none).
 *
 * - `Normal`: The character produced with no modifiers.
 * - `Shift`: The character produced with the Shift key active.
 * - `Ctrl`: The character produced with the Ctrl key active.
 * - `Alt`: The character produced with the Alt key active.
 */
typedef struct {
    char Normal; ///< Character produced with no modifiers.
    char Shift;  ///< Character produced with Shift modifier.
    char Ctrl;   ///< Character produced with Ctrl modifier.
    char Alt;    ///< Character produced with Alt modifier.
} KeyMapEntry;


/**
 * @brief A keyboard mapping table for all keys.
 *
 * `KeyMap` is an array of `KeyMapEntry` structures, where each entry defines
 * the character mappings for a specific key (identified by its scan code).
 * The array contains 256 entries, covering all possible scan codes.
 */
typedef KeyMapEntry KeyMap[256];


/**
 * @brief An event handler for keyboard events.
 *
 * A function pointer type that handles `KeyEventArgs` structures. It is used
 * to process keyboard events such as key presses and releases.
 */
typedef void (*KeyEventHandler)(KeyEventArgs);




/**
 * @brief Checks if a given scancode represents a break code (key release).
 *
 * Break codes indicate that a key has been released. In the standard
 * keyboard scancode set, a break code is identified when the highest
 * bit (bit 7) of the low byte in the scancode is set (1).
 *
 * @param scanCode The 16-bit scancode to check.
 * @return `true` if the scancode is a break code, `false` otherwise.
 */
extern bool Kbd_CheckIfBreakCode(uint16_t scanCode);


/**
 * @brief Registers a callback function for key down events.
 *
 * This function allows the user to specify a custom callback that will
 * be invoked whenever a key down event occurs.
 *
 * @param callback A function pointer of type `KeyEventHandler` to handle key down events.
 */
extern void Kbd_SetKeyDownCallback(KeyEventHandler callback);


/**
 * @brief Retrieves the currently registered key down callback.
 *
 * This function returns the previously registered callback function for
 * key down events. If no callback is set, it returns `NULL`.
 *
 * @return The currently registered `KeyEventHandler` for key down events.
 */
extern KeyEventHandler Kbd_GetKeyDownCallback(void);


/**
 * @brief Registers a callback function for key up events.
 *
 * This function allows the user to specify a custom callback that will
 * be invoked whenever a key up event occurs.
 *
 * @param callback A function pointer of type `KeyEventHandler` to handle key up events.
 */
extern void Kbd_SetKeyUpCallback(KeyEventHandler callback);


/**
 * @brief Retrieves the currently registered key up callback.
 *
 * This function returns the previously registered callback function for
 * key up events. If no callback is set, it returns `NULL`.
 *
 * @return The currently registered `KeyEventHandler` for key up events.
 */
extern KeyEventHandler Kbd_GetKeyUpCallback(void);


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
extern void Kbd_UpdateKeyModifiers(KeyCode keyCode, bool keyDown, KeyModifiers *modifiers);


/**
 * @brief Reads the current scancode from the keyboard's I/O port.
 *
 * This function reads raw scancodes from the keyboard. It handles extended scancodes
 * (prefixed with `0xE0`) by combining them into a 16-bit representation:
 * - Extended scancodes are returned as `0xE000 | scancode`.
 * - Single-byte scancodes are returned as-is.
 * - If the prefix `0xE0` is read, the function returns `0` and waits for the next scancode.
 *
 * @return A 16-bit scancode. Returns `0` if the function is processing an extended prefix.
 */
extern uint16_t Kbd_ReadScanCode(void);


/**
 * @brief Translates a scancode to its corresponding keycode.
 *
 * This function converts scancodes (raw keyboard input) into abstract keycodes
 * using a predefined mapping table and additional logic for extended scancodes
 * prefixed with `0xE0`. Extended scancodes are identified and mapped individually.
 *
 * @param scanCode The scancode to translate (8-bit or extended 16-bit value).
 * @return The corresponding `KeyCode` or `KEY_NONE` if the scancode is invalid or unrecognized.
 */
extern KeyCode Kbd_GetKeyCode(uint16_t scanCode);


/**
 * @brief Handles a key press or release event from the keyboard.
 *
 * This function reads the next scancode from the keyboard, determines whether the
 * associated key was pressed or released, and updates the modifier state accordingly.
 * It then constructs a `KeyEventArgs` structure and invokes the appropriate callback
 * (key down or key up) if one is registered.
 */
extern void Kbd_HandleKeyPress(void);


/**
 * @brief Gets the character for a keycode and modifier combination.
 *
 * This function retrieves the character from the German key map based on the given
 * `KeyCode` and the active modifiers. It prioritizes modifiers in the following order:
 * - Shift
 * - Alt
 * - Ctrl
 *
 * If no modifiers are active, the `Normal` entry from the map is returned.
 *
 * @param keyCode The keycode of the key to look up.
 * @param modifiers The current active key modifiers (Shift, Ctrl, Alt).
 * @return The character corresponding to the given keycode and modifiers, or `0` if no match is found.
 */
extern char Kbd_GetGermanKeyMapChar(KeyCode keyCode, KeyModifiers modifiers);

#endif
