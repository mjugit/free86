#include "../Include/String.h"

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
unsigned int String_GetLength(const char *string) {
  unsigned int length = 0;

  for (; string[length]; length++)
    ;

  return length;
}
