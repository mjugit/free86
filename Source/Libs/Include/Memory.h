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

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "CoreTypes.h"

/*
  Represents a the namespace for low level memory operations. The
  actual implementation is in assembler, since it is far easier to
  optimize.
 */

struct __Memory_Namespace {
  
  // Copy a block of memory without overlap protection
  void (*Copy)(void *destination, const void *source, U32 count);

  // Flood a block of memory with a certain value
  void (*Set)(void *destination, U8 value, U32 count);

  // Copy a block of memory with overlap protection
  void (*Move)(void *source, const void *destination, U32 count);

  // Compare two blocks of memory
  int (*Compare)(const void *a, const void *b, U32 count);

  // Find the first occurrence of a value in a block of memory
  void* (*Find)(const void *block, U8 value, U32 count);
  
};


/*
  As stated above, the actual namespace definition is a struct
  definition in assembler. Direct access to the concrete
  implementation is not foreseen; use this constant to access
  functions via pointers.
 */

extern struct __Memory_Namespace Memory;

#endif
