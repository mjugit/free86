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


// Heap setup

MU_TEST(Heap_Initialize__Always__InitializesHeapArea) {
  U8 testBuffer[500] = { };

  // Method to test
  HeapMemory* heapPtr = Heap.Initialize(testBuffer, sizeof(testBuffer));

  // Check results
  __HeapMemory_Header* header = (__HeapMemory_Header*)heapPtr;

  mu_check(header->MemoryAreaSize == sizeof(testBuffer));
  mu_check(header->HeapHeaderSize == sizeof(__HeapMemory_Header));
  mu_check(header->HeapDataStart == ((void*)testBuffer + sizeof(__HeapMemory_Header)));
  mu_check(header->UsedBlockList == null);
  mu_check(header->TotalBytesUsed == 0);
  
  mu_check(header->FreeBlockList->NextBlock == null);
  mu_check(header->FreeBlockList->PreviousBlock == null);
  mu_check(header->FreeBlockList->Size == (sizeof(testBuffer) - sizeof(__HeapMemory_Header)));

  mu_check(header->TotalBytesFree == header->FreeBlockList->Size);
}

MU_TEST_SUITE(Heap_Initialize) {
  MU_RUN_TEST(Heap_Initialize__Always__InitializesHeapArea);
}



// Memory allocation

MU_TEST(Heap_Allocate__NotEnoughSpace__ReturnsNull) {
  U8 testBuffer[100] = { };

  // Setup
  __HeapMemory_Header* header = (__HeapMemory_Header*)testBuffer;
  header->TotalBytesFree = 10;

  HeapMemory* heapPtr = (HeapMemory*)testBuffer;

  // Method to test
  void* actual = Heap.Allocate(heapPtr, header->TotalBytesFree + 100);

  // Check results
  mu_check(actual == null);
}

MU_TEST(Heap_Allocate__NoLargeEnoughSlice__ReturnsNull) {
  U8 testBuffer[100] = { };

  // Setup
  HeapMemory *heapPtr = Heap.Initialize(testBuffer, sizeof(testBuffer));
  __HeapMemory_Header* header = (__HeapMemory_Header*)heapPtr;
  // Bypass bytes free check
  header->TotalBytesFree = 1000;

  // Method to test
  void* actual = Heap.Allocate(heapPtr, 300);

  // Check results
  mu_check(actual == null);
}

MU_TEST(Heap_Allocate__EnoughSpace__ReturnsPointer) {
  U8 testBuffer[300] = { };

  // Setup
  HeapMemory *heapPtr = Heap.Initialize(testBuffer, sizeof(testBuffer));
  __HeapMemory_Header* header = (__HeapMemory_Header*)heapPtr;
  const U32 freeSizeBeforeAlloc = header->FreeBlockList->Size;
  
  // Method to test
  const U32 allocSize = 30;
  void* actual = Heap.Allocate(heapPtr, allocSize);

  // Check results
  mu_check(actual);
  mu_check(header->UsedBlockList == (void*)actual - sizeof(__HeapMemory_Slice));
  mu_check(header->UsedBlockList->Size == allocSize);
  mu_check(header->FreeBlockList->Size == freeSizeBeforeAlloc - sizeof(__HeapMemory_Slice) - allocSize);
  mu_check(header->TotalBytesFree == freeSizeBeforeAlloc - allocSize - sizeof(__HeapMemory_Slice));
  mu_check(header->TotalBytesUsed == allocSize + sizeof(__HeapMemory_Slice));
}

MU_TEST(Heap_Allocate__MultipleAllocs__ReturnsPointers) {
    U8 testBuffer[500] = { };

  // Setup
  HeapMemory *heapPtr = Heap.Initialize(testBuffer, sizeof(testBuffer));
  __HeapMemory_Header* header = (__HeapMemory_Header*)heapPtr;
  const U32 freeSizeBeforeAlloc = header->FreeBlockList->Size;
  
  // Method to test
  const U32 firstAllocSize = 35;
  void *firstAlloc = Heap.Allocate(heapPtr, firstAllocSize);
  const U32 secondAllocSize = 47;
  void *secondAlloc = Heap.Allocate(heapPtr, secondAllocSize);
  const U32 thirdAllocSize = 51;
  void *thirdAlloc = Heap.Allocate(heapPtr, thirdAllocSize);
  const U32 totalAllocSize = firstAllocSize + secondAllocSize + thirdAllocSize
    + (3 * sizeof(__HeapMemory_Slice));

  mu_assert(firstAlloc && secondAlloc && thirdAlloc, "At least one alloc failed.");
  mu_check(header->TotalBytesUsed == totalAllocSize);
  mu_check(header->TotalBytesFree == freeSizeBeforeAlloc - totalAllocSize);
  mu_check(header->FreeBlockList);

  mu_check(header->UsedBlockList == __Heap_GetHeapSliceStartPointer(thirdAlloc));
  __HeapMemory_Slice* secondUsedBlock = header->UsedBlockList->NextBlock;
  mu_check(secondUsedBlock == __Heap_GetHeapSliceStartPointer(secondAlloc));
  __HeapMemory_Slice* thirdUsedBlock = secondUsedBlock->NextBlock;
  mu_check(thirdUsedBlock == __Heap_GetHeapSliceStartPointer(firstAlloc));
}

MU_TEST_SUITE(Heap_Allocate) {
  MU_RUN_TEST(Heap_Allocate__NotEnoughSpace__ReturnsNull);
  MU_RUN_TEST(Heap_Allocate__NoLargeEnoughSlice__ReturnsNull);
  MU_RUN_TEST(Heap_Allocate__EnoughSpace__ReturnsPointer);
  MU_RUN_TEST(Heap_Allocate__MultipleAllocs__ReturnsPointers);
}



// Free allocs

MU_TEST(Heap_Free__Always_FreesAllocatedMemory) {
  U8 testBuffer[300] = { };

  // Setup
  HeapMemory* heapPtr = Heap.Initialize((void*)testBuffer, 100);
  __HeapMemory_Header* header = (__HeapMemory_Header*)heapPtr;
  const U32 freeSizeBeforeAlloc = header->TotalBytesFree;
  void* alloc = Heap.Allocate(heapPtr, 30);
  mu_assert(alloc, "Alloc failed.\n");

  // Method to test
  Heap.Free(heapPtr, alloc);

  // Check results
  mu_check(header->TotalBytesFree == freeSizeBeforeAlloc - sizeof(__HeapMemory_Slice));
  mu_check(header->TotalBytesUsed == 0);
  mu_check(header->UsedBlockList == null);
  mu_check(header->FreeBlockList->NextBlock);
}

MU_TEST_SUITE(Heap_Free) {
  MU_RUN_TEST(Heap_Free__Always_FreesAllocatedMemory);
}



// Helper methods

MU_TEST(_Heap_SplitHeapSlice__Always_SplitsSlice) {
  U8 testBuffer[300] = { };

  // Setup
  const U32 initialSize = sizeof(testBuffer) - sizeof(__HeapMemory_Slice);
  __HeapMemory_Slice* initialSlice = (__HeapMemory_Slice*)testBuffer;
  *initialSlice = (__HeapMemory_Slice) {
    .NextBlock = null,
    .PreviousBlock = null,
    .Size = initialSize
  };

  // Method to test
  const U32 newSliceSize = 123;
  __HeapMemory_Slice* newSlice = __Heap_SplitHeapSlice(initialSlice, newSliceSize);

  // Check results
  mu_check(initialSlice->Size == (initialSize - sizeof(__HeapMemory_Slice) - newSliceSize));
  mu_check(newSlice->Size == newSliceSize);
  mu_check(newSlice == ((void*)testBuffer +
			(sizeof(testBuffer) - sizeof(__HeapMemory_Slice) - newSliceSize)));
}

MU_TEST(_Heap_GetHeapSliceDataPointer__Always__ReturnsPointer) {
  U8 testBuffer[100] = { };

  // Setup
  const U32 initialSize = sizeof(testBuffer) - sizeof(__HeapMemory_Slice);
  __HeapMemory_Slice* slice = (__HeapMemory_Slice*)testBuffer;
  *slice = (__HeapMemory_Slice) {
    .NextBlock = null,
    .PreviousBlock = null,
    .Size = initialSize
  };

  // Method to test
  void* dataPtr = __Heap_GetHeapSliceDataPointer(slice);

  // Check results
  mu_check(dataPtr == (void*)slice + sizeof(__HeapMemory_Slice));
}

MU_TEST(_Heap_GetHeapSliceStartPointer__Always__ReturnsPointer) {
  U8 testBuffer[100] = { };

  // Setup
  const U32 size = sizeof(testBuffer) - sizeof(__HeapMemory_Slice);
  __HeapMemory_Slice* slice = (__HeapMemory_Slice*)testBuffer;
  *slice = (__HeapMemory_Slice) {
    .NextBlock = null,
    .PreviousBlock = null,
    .Size = size
  };

  // Method to test
  void* startPtr = __Heap_GetHeapSliceStartPointer((void*)slice + sizeof(__HeapMemory_Slice));

  // Check results
  mu_check(startPtr == (void*)slice);
}

MU_TEST(_Heap_GetHeapSliceEndPointer__Always__ReturnsPointer) {
  U8 testBuffer[100] = { };

  // Setup
  const U32 size = sizeof(testBuffer) - sizeof(__HeapMemory_Slice);
  __HeapMemory_Slice* slice = (__HeapMemory_Slice*)testBuffer;
  *slice = (__HeapMemory_Slice) {
    .NextBlock = null,
    .PreviousBlock = null,
    .Size = size
  };

  // Method to test
  void* endPtr = __Heap_GetHeapSliceEndPointer(slice);

  // Check results
  mu_check(endPtr == (void*)slice + sizeof(__HeapMemory_Slice) + size);
}

MU_TEST(_Heap_FindFreeSlice__NoSliceAvailable__ReturnsNull) {
  const U32 numSlices = 10;
  __HeapMemory_Slice slices[numSlices];

  // Setup
  __HeapMemory_Slice* lastSlice = null;
  for (U32 sliceIndex = 0; sliceIndex < numSlices; sliceIndex++) {
    slices[sliceIndex] = (__HeapMemory_Slice) {
      .PreviousBlock = lastSlice,
      .NextBlock = &slices[sliceIndex + 1],
      .Size = 0
    };
  }

  slices[numSlices - 1].NextBlock = null;
  
  // Method to test
  __HeapMemory_Slice* result = __Heap_FindFreeSlice(slices, 100);

  mu_check(result == null);
}

MU_TEST(_Heap_FindFreeSlice__SliceAvailable__ReturnsPointer) {
  const U32 numSlices = 10;
  __HeapMemory_Slice slices[numSlices];

  // Setup
  __HeapMemory_Slice* lastSlice = null;
  for (U32 sliceIndex = 0; sliceIndex < numSlices; sliceIndex++) {
    slices[sliceIndex] = (__HeapMemory_Slice) {
      .PreviousBlock = lastSlice,
      .NextBlock = &slices[sliceIndex + 1],
      .Size = 0
    };
  }

  slices[numSlices - 1].NextBlock = null;
  slices[numSlices - 1].Size = 300;

  // Method to test
  __HeapMemory_Slice* result = __Heap_FindFreeSlice(slices, 100);

  mu_check(result == &slices[numSlices - 1]);
}

MU_TEST(_Heap_UnhingeSlice__NoOtherSlices__ResetsPointers) {
  __HeapMemory_Slice slice = {
    .NextBlock = null,
    .PreviousBlock = null,
    .Size = 0
  };

  // Method to test
  __Heap_UnhingeSlice(&slice);

  // Should cause a access violation on failure

  // Check results
  mu_check(slice.NextBlock == null);
  mu_check(slice.PreviousBlock == null);
}

MU_TEST(_Heap_UnhingeSlice__WithOtherSlices__ReconnectsSlices) {
  __HeapMemory_Slice previous = {
    .PreviousBlock = null,
    .Size = 0
  };

  __HeapMemory_Slice next = {
    .NextBlock = null,
    .Size = 0
  };
  
  __HeapMemory_Slice slice = {
    .NextBlock = &next,
    .PreviousBlock = &previous,
    .Size = 0
  };

  next.PreviousBlock = &slice;
  previous.NextBlock = &slice;
  
  // Method to test
  __Heap_UnhingeSlice(&slice);

  // Check results
  mu_check(slice.NextBlock == null);
  mu_check(slice.PreviousBlock == null);  
  mu_check(previous.NextBlock == &next);
  mu_check(next.PreviousBlock == &previous);
}

MU_TEST_SUITE(_Heap_Helpers) {
  MU_RUN_TEST(_Heap_SplitHeapSlice__Always_SplitsSlice);
  MU_RUN_TEST(_Heap_GetHeapSliceDataPointer__Always__ReturnsPointer);
  MU_RUN_TEST(_Heap_GetHeapSliceStartPointer__Always__ReturnsPointer);
  MU_RUN_TEST(_Heap_GetHeapSliceEndPointer__Always__ReturnsPointer);
  MU_RUN_TEST(_Heap_FindFreeSlice__NoSliceAvailable__ReturnsNull);
  MU_RUN_TEST(_Heap_FindFreeSlice__SliceAvailable__ReturnsPointer);
  MU_RUN_TEST(_Heap_UnhingeSlice__NoOtherSlices__ResetsPointers);
  MU_RUN_TEST(_Heap_UnhingeSlice__WithOtherSlices__ReconnectsSlices);
}



int main(void) {
  MU_RUN_SUITE(Heap_Initialize);
  MU_RUN_SUITE(Heap_Allocate);
  MU_RUN_SUITE(Heap_Free);

  MU_RUN_SUITE(_Heap_Helpers);
  
  MU_REPORT();

  return MU_EXIT_CODE;
}
