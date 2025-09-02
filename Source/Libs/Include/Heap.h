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

#ifndef _HEAP_H_
#define _HEAP_H_

#include "../../Modules/Include/SystemCore.h"
#include "../../Modules/Include/Bitmap.h"

use(Bitmap);


typedef struct __Heap_HeapArea {
  void *BaseAddress;
  U32 TotalBytes;
  U32 UsableBytes;
  
  U16 BlockSize;
  U32 TotalBlocks;
  
  U8 *Bitmap;
  U32 BitmapBytes;
  
  void *DataStart;
  U32 DataBytes;
} HeapArea;

struct __Heap_Namespace {
  
  // Initializes a heap in a given memory location.
  // Sets up metadata, bitmap, and calculates usable blocks for allocations.
  HeapArea* (*Initialize)(void *location, U32 numBytes, U16 blockSize);

  // Allocates a memory region from the heap.
  // Searches for the required number of contiguous free blocks and marks them as used.
  void* (*Allocate)(HeapArea* heap, U32 size);
  
  // Frees a previously allocated memory region in the heap.
  // Clears the corresponding bits in the allocation bitmap.
  void (*Free)(HeapArea* heap, void* ptr, U32 numBytes);
  
};


extern const struct __Heap_Namespace Heap;

#endif
