#ifndef _STRING_H_
#define _STRING_H_

#include <stdint.h>
#include <stdarg.h>

extern unsigned int string_get_length(const char *string);
extern void string_format(char *destination, const char *format, ...);

#endif
