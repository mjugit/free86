#include "../Include/String.h"


static void _PrintIntAsHex(uint32_t value, char *buffer, int length) {
  static const char alphabet[] = "0123456789abcdef";
  buffer[length] = '\0';
  
  while (length--) {
    buffer[length] = alphabet[value & 0xf];
    
    value >>= 4;
  }
}

static void _PrintIntAsBinary(uint32_t value, char *buffer, int length) {
  buffer[length] = '\0';

  while(length--) {
    buffer[length] = (value & 1)
      ? '1'
      : '0';

    value >>= 1;
  }
}

static void _PrintIntAsDecimal(uint32_t value, char *buffer, int length) {
  buffer[length] = '\0';
  
  for (int i = length - 1; i >= 0; i--) {
    buffer[i] = (value % 10) + '0';
    value /= 10;
  }
}


void String_Format(char *destination, const char *format, ...) {
  va_list arg_list;
  va_start(arg_list, format);

  while (*format) {
    if (*format != '%') {
      *destination++ = *format++;
      continue;
    }

    char formatType = *(format + 1);
    
        
    if (formatType == 'x') {
      format += 2;
      *destination++ = '0';
      *destination++ = 'x';
      
      switch (*format) {
      case 'b':
	_PrintIntAsHex(va_arg(arg_list, int), destination, 2);
	destination += 2;
	format ++;
	break;

      case 'w':
	_PrintIntAsHex(va_arg(arg_list, int), destination, 4);
	destination += 4;
	format ++;
	break;

      case 'l':
	_PrintIntAsHex(va_arg(arg_list, long), destination, 16);
	destination += 16;
	format++;
	break;

      default:
	_PrintIntAsHex(va_arg(arg_list, int), destination, 8);
	destination += 8;
	break;
	
      }

    } else if (formatType == 'b') {

      // BINARY FORMATTING
      
      format += 2;
      
      switch (*format) {
      case 'b':
	_PrintIntAsBinary(va_arg(arg_list, int), destination, 8);
	destination += 8;
	format ++;
	break;

      case 'w':
	_PrintIntAsBinary(va_arg(arg_list, int), destination, 16);
	destination += 16;
	format ++;
	break;

      case 'l':
	_PrintIntAsBinary(va_arg(arg_list, int), destination, 32);
	destination += 32;
	format++;
	break;

      default:
	_PrintIntAsBinary(va_arg(arg_list, int), destination, 32);
	destination += 32;
	break;
	
      }

      
      *destination++ = 'b';
    } else if (formatType == 'd') {
      format += 2;

      switch (*format) {
      case 'b':
	_PrintIntAsDecimal(va_arg(arg_list, int), destination, 3);
	destination += 3;
	format ++;
	break;

      case 'w':
	_PrintIntAsDecimal(va_arg(arg_list, int), destination, 5);
	destination += 5;
	format ++;
	break;

      case 'l':
	_PrintIntAsDecimal(va_arg(arg_list, int), destination, 10);
	destination += 10;
	break;

      default:
	_PrintIntAsDecimal(va_arg(arg_list, int), destination, 10);
	destination += 10;
	break;
	
      }

    }
  }

  *destination++ = '\0';
  va_end(arg_list);
}

