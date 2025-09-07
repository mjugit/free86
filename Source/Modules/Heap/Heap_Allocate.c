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


void* _Heap_AllocateImplementation(HeapMemory heapArea, U32 size) {
  __HeapMemory_Header* header = (__HeapMemory_Header*)heapArea;

  // Fast check
  if (header->TotalBytesFree < size)
    return null;

  __Heap_Lock(header);
  // Traverse list of free slices
  __HeapMemory_Slice* freeSlice = __Heap_FindFreeSlice(header->FreeBlockList, size);
  if (!freeSlice) {
    __Heap_Unlock(header);
    return null;
  }

  
  // Will hold the result
  __HeapMemory_Slice* allocSlice = null;

  if (freeSlice->Size > size) {
    allocSlice = __Heap_SplitHeapSlice(freeSlice, size);
  } else {
    if (freeSlice == header->FreeBlockList && !freeSlice->NextBlock)
      header->FreeBlockList = null;
    
    __Heap_UnhingeSlice(freeSlice);
    allocSlice = freeSlice;
  }


  // Mark block as used
  allocSlice->NextBlock = header->UsedBlockList;
  if (allocSlice->NextBlock)
    allocSlice->NextBlock->PreviousBlock = allocSlice;
  header->UsedBlockList = allocSlice;

  // Update counters
  U32 totalSize = size + sizeof(__HeapMemory_Slice);
  header->TotalBytesFree -= totalSize;
  header->TotalBytesUsed += totalSize;

  __Heap_Unlock(header);
  
  return __Heap_GetHeapSliceDataPointer(allocSlice);
}
