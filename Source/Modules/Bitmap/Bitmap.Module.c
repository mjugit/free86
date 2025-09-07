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

void	_Bitmap_SetImplementation(void* bitmap, U32 bit);
void	_Bitmap_ClearImplementation(void *bitmap, U32 bit);
bool	_Bitmap_CheckImplementation(void *bitmap, U32 bit);
void	_Bitmap_WipeImplementation(void *bitmap, U32 count);
I32	_Bitmap_NextFreeImplementation(void *bitmap, U32 bitsFree, U32 size);


members(Bitmap) {
    .Set      = _Bitmap_SetImplementation,
    .Clear    = _Bitmap_ClearImplementation,
    .Check    = _Bitmap_CheckImplementation,
    .Wipe     = _Bitmap_WipeImplementation,
    .NextFree = _Bitmap_NextFreeImplementation
};
