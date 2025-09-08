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

#include "../Source/Modules/Include/Stream.h"


use(Stream);


// Get amount of bytes in buffer

MU_TEST(Stream_GetPendingBytes__NoPendingBytes__ReturnsZero) {
  stream testStream = {
    .ReadIndex = 15,
    .WriteIndex = 15
  };

  mu_check(Stream.GetPendingBytes(&testStream) == 0);
}

MU_TEST(Stream_GetPendingBytes__ReadIndexAfterWriteIndex__ReturnsAmount) {
  stream testStream = {
    .ReadIndex = 25,
    .WriteIndex = 10
  };

  mu_check(Stream.GetPendingBytes(&testStream) == 15);
}

MU_TEST(Stream_GetPendingBytes__ReadIndexBeforeWriteIndex__ReturnsAmount) {
  stream testStream = {
    .ReadIndex = 5,
    .WriteIndex = __STREAM_BUFFERSIZE - 1
  };

  mu_check(Stream.GetPendingBytes(&testStream) == 6);
}

MU_TEST_SUITE(Stream_GetPendingBytes) {
  MU_RUN_TEST(Stream_GetPendingBytes__NoPendingBytes__ReturnsZero);
  MU_RUN_TEST(Stream_GetPendingBytes__ReadIndexAfterWriteIndex__ReturnsAmount);
  MU_RUN_TEST(Stream_GetPendingBytes__ReadIndexBeforeWriteIndex__ReturnsAmount);
}


// Write

MU_TEST(Stream_Write__EnoughSpace__ReturnsAmountOfWrittenBytes) {
  stream testStream = {
    .ReadIndex = 0,
    .WriteIndex = 0
  };
  
  U8 testData[8] = { 1, 2, 3, 4, 5, 6, 7, 8};

  U16 bytesWritten = Stream.Write(&testStream, testData, sizeof(testData));

  mu_check(bytesWritten == sizeof(testData));
  mu_check(testStream.WriteIndex == sizeof(testData));
  mu_check(memcmp(testData, testStream.Buffer, sizeof(testData)) == 0);
}

MU_TEST(Stream_Write__NotEnoughSpace__ReturnsAmountOfWrittenBytes) {
  // 4 Bytes free
  stream testStream = {
    .ReadIndex = 5,
    .WriteIndex = 0
  };
  
  U8 testData[8] = { 1, 2, 3, 4, 5, 6, 7, 8};

  U16 bytesWritten = Stream.Write(&testStream, testData, sizeof(testData));

  mu_check(bytesWritten == 4);
  mu_check(memcmp(testData, testStream.Buffer, 4) == 0);
}

MU_TEST(Stream_Write__BufferFull__ReturnsZero) {
  // No bytes free
  stream testStream = {
    .ReadIndex = 5,
    .WriteIndex = 4
  };
  
  U8 testData[8] = { 1, 2, 3, 4, 5, 6, 7, 8};

  U16 bytesWritten = Stream.Write(&testStream, testData, sizeof(testData));

  mu_check(bytesWritten == 0);
}

MU_TEST_SUITE(Stream_Write) {
  MU_RUN_TEST(Stream_Write__EnoughSpace__ReturnsAmountOfWrittenBytes);
  MU_RUN_TEST(Stream_Write__NotEnoughSpace__ReturnsAmountOfWrittenBytes);
  MU_RUN_TEST(Stream_Write__BufferFull__ReturnsZero);
}


// Read

MU_TEST(Stream_Read__NothingToRead__ReturnsZero) {
  stream testStream = {
    .ReadIndex = 0,
    .WriteIndex = 0
  };

  U8 readBuffer[512] = { };
  U16 bytesRead = Stream.Read(&testStream, readBuffer, 16);

  mu_check(bytesRead == 0);
}

MU_TEST(Stream_Read__NoOverlap__ReturnsBytesRead) {
  U8 setupBuffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  // 8 Items in setupBuffer
  stream testStream = {
    .ReadIndex = 0,
    .WriteIndex = sizeof(setupBuffer)
  };

  memcpy(testStream.Buffer, setupBuffer, sizeof(setupBuffer));

  U8 readBuffer[512] = { };
  // Read explicitly more items, than stored (just a test)
  U16 bytesRead = Stream.Read(&testStream, readBuffer, 16);

  mu_check(bytesRead == sizeof(setupBuffer));
  mu_check(testStream.ReadIndex == testStream.WriteIndex);
  mu_check(memcmp(setupBuffer, readBuffer, sizeof(setupBuffer)) == 0);
}

MU_TEST(Stream_Read__WithOverlap__ReturnsBytesRead) {
  U8 setupBuffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  // 8 Items in setupBuffer
  stream testStream = {
    .ReadIndex = __STREAM_BUFFERSIZE - 4,
    .WriteIndex = 4
  };

  memcpy(&testStream.Buffer[__STREAM_BUFFERSIZE - 4], &setupBuffer[0], 4);
  memcpy(&testStream.Buffer[0], &setupBuffer[4], 4);

  U8 readBuffer[512] = { };
  // Read explicitly more items, than stored (just a test)
  U16 bytesRead = Stream.Read(&testStream, readBuffer, 16);

  mu_check(bytesRead == sizeof(setupBuffer));
  mu_check(testStream.ReadIndex == testStream.WriteIndex);
  mu_check(memcmp(setupBuffer, readBuffer, sizeof(setupBuffer)) == 0);
}


MU_TEST_SUITE(Stream_Read) {
  MU_RUN_TEST(Stream_Read__NothingToRead__ReturnsZero);
  MU_RUN_TEST(Stream_Read__NoOverlap__ReturnsBytesRead);
  MU_RUN_TEST(Stream_Read__WithOverlap__ReturnsBytesRead);
}



int main(void) {
  MU_RUN_SUITE(Stream_GetPendingBytes);
  MU_RUN_SUITE(Stream_Write);
  MU_RUN_SUITE(Stream_Read);

  MU_REPORT();

  return MU_EXIT_CODE;
}
