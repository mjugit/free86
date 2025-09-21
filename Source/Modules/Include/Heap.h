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


#ifndef __HEAP_MEMORY__
#define __HEAP_MEMORY__

#include "SystemCore.h"




// This is the header of a continuous memory area
typedef struct MemorySlice {
  // The amount of usable bytes
  U32 UsableBytes;

  // Pointer to the next slice in the list
  struct MemorySlice *NextSlice;
  // Pointer to the previous slice in the list
  struct MemorySlice *PreviousSlice;
} MemorySlice;



// This header represents the memory area itself
typedef struct HeapArea {
  // Holds the total size in bytes (including the header)
  U32 TotalBytes;
  // Holds the amount of bytes that can be allocated (without their headers)
  U32 TotalBytesFree;
  // Holds the amount of bytes that have been allocated
  U32 TotalBytesUsed;

  // The head of the list of free slices
  struct MemorySlice *FreeSlicesList;
  // The head of the list of allocated slices
  struct MemorySlice *UsedSlicesList;
} HeapArea;



#define _HEAP_PTR_ALIGNMENT 8

__attribute__((unused))
static inline void* _Heap_AlignPointer(void* pointer) {
    U32 address = (U32)pointer;
    U32 aligned = (address + (_HEAP_PTR_ALIGNMENT - 1)) & ~(_HEAP_PTR_ALIGNMENT - 1);
    
    return (void*)aligned;
}

__attribute__((unused))
static inline U32 _Heap_AlignSize(U32 size) {
    U32 aligned = (size + (_HEAP_PTR_ALIGNMENT - 1)) & ~(_HEAP_PTR_ALIGNMENT - 1);
    
    return aligned < _HEAP_PTR_ALIGNMENT
      ? _HEAP_PTR_ALIGNMENT
      : aligned;
}

__attribute__((unused))
static inline void* _Heap_GetSliceDataStart(MemorySlice* slice) {
  return _Heap_AlignPointer((void*)slice + sizeof(MemorySlice));
}

__attribute__((unused))
static inline void* _Heap_GetSliceDataEnd(MemorySlice* slice) {
  return _Heap_AlignPointer(_Heap_GetSliceDataStart(slice) + slice->UsableBytes);
}


__attribute__((unused))
static inline void _Heap_UnhingeListItem(MemorySlice* item) {
  if (item->PreviousSlice)
    item->PreviousSlice->NextSlice = item->NextSlice;
  if (item->NextSlice)
    item->NextSlice->PreviousSlice = item->PreviousSlice;

  item->NextSlice = item->PreviousSlice = null;
}


__attribute__((unused))
static inline void _Heap_InsertListItemBefore(MemorySlice* next, MemorySlice* item) {
  if (next->PreviousSlice)
    next->PreviousSlice->NextSlice = item;
  
  item->PreviousSlice = next->PreviousSlice;
  item->NextSlice = next;
  next->PreviousSlice = item;
}


__attribute__((unused))
static inline void _Heap_InsertListItemAfter(MemorySlice* previous, MemorySlice* item) {
  if (previous->NextSlice)
    previous->NextSlice->PreviousSlice = item;

  item->NextSlice = previous->NextSlice;
  item->PreviousSlice = previous;
  previous->NextSlice = item;
}


__attribute__((unused))
static inline MemorySlice* _Heap_SplitMemorySlice(MemorySlice* slice, U32 size) {
  void* currentSliceEnd = _Heap_GetSliceDataEnd(slice);
  
  MemorySlice* newSlice = slice;
  void* newSliceDataStart = _Heap_GetSliceDataStart(newSlice);

  MemorySlice* remainingSlice = _Heap_AlignPointer(newSliceDataStart + size);
  void* remainingSliceDataStart = _Heap_GetSliceDataStart(remainingSlice);

  *remainingSlice = (MemorySlice) {
    .UsableBytes = (void*)currentSliceEnd - (void*)remainingSliceDataStart,
    .NextSlice = slice->NextSlice,
    .PreviousSlice = slice->PreviousSlice,
  };

  if (slice->NextSlice)
    slice->NextSlice->PreviousSlice = remainingSlice;
  if (slice->PreviousSlice)
    slice->PreviousSlice->NextSlice = remainingSlice;

  *newSlice = (MemorySlice) {
    .UsableBytes = (void*)remainingSlice - (void*)newSliceDataStart,
    .NextSlice = null,
    .PreviousSlice = null
  };

  return remainingSlice;
}


__attribute__((unused))
static inline MemorySlice* _Heap_GetSliceHeaderPointer(void* pointer) {
  const U32 alignedHeaderSize = _Heap_AlignSize(sizeof(MemorySlice));

  return pointer - alignedHeaderSize;
}




module(Heap) {
  // Initialize a new dynamic memory area
  HeapArea* (*Initialize)(void *startAddress, U32 size);

  // Allocate dynamic memory
  void* (*Allocate)(HeapArea* heap, U32 size);

  // Free dynamic memory
  void (*Free)(HeapArea* heap, void* pointer);
};

#endif
