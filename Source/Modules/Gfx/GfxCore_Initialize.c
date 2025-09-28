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


#include "../Include/Gfx.h"

use(Heap);
use(Memory);

extern GfxData _Gfx_Data;

void* _GfxCore_InitializeImplementation(U16 width, U16 height, U8 numberOfPlanes, HeapArea* heap) {
  // Skip if already initialized
  if (_Gfx_Data.BackBufferAddress)
    return _Gfx_Data.BackBufferAddress;

  _Gfx_Data.ScreenWidth = width;
  _Gfx_Data.ScreenHeight = height;
  _Gfx_Data.NumberOfPlanes = numberOfPlanes;

  U32 planeSize = (width / 8) * height;
  _Gfx_Data.BackBufferSize = planeSize * numberOfPlanes;
  _Gfx_Data.BackBufferAddress = (U8*)Heap.Allocate(heap, _Gfx_Data.BackBufferSize);
  if (!_Gfx_Data.BackBufferAddress)
    return null;

  // Initialize back buffer memory
  Memory.Set(_Gfx_Data.BackBufferAddress, 0x00, _Gfx_Data.BackBufferSize);

  return _Gfx_Data.BackBufferAddress;
}
