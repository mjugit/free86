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


#ifndef __HEAP_H__
#define __HEAP_H__

#include "SystemCore.h"


// A pointer to a dynamic memory area
typedef void* HeapMemory;


// Holds the meta information about a continuous block of memory
typedef struct __HeapMemory_Slice {
  struct __HeapMemory_Slice* NextBlock;
  struct __HeapMemory_Slice* PreviousBlock;
  
  U32 Size;
} __HeapMemory_Slice;


// Holds the meta information about a dynamic memory area
typedef struct __HeapMemory_Header {
  U32 MemoryAreaSize;
  U32 TotalBytesFree;
  U32 TotalBytesUsed;

  bool IsLocked;
  U8 HeapHeaderSize;

  void* HeapDataStart;
  __HeapMemory_Slice* FreeBlockList;
  __HeapMemory_Slice* UsedBlockList;
} __HeapMemory_Header;



/*
 * REMARK:
 * Most of the methods below are only used once or twice and don't realy do much.
 * They enhance the readability of the code drastically though, so they are marked
 * as inline. This also improves the overall performance.
 */


// Get a pointer to the data area of a memory block
__attribute__((unused))
static inline void* __Heap_GetHeapSliceDataPointer(__HeapMemory_Slice* slice) {
  return (void*)slice + sizeof(__HeapMemory_Slice);
}


// Get a pointer to the start of a memory slice based on the data pointer
__attribute__((unused))
static inline __HeapMemory_Slice* __Heap_GetHeapSliceStartPointer(void* dataPointer) {
  return dataPointer - sizeof(__HeapMemory_Slice);
}


// Get a pointer to the end of a memory slice based on the start pointer
__attribute__((unused))
static inline void* __Heap_GetHeapSliceEndPointer(__HeapMemory_Slice* slice) {
  return (void*)slice + sizeof(__HeapMemory_Slice) + slice->Size;
}


// Split a slice of continuous memory
__attribute__((unused))
static inline __HeapMemory_Slice* __Heap_SplitHeapSlice(__HeapMemory_Slice* slice, U32 size) {
  U32 newSliceSize = size + sizeof(__HeapMemory_Slice);
  void *newSlicePtr = __Heap_GetHeapSliceEndPointer(slice) - newSliceSize;

  // Update original slice
  slice->Size -= newSliceSize;

  // Configure new slice
  __HeapMemory_Slice* newSlice = (__HeapMemory_Slice*)newSlicePtr;
  *newSlice = (__HeapMemory_Slice) {
    .NextBlock = null,
    .PreviousBlock = null,
    .Size = size
  };

  return newSlice;
}


// Find the first slice of a minimum size in a list of slices
__attribute__((unused))
static inline __HeapMemory_Slice* __Heap_FindFreeSlice(__HeapMemory_Slice* list, U32 minSize) {
  if (!list)
    return null;

  for (__HeapMemory_Slice* candidate = list; candidate; candidate = candidate->NextBlock)
    if (candidate->Size >= minSize)
      return candidate;

  return null;
}


// Remove the slice from the list and reconfigure the other pointers
__attribute__((unused))
static inline void __Heap_UnhingeSlice(__HeapMemory_Slice* slice) {
  if (slice->NextBlock)
    slice->NextBlock->PreviousBlock = slice->PreviousBlock;
  if (slice->PreviousBlock)
    slice->PreviousBlock->NextBlock = slice->NextBlock;

  slice->NextBlock = slice->PreviousBlock = null;
}


// Wait until the lock is released, then set the locked flag
__attribute__((unused))
static inline void __Heap_Lock(__HeapMemory_Header* header) {
  // Wait until the lock is reset
  while(header->IsLocked)
    ;
  
  header->IsLocked = true;
}


// Reset the locked flag
__attribute__((unused))
static inline void __Heap_Unlock(__HeapMemory_Header* header) {
  header->IsLocked = false;
}


// Finds the folowing memory slice by its address
__attribute__((unused))
static inline __HeapMemory_Slice* __Heap_FindNextSlice(__HeapMemory_Slice* slice) {
  if (!slice->NextBlock)
    return null;
  
  __HeapMemory_Slice *next = slice->NextBlock;
  for (__HeapMemory_Slice* candidate = slice; candidate; candidate = candidate->NextBlock) {
    if (candidate <= slice)
      continue;

    void* sliceEnd = __Heap_GetHeapSliceEndPointer(slice);
    if (candidate == sliceEnd + 1)
      return candidate;

    if (candidate < next)
    next = candidate;
  }

  return next;
}



module(Heap) {

  // Initializes a new dynamic memory area
  HeapMemory (*Initialize) (void *pointer, U32 totalSize);

  // Allocates a continuous block of memory; returns a pointer or null in case of an error
  void* (*Allocate) (HeapMemory heapArea, U32 size);

  // Free a previously allocated dynamic memory area
  void (*Free)(HeapMemory heapArea, void* pointer);

  // Merge adjacent free blocks of memory
  void (*Defrag)(HeapMemory heapArea);
  
  // Get the amount of free bytes in this area
  U32 (*GetBytesFree)(HeapMemory heapArea);

  // Get the amount of allocated bytes in this area
  U32 (*GetBytesUsed)(HeapMemory heapArea);
  
};


#endif
