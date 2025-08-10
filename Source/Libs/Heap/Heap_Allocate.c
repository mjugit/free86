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

// Allocates a memory region from the heap.
// Searches for the required number of contiguous free blocks and marks them as used.

__attribute__((unused))
void* Heap_Allocate(HeapArea *heap, U32 size) {
    if (!heap || size == 0)
        return 0; // Invalid parameters

    // Calculate number of blocks needed
    U32 blocksNeeded = size / heap->BlockSize;
    if (size % heap->BlockSize != 0)
        blocksNeeded++;

    // Search for first run of free blocks
    int startBlock = Bitmap.NextFree(heap->Bitmap, blocksNeeded, heap->TotalBlocks);
    if (startBlock < 0)
        return 0; // No space available

    // Mark blocks as used
    for (U32 i = 0; i < blocksNeeded; i++)
        Bitmap.Set(heap->Bitmap, startBlock + i);

    U8 *ptr = (U8*)heap->DataStart + (startBlock * heap->BlockSize);
    
    return (void*)ptr;
}

