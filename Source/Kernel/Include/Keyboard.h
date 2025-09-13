/*
	
  Copyright © 2025 Maximilian Jung

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the “Software”), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
*/


#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../../Modules/Include/SystemCore.h"


// An abstract number describing the key
typedef U8 KeyCode;


// Describes the active key modifiers
typedef enum {
  None = 0,
  Shift = 1 <<0,
  Ctrl = 1 << 1,
  Alt = 1 << 2
} KeyModifiers;


typedef struct KeyEventArgs {
  // The active key modifiers
  KeyModifiers* Modifiers;

  // The key code
  KeyCode KeyCode;

  // Indicates whether the key was pressed or released
  bool WasKeyPress;
} KeyEventArgs;


typedef struct KeyMapEntry {
  char Normal;
  char Shift;
  char Ctrl;
  char Alt;
} KeyMapEntry;


typedef void (*KeyEventHandler)(KeyEventArgs args);



// Scan codes
#define SCANCODE_ESC          0x01
#define SCANCODE_1            0x02
#define SCANCODE_2            0x03
#define SCANCODE_3            0x04
#define SCANCODE_4            0x05
#define SCANCODE_5            0x06
#define SCANCODE_6            0x07
#define SCANCODE_7            0x08
#define SCANCODE_8            0x09
#define SCANCODE_9            0x0A
#define SCANCODE_0            0x0B
#define SCANCODE_MINUS        0x0C
#define SCANCODE_EQUALS       0x0D
#define SCANCODE_BACKSPACE    0x0E
#define SCANCODE_TAB          0x0F
#define SCANCODE_Q            0x10
#define SCANCODE_W            0x11
#define SCANCODE_E            0x12
#define SCANCODE_R            0x13
#define SCANCODE_T            0x14
#define SCANCODE_Y            0x15
#define SCANCODE_U            0x16
#define SCANCODE_I            0x17
#define SCANCODE_O            0x18
#define SCANCODE_P            0x19
#define SCANCODE_LBRACKET     0x1A
#define SCANCODE_RBRACKET     0x1B
#define SCANCODE_ENTER        0x1C
#define SCANCODE_LCTRL        0x1D
#define SCANCODE_A            0x1E
#define SCANCODE_S            0x1F
#define SCANCODE_D            0x20
#define SCANCODE_F            0x21
#define SCANCODE_G            0x22
#define SCANCODE_H            0x23
#define SCANCODE_J            0x24
#define SCANCODE_K            0x25
#define SCANCODE_L            0x26
#define SCANCODE_SEMICOLON    0x27
#define SCANCODE_APOSTROPHE   0x28
#define SCANCODE_GRAVE        0x29
#define SCANCODE_LSHIFT       0x2A
#define SCANCODE_BACKSLASH    0x2B
#define SCANCODE_Z            0x2C
#define SCANCODE_X            0x2D
#define SCANCODE_C            0x2E
#define SCANCODE_V            0x2F
#define SCANCODE_B            0x30
#define SCANCODE_N            0x31
#define SCANCODE_M            0x32
#define SCANCODE_COMMA        0x33
#define SCANCODE_PERIOD       0x34
#define SCANCODE_SLASH        0x35
#define SCANCODE_RSHIFT       0x36
#define SCANCODE_KP_ASTERISK  0x37
#define SCANCODE_LALT         0x38
#define SCANCODE_SPACE        0x39
#define SCANCODE_CAPSLOCK     0x3A
#define SCANCODE_F1           0x3B
#define SCANCODE_F2           0x3C
#define SCANCODE_F3           0x3D
#define SCANCODE_F4           0x3E
#define SCANCODE_F5           0x3F
#define SCANCODE_F6           0x40
#define SCANCODE_F7           0x41
#define SCANCODE_F8           0x42
#define SCANCODE_F9           0x43
#define SCANCODE_F10          0x44
#define SCANCODE_NUMLOCK      0x45
#define SCANCODE_SCROLLLOCK   0x46
#define SCANCODE_KP_7         0x47
#define SCANCODE_KP_8         0x48
#define SCANCODE_KP_9         0x49
#define SCANCODE_KP_MINUS     0x4A
#define SCANCODE_KP_4         0x4B
#define SCANCODE_KP_5         0x4C
#define SCANCODE_KP_6         0x4D
#define SCANCODE_KP_PLUS      0x4E
#define SCANCODE_KP_1         0x4F
#define SCANCODE_KP_2         0x50
#define SCANCODE_KP_3         0x51
#define SCANCODE_KP_0         0x52
#define SCANCODE_KP_PERIOD    0x53
#define SCANCODE_F11          0x57
#define SCANCODE_F12          0x58


// Extended scan codes
#define SCANCODE_RCTRL        0xE01D
#define SCANCODE_RALT         0xE038
#define SCANCODE_HOME         0xE047
#define SCANCODE_UP           0xE048
#define SCANCODE_PAGEUP       0xE049
#define SCANCODE_LEFT         0xE04B
#define SCANCODE_RIGHT        0xE04D
#define SCANCODE_END          0xE04F
#define SCANCODE_DOWN         0xE050
#define SCANCODE_PAGEDOWN     0xE051
#define SCANCODE_INSERT       0xE052
#define SCANCODE_DELETE       0xE053



// Get the break code for a specific scan code
#define BREAK_CODE(scanCode)  ((scanCode) + 0x80)



// Abstract key codes
#define KEY_NONE          0x00
#define KEY_ERROR         0x01
#define KEY_RESERVED      0x02
#define KEY_SYSTEM        0x03
#define KEY_A             0x04
#define KEY_B             0x05
#define KEY_C             0x06
#define KEY_D             0x07
#define KEY_E             0x08
#define KEY_F             0x09
#define KEY_G             0x0A
#define KEY_H             0x0B
#define KEY_I             0x0C
#define KEY_J             0x0D
#define KEY_K             0x0E
#define KEY_L             0x0F
#define KEY_M             0x10
#define KEY_N             0x11
#define KEY_O             0x12
#define KEY_P             0x13
#define KEY_Q             0x14
#define KEY_R             0x15
#define KEY_S             0x16
#define KEY_T             0x17
#define KEY_U             0x18
#define KEY_V             0x19
#define KEY_W             0x1A
#define KEY_X             0x1B
#define KEY_Y             0x1C
#define KEY_Z             0x1D
#define KEY_1             0x1E
#define KEY_2             0x1F
#define KEY_3             0x20
#define KEY_4             0x21
#define KEY_5             0x22
#define KEY_6             0x23
#define KEY_7             0x24
#define KEY_8             0x25
#define KEY_9             0x26
#define KEY_0             0x27
#define KEY_ENTER         0x28
#define KEY_ESC           0x29
#define KEY_BACKSPACE     0x2A
#define KEY_TAB           0x2B
#define KEY_SPACE         0x2C
#define KEY_MINUS         0x2D
#define KEY_EQUALS        0x2E
#define KEY_LBRACKET      0x2F
#define KEY_RBRACKET      0x30
#define KEY_BACKSLASH     0x31
#define KEY_SEMICOLON     0x32
#define KEY_APOSTROPHE    0x33
#define KEY_GRAVE         0x34
#define KEY_COMMA         0x35
#define KEY_PERIOD        0x36
#define KEY_SLASH         0x37
#define KEY_CAPSLOCK      0x38
#define KEY_F1            0x39
#define KEY_F2            0x3A
#define KEY_F3            0x3B
#define KEY_F4            0x3C
#define KEY_F5            0x3D
#define KEY_F6            0x3E
#define KEY_F7            0x3F
#define KEY_F8            0x40
#define KEY_F9            0x41
#define KEY_F10           0x42
#define KEY_F11           0x43
#define KEY_F12           0x44
#define KEY_PRINTSCREEN   0x45
#define KEY_SCROLLLOCK    0x46
#define KEY_PAUSE         0x47
#define KEY_INSERT        0x48
#define KEY_HOME          0x49
#define KEY_PAGEUP        0x4A
#define KEY_DELETE        0x4B
#define KEY_END           0x4C
#define KEY_PAGEDOWN      0x4D
#define KEY_RIGHT         0x4E
#define KEY_LEFT          0x4F
#define KEY_DOWN          0x50
#define KEY_UP            0x51
#define KEY_NUMLOCK       0x52
#define KEY_KP_SLASH      0x53
#define KEY_KP_ASTERISK   0x54
#define KEY_KP_MINUS      0x55
#define KEY_KP_PLUS       0x56
#define KEY_KP_ENTER      0x57
#define KEY_KP_1          0x58
#define KEY_KP_2          0x59
#define KEY_KP_3          0x5A
#define KEY_KP_4          0x5B
#define KEY_KP_5          0x5C
#define KEY_KP_6          0x5D
#define KEY_KP_7          0x5E
#define KEY_KP_8          0x5F
#define KEY_KP_9          0x60
#define KEY_KP_0          0x61
#define KEY_KP_PERIOD     0x62
#define KEY_LCTRL         0x63
#define KEY_LSHIFT        0x64
#define KEY_LALT          0x65
#define KEY_LGUI          0x66
#define KEY_RCTRL         0x67
#define KEY_RSHIFT        0x68
#define KEY_RALT          0x69
#define KEY_RGUI          0x6A


module(Keyboard) {
  U16 (*ReadScanCode)(void);
  KeyCode (*GetKeyCode)(U16 scanCode);
  void (*UpdateModifiers)(KeyCode keyCode, bool keyDown, KeyModifiers *modifiers);
  char (*GetChar)(KeyCode keyCode, KeyModifiers modifiers);
};



#endif
