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

#include "../Source/Libs/Include/Bitmap.h"


MU_TEST(Bitmap_Set__SingleBitUnset__SetsBit) {
  unsigned char bitmap[4] = {0x00, 0x00, 0x00, 0x00};

  // Method to test
  Bitmap.Set(bitmap, 3);

  // Check results
  if ((bitmap[0] & (1 << 3)) == 0)
    mu_fail("Bit 3 was not set.");

  unsigned char expected[4] = {0x08, 0x00, 0x00, 0x00};
  for (int index = 0; index < 4; index++)
    if (bitmap[index] != expected[index]) {
      char message[100];
      sprintf(message, "Unexpected change at byte %i: got 0x%02X (expected 0x%02X).",
              index, bitmap[index], expected[index]);
      mu_fail(message);
    }
}

MU_TEST(Bitmap_Set__SingleBitSet__LeavesOthersUnchanged) {
  unsigned char bitmap[4] = {0xFF, 0xAA, 0x55, 0xCC};

  // Method to test
  Bitmap.Set(bitmap, 4); // bit 4 is already set in 0xFF

  // Check results
  unsigned char expected[4] = {0xFF, 0xAA, 0x55, 0xCC};
  for (int index = 0; index < 4; index++)
    if (bitmap[index] != expected[index]) {
      char message[100];
      sprintf(message, "Bitmap content changed unexpectedly at byte %i: got 0x%02X (expected 0x%02X).",
              index, bitmap[index], expected[index]);
      mu_fail(message);
    }
}

MU_TEST_SUITE(Bitmap_Set) {
  MU_RUN_TEST(Bitmap_Set__SingleBitUnset__SetsBit);
  MU_RUN_TEST(Bitmap_Set__SingleBitSet__LeavesOthersUnchanged);
}



MU_TEST(Bitmap_Clear__SingleBitSet__ClearsBit) {
  unsigned char bitmap[4] = {0xFF, 0xFF, 0xFF, 0xFF};

  // Method to test
  Bitmap.Clear(bitmap, 7);

  // Check results
  if ((bitmap[0] & (1 << 7)) != 0)
    mu_fail("Bit 7 was not cleared.");

  unsigned char expected[4] = {0x7F, 0xFF, 0xFF, 0xFF};
  for (int index = 0; index < 4; index++)
    if (bitmap[index] != expected[index]) {
      char message[100];
      sprintf(message, "Unexpected change at byte %i: got 0x%02X (expected 0x%02X).",
              index, bitmap[index], expected[index]);
      mu_fail(message);
    }
}

MU_TEST(Bitmap_Clear__SingleBitUnset__LeavesOthersUnchanged) {
  unsigned char bitmap[4] = {0x0F, 0xAA, 0x55, 0xCC};

  // Method to test
  Bitmap.Clear(bitmap, 5); // bit 5 in byte 0 ist 0 und soll unverändert bleiben

  
  // Check results
  unsigned char expected[4] = {0x0F, 0xAA, 0x55, 0xCC};
  for (int index = 0; index < 4; index++)
    if (bitmap[index] != expected[index]) {
      char message[100];
      sprintf(message, "Bitmap content changed unexpectedly at byte %i: got 0x%02X (expected 0x%02X).",
              index, bitmap[index], expected[index]);
      mu_fail(message);
    }
}

MU_TEST_SUITE(Bitmap_Clear) {
  MU_RUN_TEST(Bitmap_Clear__SingleBitSet__ClearsBit);
  MU_RUN_TEST(Bitmap_Clear__SingleBitUnset__LeavesOthersUnchanged);
}



MU_TEST(Bitmap_Check__BitIsSet__ReturnsOne) {
  unsigned char bitmap[4] = {0x10, 0x00, 0x00, 0x00};

  // Method to test
  int result = Bitmap.Check(bitmap, 4);

  // Check results
  if (result != 1) {
    char message[100];
    sprintf(message, "Expected 1 for bit 4 set, got %i.", result);
    mu_fail(message);
  }
}

MU_TEST(Bitmap_Check__BitIsUnset__ReturnsZero) {
  unsigned char bitmap[4] = {0x00, 0x00, 0x00, 0x00};

  // Method to test
  int result = Bitmap.Check(bitmap, 2);

  // Check results
  if (result != 0) {
    char message[100];
    sprintf(message, "Expected 0 for bit 2 unset, got %i.", result);
    mu_fail(message);
  }
}

MU_TEST_SUITE(Bitmap_Check) {
  MU_RUN_TEST(Bitmap_Check__BitIsSet__ReturnsOne);
  MU_RUN_TEST(Bitmap_Check__BitIsUnset__ReturnsZero);
}



MU_TEST(Bitmap_Wipe__Always__ClearsAllBits) {
  unsigned char bitmap[8] = {0xFF, 0xFF, 0xFF, 0xFF,
                             0xFF, 0xFF, 0xFF, 0xFF};

  // Method to test
  Bitmap.Wipe(bitmap, 64);

  // Check results
  for (int index = 0; index < 8; index++)
    if (bitmap[index] != 0x00) {
      char message[100];
      sprintf(message, "Bitmap not fully cleared at byte %i: got 0x%02X.", index, bitmap[index]);
      mu_fail(message);
    }
}

MU_TEST(Bitmap_Wipe__Partial__ClearsOnlyWithinRange) {
  unsigned char bitmap[4] = {0xFF, 0xFF, 0xFF, 0xFF};

  // Method to test
  Bitmap.Wipe(bitmap, 8); // should clear exactly 1 byte

  // Check results
  if (bitmap[0] != 0x00)
    mu_fail("First byte should be cleared.");

  for (int index = 1; index < 4; index++)
    if (bitmap[index] != 0xFF) {
      char message[100];
      sprintf(message, "Byte %i changed unexpectedly: got 0x%02X (expected 0xFF).",
              index, bitmap[index]);
      mu_fail(message);
    }
}

MU_TEST_SUITE(Bitmap_Wipe) {
  MU_RUN_TEST(Bitmap_Wipe__Always__ClearsAllBits);
  MU_RUN_TEST(Bitmap_Wipe__Partial__ClearsOnlyWithinRange);
}



MU_TEST(Bitmap_NextFree__AllClear__ReturnsZero) {
  unsigned char bitmap[4] = {0x00, 0x00, 0x00, 0x00};

  // Method to test
  int result = Bitmap.NextFree(bitmap, 1, 16);

  // Check results
  if (result != 0) {
    char message[100];
    sprintf(message, "Expected 0 for empty bitmap (1 free), got %i.", result);
    mu_fail(message);
  }
}

MU_TEST(Bitmap_NextFree__PartialFill__ReturnsFirstFreeBitIndex) {
  unsigned char bitmap[4] = {0xFF, 0x0F, 0x00, 0x00}; 

  // Method to test
  int result = Bitmap.NextFree(bitmap, 1, 16);

  // Check results
  if (result != 12) {
    char message[100];
    sprintf(message, "Expected 12 for first free bit, got %i.", result);
    mu_fail(message);
  }
}

MU_TEST(Bitmap_NextFree__AllSet__ReturnsMinusOne) {
  unsigned char bitmap[4] = {0xFF, 0xFF, 0xFF, 0xFF};

  // Method to test
  int result = Bitmap.NextFree(bitmap, 1, 16);

  // Check results
  if (result != -1) {
    char message[100];
    sprintf(message, "Expected -1 for full bitmap, got %i.", result);
    mu_fail(message);
  }
}

MU_TEST(Bitmap_NextFree__MultipleFreeBitsInSameByte__ReturnsStartIndex) {
  // Byte 0: 0b11100011 → bits 2, 3, 4 are free
  unsigned char bitmap[2] = {0xE3, 0xFF};

  // Search for 3 consecutive free bits
  int result = Bitmap.NextFree(bitmap, 3, 16);

  if (result != 2) {
    char message[100];
    sprintf(message, "Expected start index 2 for 3 free bits in same byte, got %i.", result);
    mu_fail(message);
  }
}

MU_TEST(Bitmap_NextFree__MultipleFreeBitsAcrossBytes__ReturnsStartIndex) {
  // Byte 0: 0x7F = 0b01111111 → bit 7 is free
  // Byte 1: 0xFC = 0b11111100 → bits 0 and 1 are free
  // Free run at indices 7, 8, 9 → start = 7
  unsigned char bitmap[2] = {0x7F, 0xFC};

  // Search for 3 consecutive free bits
  int result = Bitmap.NextFree(bitmap, 3, 16);

  if (result != 7) {
    char message[100];
    sprintf(message, "Expected start index 7 for 3 free bits across bytes, got %i.", result);
    mu_fail(message);
  }
}

MU_TEST_SUITE(Bitmap_NextFree) {
  MU_RUN_TEST(Bitmap_NextFree__AllClear__ReturnsZero);
  MU_RUN_TEST(Bitmap_NextFree__PartialFill__ReturnsFirstFreeBitIndex);
  MU_RUN_TEST(Bitmap_NextFree__AllSet__ReturnsMinusOne);
  MU_RUN_TEST(Bitmap_NextFree__MultipleFreeBitsInSameByte__ReturnsStartIndex);
  MU_RUN_TEST(Bitmap_NextFree__MultipleFreeBitsAcrossBytes__ReturnsStartIndex);
}



int main(void) {
  MU_RUN_SUITE(Bitmap_Set);
  MU_RUN_SUITE(Bitmap_Clear);
  MU_RUN_SUITE(Bitmap_Check);
  MU_RUN_SUITE(Bitmap_Wipe);
  MU_RUN_SUITE(Bitmap_NextFree);

  MU_REPORT();

  return MU_EXIT_CODE;
}
