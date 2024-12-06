#include "../Include/Kbd.h"

static const KeyCode _Kbd_KeyCodeMap[256] = {
  [SCANCODE_ESC]			  = KEY_ESC,
  [SCANCODE_1]				  = KEY_1,
  [SCANCODE_2]				  = KEY_2,
  [SCANCODE_3]				  = KEY_3,
  [SCANCODE_4]				  = KEY_4,
  [SCANCODE_5]				  = KEY_5,
  [SCANCODE_6]				  = KEY_6,
  [SCANCODE_7]				  = KEY_7,
  [SCANCODE_8]				  = KEY_8,
  [SCANCODE_9]				  = KEY_9,
  [SCANCODE_0]				  = KEY_0,
  [SCANCODE_MINUS]			  = KEY_MINUS,
  [SCANCODE_EQUALS]			  = KEY_EQUALS,
  [SCANCODE_BACKSPACE]			  = KEY_BACKSPACE,
  [SCANCODE_TAB]			  = KEY_TAB,
  [SCANCODE_Q]				  = KEY_Q,
  [SCANCODE_W]				  = KEY_W,
  [SCANCODE_E]				  = KEY_E,
  [SCANCODE_R]				  = KEY_R,
  [SCANCODE_T]				  = KEY_T,
  [SCANCODE_Y]				  = KEY_Y,
  [SCANCODE_U]				  = KEY_U,
  [SCANCODE_I]				  = KEY_I,
  [SCANCODE_O]				  = KEY_O,
  [SCANCODE_P]				  = KEY_P,
  [SCANCODE_LBRACKET]			  = KEY_LBRACKET,
  [SCANCODE_RBRACKET]			  = KEY_RBRACKET,
  [SCANCODE_ENTER]			  = KEY_ENTER,
  [SCANCODE_LCTRL]			  = KEY_LCTRL,
  [SCANCODE_A]				  = KEY_A,
  [SCANCODE_S]				  = KEY_S,
  [SCANCODE_D]				  = KEY_D,
  [SCANCODE_F]				  = KEY_F,
  [SCANCODE_G]				  = KEY_G,
  [SCANCODE_H]				  = KEY_H,
  [SCANCODE_J]				  = KEY_J,
  [SCANCODE_K]				  = KEY_K,
  [SCANCODE_L]				  = KEY_L,
  [SCANCODE_SEMICOLON]			  = KEY_SEMICOLON,
  [SCANCODE_APOSTROPHE]			  = KEY_APOSTROPHE,
  [SCANCODE_GRAVE]			  = KEY_GRAVE,
  [SCANCODE_LSHIFT]			  = KEY_LSHIFT,
  [SCANCODE_BACKSLASH]			  = KEY_BACKSLASH,
  [SCANCODE_Z]				  = KEY_Z,
  [SCANCODE_X]				  = KEY_X,
  [SCANCODE_C]				  = KEY_C,
  [SCANCODE_V]				  = KEY_V,
  [SCANCODE_B]				  = KEY_B,
  [SCANCODE_N]				  = KEY_N,
  [SCANCODE_M]				  = KEY_M,
  [SCANCODE_COMMA]			  = KEY_COMMA,
  [SCANCODE_PERIOD]			  = KEY_PERIOD,
  [SCANCODE_SLASH]			  = KEY_SLASH,
  [SCANCODE_RSHIFT]			  = KEY_RSHIFT,
  [SCANCODE_KP_ASTERISK]		  = KEY_KP_ASTERISK,
  [SCANCODE_LALT]			  = KEY_LALT,
  [SCANCODE_SPACE]			  = KEY_SPACE,
  [SCANCODE_CAPSLOCK]			  = KEY_CAPSLOCK,
  [SCANCODE_F1]				  = KEY_F1,
  [SCANCODE_F2]				  = KEY_F2,
  [SCANCODE_F3]				  = KEY_F3,
  [SCANCODE_F4]				  = KEY_F4,
  [SCANCODE_F5]				  = KEY_F5,
  [SCANCODE_F6]				  = KEY_F6,
  [SCANCODE_F7]				  = KEY_F7,
  [SCANCODE_F8]				  = KEY_F8,
  [SCANCODE_F9]				  = KEY_F9,
  [SCANCODE_F10]			  = KEY_F10,
  [SCANCODE_NUMLOCK]			  = KEY_NUMLOCK,
  [SCANCODE_SCROLLLOCK]			  = KEY_SCROLLLOCK,
  [SCANCODE_KP_7]			  = KEY_KP_7,
  [SCANCODE_KP_8]			  = KEY_KP_8,
  [SCANCODE_KP_9]			  = KEY_KP_9,
  [SCANCODE_KP_MINUS]			  = KEY_KP_MINUS,
  [SCANCODE_KP_4]			  = KEY_KP_4,
  [SCANCODE_KP_5]			  = KEY_KP_5,
  [SCANCODE_KP_6]			  = KEY_KP_6,
  [SCANCODE_KP_PLUS]			  = KEY_KP_PLUS,
  [SCANCODE_KP_1]			  = KEY_KP_1,
  [SCANCODE_KP_2]			  = KEY_KP_2,
  [SCANCODE_KP_3]			  = KEY_KP_3,
  [SCANCODE_KP_0]			  = KEY_KP_0,
  [SCANCODE_KP_PERIOD]			  = KEY_KP_PERIOD,
  [SCANCODE_F11]			  = KEY_F11,
  [SCANCODE_F12]			  = KEY_F12
};


KeyCode Kbd_GetKeyCode(uint16_t scanCode) {
  if (0xe000 & scanCode) {
    // Is extended scancode
    
    switch (scanCode) {
      // --- Arrow keys ---
      
    case SCANCODE_UP:
    case BREAK_CODE(SCANCODE_UP):
      // Arrow up
      return KEY_UP;

    case SCANCODE_LEFT:
    case BREAK_CODE(SCANCODE_LEFT):
      // Arrow left
      return KEY_LEFT;
      
    case SCANCODE_RIGHT:
    case BREAK_CODE(SCANCODE_RIGHT):
      // Arrow right
      return KEY_RIGHT;

    case SCANCODE_DOWN:
    case BREAK_CODE(SCANCODE_DOWN):
      // Arrow down
      return KEY_DOWN;


      // --- Modifier keys ---
      
    case SCANCODE_RCTRL:
    case BREAK_CODE(SCANCODE_RCTRL):
      // Right control key
      return KEY_RCTRL;
      
    case SCANCODE_RALT:
    case BREAK_CODE(SCANCODE_RALT):
      // Right alt key
      return KEY_RALT;


      // --- Command keys ---
      
    case SCANCODE_HOME:
    case BREAK_CODE(SCANCODE_HOME):
      return KEY_HOME;
      
    case SCANCODE_PAGEUP:
    case BREAK_CODE(SCANCODE_PAGEUP):
      return KEY_PAGEUP;
      
    case SCANCODE_END:
    case BREAK_CODE(SCANCODE_END):
      return KEY_END;

    case SCANCODE_PAGEDOWN: 
    case BREAK_CODE(SCANCODE_PAGEDOWN): 
      return KEY_PAGEDOWN;
      
    case SCANCODE_INSERT:
    case BREAK_CODE(SCANCODE_INSERT):
      return KEY_INSERT;
      
    case SCANCODE_DELETE:
    case BREAK_CODE(SCANCODE_DELETE):
      return KEY_DELETE;
    }
  }

  return _Kbd_KeyCodeMap[scanCode];
}
