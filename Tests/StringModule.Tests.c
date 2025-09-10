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


#include "MinUnit.h"

#include "../Source/Modules/Include/String.h"


use(String);


// Get length

MU_TEST(String_GetLength__EmptyString__ReturnsZero) {
  string testString = "\0";

  mu_check(String.GetLength(testString) == 0);
}

MU_TEST(String_GetLength__NonEmptyString__ReturnsLength) {
  string testString = "1234567890\0";

  mu_check(String.GetLength(testString) == 10);
}

MU_TEST_SUITE(String_GetLength) {
  MU_RUN_TEST(String_GetLength__EmptyString__ReturnsZero);
  MU_RUN_TEST(String_GetLength__NonEmptyString__ReturnsLength);
}



// Reverse

MU_TEST(String_Reverse__NotEmpty__ReversesString) {
  // Has to be defined with []; otherwise it is relocated to .rodata
  char testString[] = "1234567890\0";
  string expected = "0987654321\0";

  String.Reverse(testString);

  mu_check(memcmp(expected, testString, 10) == 0);
}

MU_TEST(String_Reverse__Empty__Returns) {
  // Has to be defined with []; otherwise it is relocated to .rodata
  char testString[] = "\0";

  String.Reverse(testString);
}

MU_TEST_SUITE(String_Reverse) {
  MU_RUN_TEST(String_Reverse__NotEmpty__ReversesString);
  MU_RUN_TEST(String_Reverse__Empty__Returns);
}



// Format

MU_TEST(String_Format__Decimal__FormatsIntoDecimal) {
  char testBuffer[100] = { };
  char expected[] = "1234\0";

  String.Format(testBuffer, "%d", 1234);

  mu_check(memcmp(testBuffer, expected, 5) == 0);
}

MU_TEST_SUITE(String_Format) {
  MU_RUN_TEST(String_Format__Decimal__FormatsIntoDecimal);
}


int main(void) {
  MU_RUN_SUITE(String_GetLength);
  MU_RUN_SUITE(String_Reverse);
  MU_RUN_SUITE(String_Format);

  MU_REPORT();

  return MU_EXIT_CODE;
}
