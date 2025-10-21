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
#include "../Source/Modules/Include/Heap.h"

use(Heap);


// List operations

MU_TEST(Heap_UnhingeListItem__Always__RemovesItemFromList) {
  MemorySlice previous, current, next;

  previous.NextSlice = &current;
  current.PreviousSlice = &previous;
  current.NextSlice = &next;
  next.PreviousSlice = &current;

  // Method to test
  _Heap_UnhingeListItem(&current);

  // Check results
  mu_check(previous.NextSlice == &next);
  mu_check(next.PreviousSlice == &previous);

  mu_check(!current.PreviousSlice && !current.NextSlice);
}

MU_TEST(Heap_InsertListItemBefore__Always__InsertsListItem) {
  MemorySlice previous, middle, next;

  previous.NextSlice = &middle;
  middle.PreviousSlice = &previous;
  middle.NextSlice = &next;
  next.PreviousSlice = &middle;

  MemorySlice testItem;

  // Method to test
  _Heap_InsertListItemBefore(&middle, &testItem);

  // Check results
  mu_check(previous.NextSlice == &testItem);
  mu_check(testItem.PreviousSlice == &previous);
  mu_check(testItem.NextSlice == &middle);
  mu_check(middle.PreviousSlice == &testItem);
}

MU_TEST(Heap_InsertListItemAfter__Always__InsertsListItem) {
  MemorySlice previous, middle, next;

  previous.NextSlice = &middle;
  middle.PreviousSlice = &previous;
  middle.NextSlice = &next;
  next.PreviousSlice = &middle;

  MemorySlice testItem;

  // Method to test
  _Heap_InsertListItemAfter(&middle, &testItem);

  // Check results
  mu_check(middle.NextSlice == &testItem);
  mu_check(testItem.PreviousSlice == &middle);
  mu_check(testItem.NextSlice == &next);
  mu_check(next.PreviousSlice == &testItem);
}

MU_TEST_SUITE(Heap_ListOperations) {
  MU_RUN_TEST(Heap_UnhingeListItem__Always__RemovesItemFromList);
  MU_RUN_TEST(Heap_InsertListItemBefore__Always__InsertsListItem);
  MU_RUN_TEST(Heap_InsertListItemAfter__Always__InsertsListItem);
}



// Slice operations

MU_TEST(Heap_SplitMemorySlice__Always__SplitsSlice) {
  U8 testBuffer[256] = { };

  MemorySlice previousSlice = {};
  MemorySlice nextSlice = {};
  
  MemorySlice* initialSlice = (MemorySlice*)testBuffer;
  *initialSlice = (MemorySlice) {
    .UsableBytes = _Heap_AlignSize(128),
    .NextSlice = &nextSlice,
    .PreviousSlice = &previousSlice
  };

  previousSlice.NextSlice = initialSlice;
  nextSlice.PreviousSlice = initialSlice;

  // Method to test
  MemorySlice* remaining = _Heap_SplitMemorySlice(initialSlice, 64);

  // Verify results
  mu_check(remaining);
  mu_check(remaining->NextSlice == &nextSlice);
  mu_check(remaining->PreviousSlice == &previousSlice);
  mu_check(previousSlice.NextSlice == remaining);
  mu_check(nextSlice.PreviousSlice == remaining);
  
  mu_check(initialSlice->UsableBytes == _Heap_AlignSize(64));
  mu_check(!initialSlice->NextSlice && !initialSlice->PreviousSlice);
}

MU_TEST_SUITE(Heap_SliceOperations) {
  MU_RUN_TEST(Heap_SplitMemorySlice__Always__SplitsSlice);
}



// Pointers

MU_TEST(Heap_GetSliceHeaderPointer__Always__ReturnsHeaderPointer) {
  U8 testBuffer[128] = { };
  MemorySlice* slicePointer = _Heap_AlignPointer(testBuffer);

  void* dataPointer = _Heap_GetSliceDataStart(slicePointer);
  MemorySlice* headerPointer = _Heap_GetSliceHeaderPointer(dataPointer);

  mu_check(slicePointer = headerPointer);
}

MU_TEST_SUITE(Heap_PointerOperations) {
  MU_RUN_TEST(Heap_GetSliceHeaderPointer__Always__ReturnsHeaderPointer);
}



// Setup methods

MU_TEST(Heap_Initialize__LessThan128Bytes__ReturnsNull) {
  U8 testBuffer[64] = { };
  mu_check(Heap.Initialize(testBuffer, sizeof(testBuffer)) == null);
}

MU_TEST(Heap_Initialize__EnoughSpace__ReturnsPointer) {
  U8 testBuffer[512] = { };

  // Method to test
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));


  // Check results
  mu_check(heap->TotalBytes == sizeof(testBuffer) - ((void*)heap - (void*)testBuffer));

  void* endOfHeader = _Heap_AlignPointer((void*)testBuffer) + sizeof(HeapArea);
  void* firstSliceStart = _Heap_AlignPointer(endOfHeader);
  void* endOfSliceHeader = _Heap_AlignPointer(firstSliceStart + sizeof(MemorySlice));
  
  mu_check(heap->TotalBytesFree == sizeof(testBuffer) - (endOfSliceHeader - (void*)testBuffer));
  mu_check(heap->TotalBytesUsed == 0);
  mu_check(heap->FreeSlicesList);
  mu_check(!heap->UsedSlicesList);
}

MU_TEST_SUITE(Heap_Initialize) {
  MU_RUN_TEST(Heap_Initialize__LessThan128Bytes__ReturnsNull);
  MU_RUN_TEST(Heap_Initialize__EnoughSpace__ReturnsPointer);
}



// Memory allocation

MU_TEST(Heap_Allocate__NotEnoughSpace__ReturnsNull) {
  U8 testBuffer[256] = { };
  
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap!");

  // Method to test
  void* pointer = Heap.Allocate(heap, sizeof(testBuffer) * 2);

  // Verify results
  mu_check(!pointer);
}

MU_TEST(Heap_Allocate__EnoughSpace__ReturnsPointer) {
  U8 testBuffer[256] = { };

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap!");
  const U32 freeBytesBeforeAlloc = heap->TotalBytesFree;
  const U32 usedBytesBeforeAlloc = heap->TotalBytesUsed;
  
  // Method to test
  const U32 size = 13;
  void* pointer = Heap.Allocate(heap, size);

  mu_check(pointer);
  mu_check(heap->TotalBytesFree == freeBytesBeforeAlloc - _Heap_AlignSize(size + sizeof(MemorySlice)));
  mu_check(heap->TotalBytesUsed == usedBytesBeforeAlloc + _Heap_AlignSize(size + sizeof(MemorySlice)));

  mu_check(!heap->FreeSlicesList->NextSlice);
  mu_check(heap->UsedSlicesList);
}

MU_TEST(Heap_Allocate__MultipleAllocs__ReturnsPointers) {
  U8 testBuffer[1024] = { };

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap!");

  char string1[] = "Hello, world!";
  void* pointer1 = Heap.Allocate(heap, sizeof(string1));
  memcpy(pointer1, string1, sizeof(string1));

  char string2[] = "Another string!";
  void* pointer2 = Heap.Allocate(heap, sizeof(string2));
  memcpy(pointer2, string2, sizeof(string2));

  char string3[] = "A third string!";
  void* pointer3 = Heap.Allocate(heap, sizeof(string3));
  memcpy(pointer3, string3, sizeof(string3));

  mu_check(!memcmp(pointer1, string1, sizeof(string1)));
  mu_check(!memcmp(pointer2, string2, sizeof(string2)));
  mu_check(!memcmp(pointer3, string3, sizeof(string3)));
}

MU_TEST_SUITE(Heap_Allocate) {
  MU_RUN_TEST(Heap_Allocate__NotEnoughSpace__ReturnsNull);
  MU_RUN_TEST(Heap_Allocate__EnoughSpace__ReturnsPointer);
  MU_RUN_TEST(Heap_Allocate__MultipleAllocs__ReturnsPointers);
}



// Free

MU_TEST(Heap_Free__SingleAlloc__FreesAllocatedMemory) {
  U8 testBuffer[512] = { };
  
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap!");

  U32 freeBytesBeforeAlloc = heap->TotalBytesFree;
  U32 usedBytesBeforeAlloc = heap->TotalBytesUsed;
  
  // Method to test
  void* pointer = Heap.Allocate(heap, 128);
  mu_assert(pointer, "Unable to allocate memory.");
  
  Heap.Free(heap, pointer);

  mu_check(!heap->UsedSlicesList);
  mu_assert_int_eq(freeBytesBeforeAlloc, heap->TotalBytesFree);
  mu_assert_int_eq(usedBytesBeforeAlloc, heap->TotalBytesUsed);
  mu_check(!heap->FreeSlicesList->NextSlice);
}

MU_TEST(Heap_Free__MultipleAllocs__FreesAllocatedMemory) {
    U8 testBuffer[512] = { };
  
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap!");

  U32 freeBytesBeforeAlloc = heap->TotalBytesFree;
  U32 usedBytesBeforeAlloc = heap->TotalBytesUsed;
  
  // Method to test
  void* pointer1 = Heap.Allocate(heap, 128);
  void* pointer2 = Heap.Allocate(heap, 128);
  void* pointer3 = Heap.Allocate(heap, 128);
  mu_assert(pointer1 && pointer2 && pointer3, "Unable to allocate memory.");

  Heap.Free(heap, pointer1);
  Heap.Free(heap, pointer2);
  Heap.Free(heap, pointer3);

  mu_check(!heap->UsedSlicesList);
  mu_assert_int_eq(freeBytesBeforeAlloc, heap->TotalBytesFree);
  mu_assert_int_eq(usedBytesBeforeAlloc, heap->TotalBytesUsed);
  mu_check(!heap->FreeSlicesList->NextSlice);
}

MU_TEST_SUITE(Heap_Free) {
  MU_RUN_TEST(Heap_Free__SingleAlloc__FreesAllocatedMemory);
  MU_RUN_TEST(Heap_Free__MultipleAllocs__FreesAllocatedMemory);
}



int main(void) {
  MU_RUN_SUITE(Heap_ListOperations);
  MU_RUN_SUITE(Heap_SliceOperations);
  MU_RUN_SUITE(Heap_PointerOperations);
  
  MU_RUN_SUITE(Heap_Initialize);
  MU_RUN_SUITE(Heap_Allocate);
  MU_RUN_SUITE(Heap_Free);
  
  MU_REPORT();

  return MU_EXIT_CODE;
}
