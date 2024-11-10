#include "../Include/String.h"

unsigned int String_GetLength(const char *string) {
  unsigned int length = 0;

  for (; string[length]; length++)
    ;

  return length;
}
