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


#include "../Include/Keyboard.h"


U16	_Keyboard_ReadScanCodeImplementation(void);
KeyCode _Keyboard_GetKeyCodeImplementation(U16 scanCode);
char	_Keyboard_GetCharImplementation(KeyCode keyCode, KeyModifiers modifiers);
void	_Keyboard_UpdateModifiersImplementation(KeyCode keyCode, bool keyDown, KeyModifiers *modifiersPtr);



members(Keyboard) {
  .ReadScanCode	   = _Keyboard_ReadScanCodeImplementation,
  .GetKeyCode	   = _Keyboard_GetKeyCodeImplementation,
  .UpdateModifiers = _Keyboard_UpdateModifiersImplementation,
  .GetChar	   = _Keyboard_GetCharImplementation
};
