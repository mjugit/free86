#include "Include/Keyboard.h"

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static bool ShiftPressed = false;
static bool AltPressed = false;

static Charset *Charset1;
static Stream *InputStream;

void _ISubR_KeyboardHandler(void) {
  unsigned char scancode = inb(0x60);

  switch (scancode) {
  case KeyLeftShift:
  case KeyRightShift:
    ShiftPressed = !ShiftPressed;
    return;

  case BREAK_CODE(KeyLeftShift):
  case BREAK_CODE(KeyRightShift):
    ShiftPressed = !ShiftPressed;
    return;

  default:
    if (scancode & 0x80)
      return;
    
    char character;
    if ((character = Keyboard_GetChar(Charset1, scancode)))
      Stream_Write(InputStream, character);
    break;
  }
}


void Keyboard_SetCharset(Charset *charset) {
  Charset1 = charset;
}

void Keyboard_SetInputStream(Stream *inputStream) {
  InputStream = inputStream;
}

char Keyboard_GetChar(Charset *charset, unsigned char scanCode) {
  char *map = AltPressed
    ? charset->Alternate
    : (ShiftPressed
       ? charset->Uppercase
       : charset->Lowercase);

  return map[scanCode];
}


Charset Keyboard_GetGermanCharset(void) {
  Charset german= {
    .Name = "German"
  };

  german.Lowercase[Key1] = german.Lowercase[KeyNumPad1] = german.Uppercase[KeyNumPad1] = '1';
  german.Lowercase[Key2] = german.Lowercase[KeyNumPad2] = german.Uppercase[KeyNumPad2] = '2';
  german.Lowercase[Key3] = german.Lowercase[KeyNumPad3] = german.Uppercase[KeyNumPad3] = '3';
  german.Lowercase[Key4] = german.Lowercase[KeyNumPad4] = german.Uppercase[KeyNumPad4] = '4';
  german.Lowercase[Key5] = german.Lowercase[KeyNumPad5] = german.Uppercase[KeyNumPad5] = '5';
  german.Lowercase[Key6] = german.Lowercase[KeyNumPad6] = german.Uppercase[KeyNumPad6] = '6';
  german.Lowercase[Key7] = german.Lowercase[KeyNumPad7] = german.Uppercase[KeyNumPad7] = '7';
  german.Lowercase[Key8] = german.Lowercase[KeyNumPad8] = german.Uppercase[KeyNumPad8] = '8';
  german.Lowercase[Key9] = german.Lowercase[KeyNumPad9] = german.Uppercase[KeyNumPad9] = '9';
  german.Lowercase[Key0] = german.Lowercase[KeyNumPad0] = german.Uppercase[KeyNumPad0] = '0';

  german.Lowercase[KeyA] = 'a';
  german.Uppercase[KeyA] = 'A';
  german.Lowercase[KeyB] = 'b';
  german.Uppercase[KeyB] = 'B';
  german.Lowercase[KeyC] = 'c';
  german.Uppercase[KeyC] = 'C';
  german.Lowercase[KeyD] = 'd';
  german.Uppercase[KeyD] = 'D';
  german.Lowercase[KeyE] = 'e';
  german.Uppercase[KeyE] = 'E';
  german.Lowercase[KeyF] = 'f';
  german.Uppercase[KeyF] = 'F';
  german.Lowercase[KeyG] = 'g';
  german.Uppercase[KeyG] = 'G';
  german.Lowercase[KeyH] = 'h';
  german.Uppercase[KeyH] = 'H';
  german.Lowercase[KeyI] = 'i';
  german.Uppercase[KeyI] = 'I';
  german.Lowercase[KeyJ] = 'j';
  german.Uppercase[KeyJ] = 'J';
  german.Lowercase[KeyK] = 'k';
  german.Uppercase[KeyK] = 'K';
  german.Lowercase[KeyL] = 'l';
  german.Uppercase[KeyL] = 'L';
  german.Lowercase[KeyM] = 'm';
  german.Uppercase[KeyM] = 'm';
  german.Lowercase[KeyN] = 'n';
  german.Uppercase[KeyN] = 'N';
  german.Lowercase[KeyO] = 'o';
  german.Uppercase[KeyO] = 'O';
  german.Lowercase[KeyP] = 'p';
  german.Uppercase[KeyP] = 'P';
  german.Lowercase[KeyQ] = 'q';
  german.Uppercase[KeyQ] = 'Q';
  german.Lowercase[KeyR] = 'r';
  german.Uppercase[KeyR] = 'R';   
  german.Lowercase[KeyS] = 's';
  german.Uppercase[KeyS] = 'S';
  german.Lowercase[KeyT] = 't';
  german.Uppercase[KeyT] = 'T';
  german.Lowercase[KeyU] = 'u';
  german.Uppercase[KeyU] = 'U';
  german.Lowercase[KeyV] = 'v';
  german.Uppercase[KeyV] = 'V';
  german.Lowercase[KeyW] = 'w';
  german.Uppercase[KeyW] = 'W';
  german.Lowercase[KeyX] = 'x';
  german.Uppercase[KeyX] = 'X';
  german.Lowercase[KeyY] = 'y';
  german.Uppercase[KeyY] = 'Y';
  german.Lowercase[KeyZ] = 'z';
  german.Uppercase[KeyZ] = 'Z';

  german.Lowercase[KeyDot] = '.';
  german.Uppercase[KeyDot] = ':';
  german.Lowercase[KeyComma] = ',';
  german.Uppercase[KeyComma] = ';';
  german.Lowercase[KeyDash] = '-';
  german.Uppercase[KeyDash] = '_';
  german.Lowercase[KeyHash] = '#';
  german.Uppercase[KeyHash] = '\'';

  german.Uppercase[Key1] = '!';
  german.Uppercase[Key2] = '\"';
  german.Uppercase[Key4] = '$';
  german.Uppercase[Key5] = '%';
  german.Uppercase[Key6] = '&';
  german.Uppercase[Key7] = '/';
  german.Uppercase[Key8] = '(';
  german.Uppercase[Key9] = ')';
  german.Uppercase[Key0] = '=';
  german.Uppercase[KeySz] = '?';

  german.Lowercase[KeyTag] = '<';
  german.Uppercase[KeyTag] = '>';

  german.Uppercase[KeySpace] = german.Lowercase[KeySpace] = ' ';
  german.Uppercase[KeyBackspace] = german.Lowercase[KeyBackspace] = '\b';
  german.Uppercase[KeyTab] = german.Lowercase[KeyTab] = '\t';
  german.Uppercase[KeyEnter] = german.Lowercase[KeyEnter] = '\n';

  return german;
}
