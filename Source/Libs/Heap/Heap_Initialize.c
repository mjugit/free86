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

#include "../Include/Heap.h"

// Initializes a heap in a given memory location.
// Sets up metadata, bitmap, and calculates usable blocks for allocations.

__attribute__((unused))
HeapArea* Heap_Initialize(void *location, U32 numBytes, U16 blockSize) {
    // Cast base address for pointer arithmetic
    U8 *base = (U8*)location;

    // Place the HeapArea structure at the start of the memory
    HeapArea *heap = (HeapArea*)base;
    heap->BaseAddress = location;
    heap->TotalBytes = numBytes;

    // Align blockSize to at least 1 byte
    if (blockSize == 0)
        return 0; // Invalid
    
    heap->BlockSize = blockSize;

    // Usable bytes (exclude HeapArea struct itself)
    heap->UsableBytes = numBytes - sizeof(HeapArea);
    if (heap->UsableBytes <= 0)
        return 0; // No usable space
    
    // Calculate total blocks
    heap->TotalBlocks = heap->UsableBytes / blockSize;
    if (heap->TotalBlocks == 0)
        return 0; // Not enough memory for even one block

    // Bitmap size in bytes
    heap->BitmapBytes = (heap->TotalBlocks + 7) / 8;

    // Bitmap starts right after the HeapArea struct
    heap->Bitmap = base + sizeof(HeapArea);

    // Data starts after bitmap
    heap->DataStart = heap->Bitmap + heap->BitmapBytes;

    // DataBytes = remaining memory after bitmap
    heap->DataBytes = numBytes - (sizeof(HeapArea) + heap->BitmapBytes);

    Bitmap.Wipe(heap->Bitmap, heap->TotalBlocks);

    return heap;
}

