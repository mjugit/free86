#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../../StandardLib/Include/Stream.h"

#define BREAK_CODE(scancode) ((scancode) + 0x80)

// Scancodes for german keyboards

typedef enum {
  KeyEscape = 0x01,
  Key1 = 0x02,
  Key2 = 0x03,
  Key3 = 0x04,
  Key4 = 0x05,
  Key5 = 0x06,
  Key6 = 0x07,
  Key7 = 0x08,
  Key8 = 0x09,
  Key9 = 0x0a,
  Key0 = 0x0b,
  KeySz = 0x0c,         // originally '-'
  KeyBacktic = 0x0d,    // originally '='
  KeyBackspace = 0x0e,
  KeyTab = 0x0f,
  KeyQ = 0x10,
  KeyW = 0x11,
  KeyE = 0x12,
  KeyR = 0x13,
  KeyT = 0x14,
  KeyZ = 0x15,
  KeyU = 0x16,
  KeyI = 0x17,
  KeyO = 0x18,
  KeyP = 0x19,
  KeyUe = 0x1a,         // originally '['
  KeyPlus = 0x1b,       // originally ']'
  KeyEnter = 0x1c,
  KeyLeftCtrl = 0x1d,
  KeyA = 0x1e,
  KeyS = 0x1f,
  KeyD = 0x20,
  KeyF = 0x21,
  KeyG = 0x22,
  KeyH = 0x23,
  KeyJ = 0x24,
  KeyK = 0x25,
  KeyL = 0x26,
  KeyOe = 0x27,          // originally ';'
  KeyAe = 0x28,          // originally '''
  KeyCircumflex = 0x29,  // originally '`'
  KeyLeftShift = 0x2a,
  KeyHash = 0x2b,        // originally '\'
  KeyY = 0x2c,           // originally 'z'
  KeyX = 0x2d,
  KeyC = 0x2e,
  KeyV = 0x2f,
  KeyB = 0x30,
  KeyN = 0x31,
  KeyM = 0x32,
  KeyComma = 0x33,
  KeyDot = 0x34,
  KeyDash = 0x35,        // originally '/'
  KeyRightShift = 0x36,
  KeyNumPadStar = 0x37,
  KeyLeftAlt = 0x38,
  KeySpace = 0x39,
  KeyCapsLock = 0x3a,
  KeyF1 = 0x3b,
  KeyF2 = 0x3c,
  KeyF3 = 0x3d,
  KeyF4 = 0x3e,
  KeyF5 = 0x3f,
  KeyF6 = 0x40,
  KeyF7 = 0x41,
  KeyF8 = 0x42,
  KeyF9 = 0x43,
  KeyF10 = 0x44,
  KeyNumLock = 0x45,
  KeyScrollLock = 0x46,
  KeyNumPad7 = 0x47,
  KeyNumPad8 = 0x48,
  KeyNumPad9 = 0x49,
  KeyNumPadMinus = 0x4a,
  KeyNumPad4 = 0x4b,
  KeyNumPad5 = 0x4c,
  KeyNumPad6 = 0x4d,
  KeyNumPadPlus = 0x4e,
  KeyNumPad1 = 0x4f,
  KeyNumPad2 = 0x50,
  KeyNumPad3 = 0x51,
  KeyNumPad0 = 0x52,
  KeyNumPadDot = 0x53,
  KeyTag = 0x56,

} Scancode;


typedef struct {
  char Name[8];
  char Lowercase[128];
  char Uppercase[128];
  char Alternate[128];
} Charset;


extern void Keyboard_SetCharset(Charset *charset);
extern void Keyboard_SetInputStream(Stream *inputStream);
extern char Keyboard_GetChar(Charset *charset, unsigned char scanCode);
extern Charset Keyboard_GetGermanCharset(void);

extern void _ISubR_KeyboardHandler(void);

#endif
