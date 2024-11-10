#ifndef _STRING_H_
#define _STRING_H_

#include <stdarg.h>
#include <stdint.h>

extern unsigned int String_GetLength(const char *string);
extern void String_Format(char *destination, const char *format, ...);

#endif
