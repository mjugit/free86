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


void _Heap_DefragImplementation(HeapMemory heapArea) {
  __HeapMemory_Header* header = (__HeapMemory_Header*)heapArea;

  if (!header->FreeBlockList)
    return;
  
  __Heap_Lock(header);

  __HeapMemory_Slice* sorted = null;
  __HeapMemory_Slice* current = header->FreeBlockList;

  while (current) {
    __HeapMemory_Slice* next = current->NextBlock;
    current->NextBlock = null;
    current->PreviousBlock = null;
    
    if (!sorted || current < sorted) {
      current->NextBlock = sorted;
      if (sorted)
        sorted->PreviousBlock = current;
      sorted = current;
    } else {
      __HeapMemory_Slice* position = sorted;
      while (position->NextBlock && position->NextBlock < current)
        position = position->NextBlock;

      current->NextBlock = position->NextBlock;
      if (position->NextBlock)
        position->NextBlock->PreviousBlock = current;

      position->NextBlock = current;
      current->PreviousBlock = position;
    }

    current = next;
  }

  header->FreeBlockList = sorted;
  __HeapMemory_Slice* slice = header->FreeBlockList;

  while (slice && slice->NextBlock) {
    void* sliceEnd = __Heap_GetHeapSliceEndPointer(slice);

    if ((void*)slice->NextBlock == sliceEnd) {
      __HeapMemory_Slice* next = slice->NextBlock;
      slice->NextBlock = next->NextBlock;
      if (next->NextBlock)
        next->NextBlock->PreviousBlock = slice;

      slice->Size += sizeof(__HeapMemory_Slice) + next->Size;
      header->TotalBytesFree += sizeof(__HeapMemory_Slice);
    } else {
      slice = slice->NextBlock;
    }
  }

  
  __Heap_Unlock(header);
}
