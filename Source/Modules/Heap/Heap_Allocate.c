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

static MemorySlice* _TryGetFreeSlice(HeapArea* heap, U32 minSize);


void* _Heap_AllocateImplementation(HeapArea* heap, U32 size) {
  if (size > heap->TotalBytesFree)
    return null;

  U32 minSize = _Heap_AlignSize(size);
  MemorySlice* freeSlice = _TryGetFreeSlice(heap, minSize);
  if (!freeSlice)
    return null;

  if (heap->FreeSlicesList == freeSlice)
    heap->FreeSlicesList = _Heap_SplitMemorySlice(freeSlice, minSize);
  else
    _Heap_SplitMemorySlice(freeSlice, minSize);
  heap->TotalBytesFree -= _Heap_AlignSize(minSize + sizeof(MemorySlice));
  
  if (!heap->UsedSlicesList)
    heap->UsedSlicesList = freeSlice;
  else
    _Heap_InsertListItemBefore(heap->UsedSlicesList, freeSlice);
  heap->TotalBytesUsed += _Heap_AlignSize(minSize + sizeof(MemorySlice));
  
  return _Heap_GetSliceDataStart(freeSlice);
}


static MemorySlice* _TryGetFreeSlice(HeapArea* heap, U32 minSize) {
  U32 requiredSize = _Heap_AlignSize(minSize + sizeof(MemorySlice));
  
  for (MemorySlice* slice = heap->FreeSlicesList; slice; slice = slice->NextSlice)
    if (slice->UsableBytes >= requiredSize)
      return slice;

  return null;
}
