#ifndef _STRING_H_
#define _STRING_H_

#include <stdarg.h>
#include <stdint.h>

/**
 * @brief Calculates the length of a null-terminated string.
 *
 * This function iterates through the characters of the given string
 * until it encounters the null terminator (`'\0'`). The length of the
 * string, excluding the null terminator, is then returned.
 *
 * @param string A pointer to the null-terminated string.
 * @return The length of the string (number of characters before the null terminator).
 */
extern unsigned int String_GetLength(const char *string);


/**
 * @brief Formats a string with variable arguments and writes it to a destination buffer.
 *
 * This function processes a format string with placeholders and replaces
 * them with formatted values based on the variable arguments provided.
 *
 * Supported placeholders:
 * - `%x[b|w|l]`: Hexadecimal format (`b`=byte, `w`=word, `l`=long).
 * - `%b[b|w|l]`: Binary format (`b`=byte, `w`=word, `l`=long).
 * - `%d[b|w|l]`: Decimal format (`b`=byte, `w`=word, `l`=long).
 *
 * @param destination A pointer to the buffer where the formatted string will be written.
 * @param format The format string containing placeholders.
 * @param ... Variable arguments matching the format specifiers.
 */
extern void String_Format(char *destination, const char *format, ...);

#endif
