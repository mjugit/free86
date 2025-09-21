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


void _Heap_Free_Implementation(HeapArea* heap, void* pointer) {
  MemorySlice* slice = _Heap_GetSliceHeaderPointer(pointer);

  if (heap->UsedSlicesList == slice)
    heap->UsedSlicesList = slice->NextSlice;
  
  _Heap_UnhingeListItem(slice);
  
  // Find correct insert position in FreeSlicesList
  MemorySlice* current = heap->FreeSlicesList;
  MemorySlice* prev = null;
  while (current && current < slice) {
    prev = current;
    current = current->NextSlice;
  }

  // Insert the slice in FreeSlicesList
  slice->PreviousSlice = prev;
  slice->NextSlice = current;
  if (prev)
    prev->NextSlice = slice;
  else
    heap->FreeSlicesList = slice;
  
  if (current)
    current->PreviousSlice = slice;

  // Try to merge with previous
  if (prev && ((U8*)_Heap_GetSliceDataEnd(prev) == (U8*)slice)) {
    prev->UsableBytes += ((U8*)_Heap_GetSliceDataEnd(slice) - (U8*)slice);
    prev->NextSlice = slice->NextSlice;
    
    if (slice->NextSlice)
      slice->NextSlice->PreviousSlice = prev;
    slice = prev;
  }
  
  // Try to merge with next
  if (slice->NextSlice && ((U8*)_Heap_GetSliceDataEnd(slice) == (U8*)slice->NextSlice)) {
    MemorySlice* next = slice->NextSlice;
    slice->UsableBytes += ((U8*)_Heap_GetSliceDataEnd(next) - (U8*)next);
    slice->NextSlice = next->NextSlice;
    
    if (next->NextSlice)
      next->NextSlice->PreviousSlice = slice;
  }
}

