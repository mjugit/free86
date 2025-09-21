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


HeapArea* _Heap_InitializeImplementation(void *startAddress, U32 size) {
  if (size < 128)
    return null;

  void* endAddress = startAddress + size;
  HeapArea* heap = _Heap_AlignPointer(startAddress);

  MemorySlice* initialSlice = _Heap_AlignPointer((void*)heap + sizeof(HeapArea));
  *initialSlice = (MemorySlice) {
    .UsableBytes = endAddress - _Heap_AlignPointer((void*)initialSlice + sizeof(MemorySlice)),
    .NextSlice = null,
    .PreviousSlice = null
  };

  *heap = (HeapArea) {
    .TotalBytes = size - ((void*)heap - startAddress),
    .TotalBytesFree = initialSlice->UsableBytes,
    .TotalBytesUsed = 0,
    .FreeSlicesList = initialSlice,
    .UsedSlicesList = null
  };
  
  return heap;
}
