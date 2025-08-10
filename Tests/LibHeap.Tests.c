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

#include "../Source/Libs/Include/CoreTypes.h"
#include "../Source/Libs/Include/Bitmap.h"
#include "../Source/Libs/Include/Heap.h"



MU_TEST(Heap_Initialize__ValidInput__SetsFieldsAndClearsBitmap) {
  U8 memory[1024];

  // Method to test
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 16);

  // Check results
  if (heap == 0)
    mu_fail("Initialize returned NULL.");

  // Basic invariants
  if (heap->BaseAddress != memory)
    mu_fail("BaseAddress mismatch.");

  if (heap->TotalBytes != sizeof(memory)) {
    char message[120];
    sprintf(message, "TotalBytes mismatch: got %u.", (unsigned)heap->TotalBytes);
    mu_fail(message);
  }

  if (heap->BlockSize != 16)
    mu_fail("BlockSize mismatch.");

  if (heap->TotalBlocks == 0)
    mu_fail("TotalBlocks must be > 0.");

  if (heap->Bitmap == 0 || heap->BitmapBytes == 0)
    mu_fail("Bitmap not initialized.");

  if (heap->DataStart == 0 || heap->DataBytes == 0)
    mu_fail("DataStart/DataBytes not initialized.");

  // Bitmap must be cleared
  for (U32 bit = 0; bit < heap->TotalBlocks; bit++) {
    int used = Bitmap.Check(heap->Bitmap, bit);
    if (used != 0) {
      char message[120];
      sprintf(message, "Bitmap not cleared at bit %u.", (unsigned)bit);
      mu_fail(message);
    }
  }
}

MU_TEST(Heap_Initialize__ZeroBlockSize__ReturnsNull) {
  U8 memory[256];

  // Method to test
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 0);

  // Check results
  if (heap != 0)
    mu_fail("Expected NULL when block size is 0.");
}

MU_TEST(Heap_Initialize__NotEnoughForOneBlock__ReturnsNull) {
  // Ensure total space is smaller than one block after metadata
  const U16 BS = 16;
  U8 memory[sizeof(HeapArea) + (BS - 1)];

  // Method to test
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), BS);

  // Check results
  if (heap != 0)
    mu_fail("Expected NULL when not enough space for a single block.");
}

MU_TEST_SUITE(Heap_Initialize_Suite) {
  MU_RUN_TEST(Heap_Initialize__ValidInput__SetsFieldsAndClearsBitmap);
  MU_RUN_TEST(Heap_Initialize__ZeroBlockSize__ReturnsNull);
  MU_RUN_TEST(Heap_Initialize__NotEnoughForOneBlock__ReturnsNull);
}



MU_TEST(Heap_Allocate__SingleBlock__ReturnsAlignedPointer) {
  U8 memory[1024];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 32);
  if (heap == 0) mu_fail("Initialize failed.");

  // Method to test
  void* p = Heap.Allocate(heap, 1);

  // Check results
  if (p == 0)
    mu_fail("Allocate returned NULL for available space.");

  uintptr_t off = (uintptr_t)((U8*)p - (U8*)heap->DataStart);
  if (off % heap->BlockSize != 0) {
    char message[120];
    sprintf(message, "Pointer is not block-aligned: offset %lu.", (unsigned long)off);
    mu_fail(message);
  }

  // Exactly one block should be marked used
  U32 startBlock = (U32)(off / heap->BlockSize);
  if (Bitmap.Check(heap->Bitmap, startBlock) != 1)
    mu_fail("Allocated block not marked in bitmap.");
}

MU_TEST(Heap_Allocate__MultipleBlocks__ReturnsContiguousRegion) {
  U8 memory[2048];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 32);
  if (heap == 0) mu_fail("Initialize failed.");

  U32 size = 5 * heap->BlockSize; // needs 5 blocks
  void* p = Heap.Allocate(heap, size);
  if (p == 0) mu_fail("Allocate returned NULL.");

  U32 start = (U32)(((U8*)p - (U8*)heap->DataStart) / heap->BlockSize);
  for (U32 i = 0; i < 5; i++) {
    if (Bitmap.Check(heap->Bitmap, start + i) != 1) {
      char message[140];
      sprintf(message, "Contiguous region not fully marked: bit %u is 0.", (unsigned)(start + i));
      mu_fail(message);
    }
  }
}

MU_TEST(Heap_Allocate__OutOfMemory__ReturnsNull) {
  U8 memory[256];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 64);
  if (heap == 0) mu_fail("Initialize failed.");

  // Fill the heap
  while (1) {
    void* p = Heap.Allocate(heap, 64);
    if (p == 0) break;
  }

  // One more should fail
  void* q = Heap.Allocate(heap, 64);
  if (q != 0)
    mu_fail("Expected NULL when heap is full.");
}

MU_TEST(Heap_Allocate__ExactFitAtEnd__ReturnsLastRegion) {
  U8 memory[1024];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 64);
  if (heap == 0) mu_fail("Initialize failed.");

  // Compute how many full blocks we have
  U32 blocks = heap->TotalBlocks;
  if (blocks < 2) mu_fail("Not enough blocks for this test.");

  // Allocate all but the last block
  for (U32 i = 0; i < blocks - 1; i++) {
    void* p = Heap.Allocate(heap, heap->BlockSize);
    if (p == 0) {
      mu_fail("Unexpected OOM while pre-filling.");
    }
  }

  // Allocate the last block
  void* last = Heap.Allocate(heap, heap->BlockSize);
  if (last == 0)
    mu_fail("Expected last exact-fit allocation to succeed.");

  U32 start = (U32)(((U8*)last - (U8*)heap->DataStart) / heap->BlockSize);
  if (start != blocks - 1) {
    char message[140];
    sprintf(message, "Expected last block index %u, got %u.", (unsigned)(blocks - 1), (unsigned)start);
    mu_fail(message);
  }
}

MU_TEST(Heap_Allocate__ZeroSize__ReturnsNull) {
  U8 memory[512];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 32);
  if (!heap) mu_fail("Initialize failed.");

  // Method to test
  void* p = Heap.Allocate(heap, 0);

  // Check results
  if (p != 0)
    mu_fail("Expected NULL when requested size is 0.");
}

MU_TEST(Heap_Allocate__LargerThanHeap__ReturnsNull) {
  U8 memory[512];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 64);
  if (!heap) mu_fail("Initialize failed.");

  // Request absurdly large size
  void* p = Heap.Allocate(heap, 1024 * 1024);

  if (p != 0)
    mu_fail("Expected NULL when requested size exceeds heap capacity.");
}

MU_TEST(Heap_Allocate__BlockSizeOne__AllocatesAndMarksBits) {
  U8 memory[128];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 1);
  if (!heap) mu_fail("Initialize failed.");

  // Allocate 5 bytes → 5 bits
  void* p = Heap.Allocate(heap, 5);
  if (!p) mu_fail("Allocate failed with block size = 1.");

  U32 start = (U32)((U8*)p - (U8*)heap->DataStart); // blockSize=1 -> index == offset
  for (U32 i = 0; i < 5; i++)
    if (Bitmap.Check(heap->Bitmap, start + i) != 1) {
      char message[120];
      sprintf(message, "Bit %u not set for blockSize=1 case.", (unsigned)(start + i));
      mu_fail(message);
    }
}

MU_TEST_SUITE(Heap_Allocate_Suite) {
  MU_RUN_TEST(Heap_Allocate__SingleBlock__ReturnsAlignedPointer);
  MU_RUN_TEST(Heap_Allocate__MultipleBlocks__ReturnsContiguousRegion);
  MU_RUN_TEST(Heap_Allocate__OutOfMemory__ReturnsNull);
  MU_RUN_TEST(Heap_Allocate__ExactFitAtEnd__ReturnsLastRegion);
  MU_RUN_TEST(Heap_Allocate__ZeroSize__ReturnsNull);
  MU_RUN_TEST(Heap_Allocate__LargerThanHeap__ReturnsNull);
  MU_RUN_TEST(Heap_Allocate__BlockSizeOne__AllocatesAndMarksBits);
}



MU_TEST(Heap_Free__PreviouslyAllocated__MakesSpaceAvailableAgain) {
  U8 memory[1024];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 32);
  if (heap == 0) mu_fail("Initialize failed.");

  void* p = Heap.Allocate(heap, 100); // needs 4 blocks
  if (p == 0) mu_fail("Allocate failed.");

  U32 start = (U32)(((U8*)p - (U8*)heap->DataStart) / heap->BlockSize);
  U32 blocks = (100 + heap->BlockSize - 1) / heap->BlockSize;

  // Verify marked
  for (U32 i = 0; i < blocks; i++) {
    if (Bitmap.Check(heap->Bitmap, start + i) != 1) {
      mu_fail("Allocated bits not set before free.");
    }
  }

  // Method to test
  Heap.Free(heap, p, 100);

  // Check results: bits cleared
  for (U32 i = 0; i < blocks; i++) {
    if (Bitmap.Check(heap->Bitmap, start + i) != 0) {
      char message[140];
      sprintf(message, "Bit %u not cleared after free.", (unsigned)(start + i));
      mu_fail(message);
    }
  }

  // Should be allocatable again
  void* q = Heap.Allocate(heap, 100);
  if (q == 0)
    mu_fail("Region not reusable after free.");
}

MU_TEST(Heap_Free__UnalignedSize__FreesRoundedBlockCount) {
  U8 memory[1024];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 64);
  if (heap == 0) mu_fail("Initialize failed.");

  void* p = Heap.Allocate(heap, 65); // 2 blocks
  if (p == 0) mu_fail("Allocate failed.");

  U32 start = (U32)(((U8*)p - (U8*)heap->DataStart) / heap->BlockSize);

  // Method to test (free with unaligned size that still rounds to 2 blocks)
  Heap.Free(heap, p, 65);

  if (Bitmap.Check(heap->Bitmap, start) != 0 || Bitmap.Check(heap->Bitmap, start + 1) != 0) {
    mu_fail("Rounded blocks were not fully cleared on free.");
  }
}

MU_TEST(Heap_Free__PointerOutsideHeap__DoesNotTouchBitmap) {
  U8 memory[512];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 32);
  if (!heap) mu_fail("Initialize failed.");

  // Pre-allocate one block
  void* p = Heap.Allocate(heap, 32);
  if (!p) mu_fail("Pre-allocate failed.");

  // Snapshot bitmap for comparison
  U32 bits = heap->TotalBlocks;
  // We'll just count how many bits are set before and after
  U32 beforeSet = 0;
  for (U32 i = 0; i < bits; i++) beforeSet += (Bitmap.Check(heap->Bitmap, i) ? 1 : 0);

  // Method to test: pointer far outside data region
  int dummy;
  Heap.Free(heap, &dummy, 32);

  // Verify bitmap unchanged
  U32 afterSet = 0;
  for (U32 i = 0; i < bits; i++) afterSet += (Bitmap.Check(heap->Bitmap, i) ? 1 : 0);

  if (beforeSet != afterSet) {
    char message[120];
    sprintf(message, "Bitmap changed when freeing pointer outside heap (before=%u after=%u).",
            (unsigned)beforeSet, (unsigned)afterSet);
    mu_fail(message);
  }
}

MU_TEST(Heap_Free__NullPointer__DoesNothing) {
  U8 memory[512];
  HeapArea* heap = Heap.Initialize(memory, sizeof(memory), 32);
  if (!heap) mu_fail("Initialize failed.");

  // Pre-allocate two blocks
  void* p = Heap.Allocate(heap, 64);
  if (!p) mu_fail("Allocate failed.");

  // Snapshot set bit count
  U32 bits = heap->TotalBlocks;
  U32 beforeSet = 0;
  for (U32 i = 0; i < bits; i++) beforeSet += (Bitmap.Check(heap->Bitmap, i) ? 1 : 0);

  // Method to test
  Heap.Free(heap, 0, 64);

  // Verify unchanged
  U32 afterSet = 0;
  for (U32 i = 0; i < bits; i++) afterSet += (Bitmap.Check(heap->Bitmap, i) ? 1 : 0);

  if (beforeSet != afterSet)
    mu_fail("Bitmap changed when freeing NULL pointer.");
}

MU_TEST_SUITE(Heap_Free_Suite) {
  MU_RUN_TEST(Heap_Free__PreviouslyAllocated__MakesSpaceAvailableAgain);
  MU_RUN_TEST(Heap_Free__UnalignedSize__FreesRoundedBlockCount);
  MU_RUN_TEST(Heap_Free__PointerOutsideHeap__DoesNotTouchBitmap);
  MU_RUN_TEST(Heap_Free__NullPointer__DoesNothing);
}



int main(void) {
  MU_RUN_SUITE(Heap_Initialize_Suite);
  MU_RUN_SUITE(Heap_Allocate_Suite);
  MU_RUN_SUITE(Heap_Free_Suite);

  MU_REPORT();
  return MU_EXIT_CODE;
}
