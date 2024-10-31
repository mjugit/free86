#include "../include/string.h"

unsigned int string_get_length(const char *string) {
  unsigned int len = 0;

  for (; string[len]; len++)
    ;

  return len;
}
