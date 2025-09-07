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


#include "../Include/Bitmap.h"


I32 _Bitmap_NextFreeImplementation(void *bitmap_ptr, U32 bitsFree, U32 bitCount) {
  U8 *bitmap = (U8*)bitmap_ptr;
  
  if (bitsFree == 0)
    return 0;
  if (bitmap == 0 || bitCount == 0 || bitsFree > bitCount)
    return -1;

  // Initialize state

  U32	runStart = 0;
  U32	runLen   = 0;

  for (U32 index = 0; index < bitCount; index++) {
    U32 byteIndex = index >> 3;
    U32 bitInByte = index & 7;
    U8	byte	  = bitmap[byteIndex];
    U8	occupied  = (U8)((byte >> bitInByte) & 1u);
    
    if (occupied == 0) {
      if (runLen == 0)
        runStart = index;
      runLen++;
      if (runLen >= bitsFree)
        return (I32)runStart;
    } else {
      runLen	 = 0;
    }
  }

  return -1;
}
