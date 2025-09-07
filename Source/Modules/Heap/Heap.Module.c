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

HeapMemory*	_Heap_InitializeImplementation(void *pointer, U32 totalSize);
void*		_Heap_AllocateImplementation(HeapMemory *heapArea, U32 size);
void		_Heap_FreeImplementation(HeapMemory *heapArea, void* pointer);
U32		_Heap_GetBytesFreeImplementation(HeapMemory *heapArea);
U32		_Heap_GetBytesUsedImplementation(HeapMemory *heapArea);


members(Heap) {
    .Initialize	  = _Heap_InitializeImplementation,
    .Allocate	  = _Heap_AllocateImplementation,
    .Free	  = _Heap_FreeImplementation,
    .GetBytesFree = _Heap_GetBytesFreeImplementation,
    .GetBytesUsed = _Heap_GetBytesUsedImplementation
};
