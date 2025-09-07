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


#include "../Include/String.h"


static const string Alphabet = "0123456789abcdef";


static void _StringFormat_WriteHexInt(U32 value, string destination, U8 length);
static void _PrintIntAsDecimal(uint32_t value, char *buffer, int length);

void _String_FormatImplementation(string destination, const string formatStr, ...) {
  va_list argumentList;
  va_start(argumentList, formatStr);

  for (char *formatPtr = formatStr; *formatPtr; formatPtr++) {
    if (*formatPtr != '%') {
      *destination++ = *formatPtr;
      continue;
      }

    char formatType = *(formatPtr + 1);
    switch (formatType) {
    case 'x':
      formatPtr++;
      *destination++ = '0';
      *destination++ = 'x';

      switch (*formatPtr) {
      case 'b':
	_StringFormat_WriteHexInt(va_arg(argumentList, int), destination, 2);
	destination += 2;
	formatPtr++;
	break;

      case 'd':
	_StringFormat_WriteHexInt(va_arg(argumentList, int), destination, 4);
	destination += 4;
	formatPtr++;
	break;

      case 'l':
	_StringFormat_WriteHexInt(va_arg(argumentList, int), destination, 16);
	destination += 16;
	formatPtr++;
	break;

      default:
	_StringFormat_WriteHexInt(va_arg(argumentList, int), destination, 8);
	destination += 8;
	break;
      }
      
      break;


    case 'd':
      switch (*formatPtr) {
      case 'b':
	_PrintIntAsDecimal(va_arg(argumentList, int), destination, 3);
	destination += 3;
	formatPtr++;
	break;

      case 'd':
	_PrintIntAsDecimal(va_arg(argumentList, int), destination, 5);
	destination += 5;
	formatPtr++;
	break;

      case 'l':
	_PrintIntAsDecimal(va_arg(argumentList, int), destination, 10);
	destination += 10;
	formatPtr++;
	break;

      default:
	_PrintIntAsDecimal(va_arg(argumentList, int), destination, 10);
	destination += 10;
	formatPtr++;
	break;
	
      }
    }
    
  }
  
  *destination++ = '\0';
  va_end(argumentList);
}



static void _StringFormat_WriteHexInt(U32 value, string destination, U8 length) {
  destination[length] = '\0';

  while (length--) {
    destination[length] = Alphabet[value & 0xf];
    value >>= 4;
  }
}

static void _PrintIntAsDecimal(uint32_t value, char *buffer, int length) {
  buffer[length] = '\0';
  
  for (int i = length - 1; i >= 0; i--) {
    buffer[i] = (value % 10) + '0';
    value /= 10;
  }
}
