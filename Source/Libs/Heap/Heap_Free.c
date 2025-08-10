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

// Frees a previously allocated memory region in the heap.
// Clears the corresponding bits in the allocation bitmap.

__attribute__((unused))
void Heap_Free(HeapArea* heap, void* ptr, U32 numBytes) {
    if (!heap || !ptr || numBytes == 0)
        return; // Invalid parameters

    // Calculate start block index
    U32 offset = (U8*)ptr - (U8*)heap->DataStart;
    if (offset >= heap->DataBytes)
        return; // Pointer outside of heap range

    U32 startBlock = offset / heap->BlockSize;

    // Calculate number of blocks to free
    U32 blocksToFree = numBytes / heap->BlockSize;
    if (numBytes % heap->BlockSize != 0)
        blocksToFree++;

    // Clear bits in the bitmap
    for (U32 i = 0; i < blocksToFree && (startBlock + i) < heap->TotalBlocks; i++)
        Bitmap.Clear(heap->Bitmap, startBlock + i);
}

