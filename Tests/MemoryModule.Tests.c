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

#include "../Source/Modules/Include/Memory.h"

use(Memory);


MU_TEST(Memory_Copy__Always__CopiesSourceToDestination) {
  const int SOURCE_SIZE = 100;
  const int DESTINATION_SIZE = 120;
  
  char source[SOURCE_SIZE];
  char destination[DESTINATION_SIZE];

  // Prepare test
  for (int index = 0; index < SOURCE_SIZE; index++)
    source[index] = 0xff;
  
  for (int index = 0; index < DESTINATION_SIZE; index++)
    destination[index] = 0x00;

  
  // Method to test
  Memory.Copy(destination, source, SOURCE_SIZE);

  
  // Check results
  for (int index = 0; index < SOURCE_SIZE; index++)
    if (source[index] != destination[index])
      mu_fail("Did not copy all bytes.");
  
  for (int index = SOURCE_SIZE; index < DESTINATION_SIZE; index++)
    if (destination[index] != 0x00)
      mu_fail("Copied to much.");
}

MU_TEST_SUITE(Memory_Copy) {
  MU_RUN_TEST(Memory_Copy__Always__CopiesSourceToDestination);
}



MU_TEST(Memory_Set__Always__SetsBlockOfMemoryToValue) {
  const int BLOCK_SIZE = 100;
  const int BUFFER_SIZE = BLOCK_SIZE * 2;

  char buffer[BUFFER_SIZE];

  // Prepare test
  for (int index = 0; index < BUFFER_SIZE; index++)
    buffer[index] = 0x00;


  // Method to test
  Memory.Set(buffer, 0xff, BLOCK_SIZE);


  // Check results
  for (int index = 0; index < BLOCK_SIZE; index++)
    if (buffer[index] == 0x00) {
      char message[100];
      sprintf(message, "Not all bytes have been set: Index = %i is zero.", index);
      mu_fail(message);
    }

  for (int index = BLOCK_SIZE; index < BUFFER_SIZE; index++)
    if (buffer[index] != 0x00)
      mu_fail("Exceeded block limit.");
}

MU_TEST_SUITE(Memory_Set) {
  MU_RUN_TEST(Memory_Set__Always__SetsBlockOfMemoryToValue);
}



MU_TEST(Memory_Move__ForwardCopy__MovesBlockToNewDestination) {
  const int BLOCK_SIZE = 100;
  const int BUFFER_SIZE = BLOCK_SIZE * 2;

  char buffer[BUFFER_SIZE];

  // Prepare test
  for (int index = 0; index < BUFFER_SIZE; index++)
    buffer[index] = index;

  // Keep a copy of the original buffer for later comparison
  char original[BUFFER_SIZE];
  memcpy(original, buffer, BUFFER_SIZE);

  // Method to test
  const int destinationIndex = 50;
  char* source = buffer;
  char* destination = &buffer[destinationIndex];
  Memory.Move(source, destination, BLOCK_SIZE);

  // Check copied block
  for (int index = destinationIndex; index < destinationIndex + BLOCK_SIZE; index++) {
    int expectedValue = index - destinationIndex;
    if (buffer[index] != expectedValue) {
      char message[100];
      sprintf(message, "Did not copy properly: Index = %i is %i (expected %i).",
              index, buffer[index], expectedValue);
      mu_fail(message);
    }
  }

  // Check data before destination is unchanged
  for (int index = 0; index < destinationIndex; index++) {
    if (buffer[index] != original[index]) {
      char message[100];
      sprintf(message, "Data before destination changed: Index = %i is %i (expected %i).",
              index, buffer[index], original[index]);
      mu_fail(message);
    }
  }

  // Check data after copied block is unchanged
  int afterIndex = destinationIndex + BLOCK_SIZE;
  for (int index = afterIndex; index < BUFFER_SIZE; index++) {
    if (buffer[index] != original[index]) {
      char message[100];
      sprintf(message, "Data after copied block changed: Index = %i is %i (expected %i).",
              index, buffer[index], original[index]);
      mu_fail(message);
    }
  }
}

MU_TEST(Memory_Move__BackwardCopy__MovesBlockToNewDestination) {
  const int BLOCK_SIZE = 100;
  const int BUFFER_SIZE = BLOCK_SIZE * 2;

  char buffer[BUFFER_SIZE];

  // Prepare test
  for (int index = 0; index < BUFFER_SIZE; index++)
    buffer[index] = index;

  // Keep a copy of the original buffer for later comparison
  char original[BUFFER_SIZE];
  memcpy(original, buffer, BUFFER_SIZE);

  // Method to test
  const int sourceIndex = 50;
  char* source = &buffer[sourceIndex];
  char* destination = buffer; // backward copy case
  Memory.Move(source, destination, BLOCK_SIZE);

  // Check copied block
  for (int index = 0; index < BLOCK_SIZE; index++) {
    int expectedValue = original[sourceIndex + index];
    if (destination[index] != expectedValue) {
      char message[100];
      sprintf(message, "Did not copy properly: Index = %i is %i (expected %i).",
              index, destination[index], expectedValue);
      mu_fail(message);
    }
  }

  // Check data before copied block (none in this case)

  // Check data after copied block is unchanged
  int afterIndex = BLOCK_SIZE;
  for (int index = afterIndex; index < BUFFER_SIZE; index++) {
    if (buffer[index] != original[index]) {
      char message[100];
      sprintf(message, "Data after copied block changed: Index = %i is %i (expected %i).",
              index, buffer[index], original[index]);
      mu_fail(message);
    }
  }
}

MU_TEST_SUITE(Memory_Move) {
  MU_RUN_TEST(Memory_Move__ForwardCopy__MovesBlockToNewDestination);
  MU_RUN_TEST(Memory_Move__BackwardCopy__MovesBlockToNewDestination);
}



MU_TEST(Memory_Compare__BlocksAreIdentical__ReturnsZero) {
  const int SIZE = 128;
  char a[SIZE];
  char b[SIZE];

  // Prepare test
  for (int index = 0; index < SIZE; index++) {
    a[index] = index;
    b[index] = index;
  }

  // Method to test
  int result = Memory.Compare(a, b, SIZE);

  // Check result
  if (result != 0) {
    char message[100];
    sprintf(message, "Expected 0 for identical blocks, got %i.", result);
    mu_fail(message);
  }
}

MU_TEST(Memory_Compare__BlocksAreNotIdentical__ReturnsDiffOfFirstUnidenticalBytes) {
  const int SIZE = 128;
  char a[SIZE];
  char b[SIZE];

  // Prepare test
  for (int index = 0; index < SIZE; index++) {
    a[index] = index;
    b[index] = index;
  }

  // Introduce difference at a specific position
  const int diffIndex = 50;
  a[diffIndex] = 10;
  b[diffIndex] = 20;

  // Method to test
  int result = Memory.Compare(a, b, SIZE);

  // Calculate expected result
  int expected = (unsigned char)a[diffIndex] - (unsigned char)b[diffIndex];

  // Check result
  if (result != expected) {
    char message[120];
    sprintf(message,
            "Expected %i for difference at index %i, got %i.",
            expected, diffIndex, result);
    mu_fail(message);
  }
}

MU_TEST_SUITE(Memory_Compare) {
  MU_RUN_TEST(Memory_Compare__BlocksAreIdentical__ReturnsZero);
  MU_RUN_TEST(Memory_Compare__BlocksAreNotIdentical__ReturnsDiffOfFirstUnidenticalBytes);
}



MU_TEST(Memory_Find__NoMatch__ReturnsNull) {
  const int SIZE = 64;
  char buffer[SIZE];

  // Prepare test
  for (int index = 0; index < SIZE; index++)
    buffer[index] = index;

  // Search value not present in buffer
  unsigned char searchValue = 255;

  // Method to test
  void* result = Memory.Find(buffer, searchValue, SIZE);

  // Check result
  if (result != NULL) {
    char message[100];
    sprintf(message, "Expected NULL for value %u not in buffer, got non-NULL pointer.", searchValue);
    mu_fail(message);
  }
}

MU_TEST(Memory_Find__MatchExists__ReturnsPointer) {
  const int SIZE = 64;
  char buffer[SIZE];

  // Prepare test
  for (int index = 0; index < SIZE; index++)
    buffer[index] = index;

  // Pick a value from the buffer
  const int matchIndex = 25;
  unsigned char searchValue = buffer[matchIndex];

  // Method to test
  void* result = Memory.Find(buffer, searchValue, SIZE);

  // Check result
  if (result != &buffer[matchIndex]) {
    char message[120];
    sprintf(message, "Expected pointer to index %i for value %u, got %p (expected %p).",
            matchIndex, searchValue, result, &buffer[matchIndex]);
    mu_fail(message);
  }
}

MU_TEST_SUITE(Memory_Find) {
  MU_RUN_TEST(Memory_Find__NoMatch__ReturnsNull);
  MU_RUN_TEST(Memory_Find__MatchExists__ReturnsPointer);
}


int main(void) {
  MU_RUN_SUITE(Memory_Copy);
  MU_RUN_SUITE(Memory_Set);
  MU_RUN_SUITE(Memory_Move);
  MU_RUN_SUITE(Memory_Compare);
  MU_RUN_SUITE(Memory_Find);
  
  MU_REPORT();
  
  return MU_EXIT_CODE;
}

