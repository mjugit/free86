#include "../include/string.h"


void _integer_to_hex(uint32_t value, char *buffer, int length) {
  static const char alphabet[] = "0123456789abcdef";
  buffer[length] = '\0';
  
  while (length--) {
    buffer[length] = alphabet[value & 0xf];
    
    value >>= 4;
  }
}

void _integer_to_bin(uint32_t value, char *buffer, int length) {
  buffer[length] = '\0';

  while(length--) {
    buffer[length] = (value & 1)
      ? '1'
      : '0';

    value >>= 1;
  }
}


void string_format(char *destination, const char *format, ...) {
  va_list arg_list;
  va_start(arg_list, format);

  while (*format) {
    if (*format != '%') {
      *destination++ = *format++;
      continue;
    }
    
    if (*(format + 1) == 'x') {
      format += 2;
      *destination++ = '0';
      *destination++ = 'x';
      
      switch (*format) {
      case 'b':
	_integer_to_hex(va_arg(arg_list, int), destination, 2);
	destination += 2;
	format ++;
	break;

      case 'w':
	_integer_to_hex(va_arg(arg_list, int), destination, 4);
	destination += 4;
	format ++;
	break;

      case 'l':
	_integer_to_hex(va_arg(arg_list, long), destination, 16);
	destination += 16;
	format++;
	break;

      default:
	_integer_to_hex(va_arg(arg_list, int), destination, 8);
	destination += 8;
      }

    } else if (*(format + 1) == 'b') {

      // BINARY FORMATTING
      
      format += 2;
      
      switch (*format) {
      case 'b':
	_integer_to_bin(va_arg(arg_list, int), destination, 8);
	destination += 8;
	format ++;
	break;

      case 'w':
	_integer_to_bin(va_arg(arg_list, int), destination, 16);
	destination += 16;
	format ++;
	break;

      default:
	_integer_to_bin(va_arg(arg_list, int), destination, 32);
	destination += 32;
      }

      
      *destination++ = 'b';
    }
  }

  *destination++ = '\0';
  va_end(arg_list);
}
