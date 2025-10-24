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

#include "../Source/Modules/Include/Hash.h"

use(Hash);


MU_TEST(Murmur3_Finalizer__Always__ReturnsExpectedHash) {
  mu_assert_int_eq(0x00000000, Hash.Murmur.HashNumber(0x00000000));
  mu_assert_int_eq(0x514e28b7, Hash.Murmur.HashNumber(0x00000001));
  mu_assert_int_eq(0x81f16f39, Hash.Murmur.HashNumber(0xffffffff));
} 

MU_TEST(Murmur3_HashBlock__Always__ReturnsExpectedHash) {
  const char testInput1[] = "test";

  mu_assert_int_eq(0xa8e72cf1, Hash.Murmur.HashBlock(testInput1, 4, 0x9747b28c));
}


MU_TEST_SUITE(Murmur3) {
  MU_RUN_TEST(Murmur3_Finalizer__Always__ReturnsExpectedHash);
  MU_RUN_TEST(Murmur3_HashBlock__Always__ReturnsExpectedHash);
}


int main(void) {
  MU_RUN_SUITE(Murmur3);

  MU_REPORT();

  return MU_EXIT_CODE;
}
