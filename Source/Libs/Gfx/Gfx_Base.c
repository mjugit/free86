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

static U8* _BackBufferAddress = 0;
static U32 _BackBufferSize = 0;

static U16 _ScreenWidth = 0;
static U16 _ScreenHeight = 0;
static U8 _NumPlanes = 0;


__attribute__((unused))
void* Gfx_Initialize(U16 width, U16 height, U8 numPlanes, HeapArea* heap) {
  // Skip if already initialized
  if (_BackBufferAddress)
    return _BackBufferAddress;
  
  _ScreenWidth = width;
  _ScreenHeight = height;
  _NumPlanes = numPlanes;

  U32 planeSize = (width / 8) * height;
  _BackBufferSize = planeSize * numPlanes;

  _BackBufferAddress = (U8*)Heap.Allocate(heap, _BackBufferSize);
  if (!_BackBufferAddress)
    return 0;
  
  Memory.Set(_BackBufferAddress, 0x00, _BackBufferSize);
  return _BackBufferAddress;
}



__attribute__((unused))
void* Gfx_GetBackBuffer(void) {
  return _BackBufferAddress;
}



__attribute__((unused))
U32 Gfx_GetBackBufferSize(void) {
  return _BackBufferSize;
}



__attribute__((unused))
U16 Gfx_GetScreenWidth(void) {
  return _ScreenWidth;
}



__attribute__((unused))
U16 Gfx_GetScreenHeight(void) {
  return _ScreenHeight;
}



__attribute__((unused))
U8 Gfx_GetNumPlanes(void) {
  return _NumPlanes;
}



__attribute__((unused))
void Gfx_RefreshFromBackBuffer(void) {
  if (!_BackBufferAddress)
    return;

  Gfx.Core.SetBitMask(0xff);
  Gfx.Core.WaitForVSync();

  U8* vramAddress = (U8*)0xa0000;
  U8* backBufferAddress = (U8*)_BackBufferAddress;
  U32 bytesPerPlane = (_ScreenWidth / 8) * _ScreenHeight;
  
  for (int plane = 0; plane < 4; plane++) {
    Gfx.Core.SetPlaneMask(1 << plane);
    
    U8* buffer = backBufferAddress + (bytesPerPlane * plane);
    Memory.Copy(vramAddress, buffer, bytesPerPlane);
  }
}
