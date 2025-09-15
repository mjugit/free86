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

static U8* _VRamAddress = (U8*)0xa0000;
U8* _BackBufferAddress = 0;
static U32 _BackBufferSize = 0;

static U16 _ScreenWidth = 0;
static U16 _ScreenHeight = 0;
static U8 _NumPlanes = 0;


__attribute__((unused))
void* Gfx_Initialize(U16 width, U16 height, U8 numPlanes, HeapMemory heap) {
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


static void Gfx_Vram_Copy(void *dest, const void *src, unsigned int count) {
  unsigned int dwords = count / 4;
  unsigned int bytes  = count % 4;

  __asm__ __volatile__ (
			"cld\n\t" 
			"rep movsd\n\t" 
			: "+D"(dest), "+S"(src), "+c"(dwords)
			:
			: "memory"
			);

  __asm__ __volatile__ (
			"movl %2, %%ecx\n\t"
			"rep movsb\n\t"
			: "+D"(dest), "+S"(src)
			: "r"(bytes)
			: "ecx", "memory"
			);
}


__attribute__((unused))
void Gfx_RefreshFromBackBuffer(void) {
  if (!_BackBufferAddress)
    return;

  Gfx.Core.SetBitMask(0xff);
  Gfx.Core.WaitForVSync();

  U32 bytesPerPlane = (_ScreenWidth / 8) * _ScreenHeight;
  
  for (U8 plane = 0; plane < _NumPlanes; plane++) {
    Gfx.Core.SetPlaneMask(1 << plane);

    U8* buffer = _BackBufferAddress + (bytesPerPlane * plane);
    Gfx_Vram_Copy(_VRamAddress, buffer, bytesPerPlane);
  }
}



// Sets a single pixel in the backbuffer to the given 4-bit color index
__attribute__((unused))
void Gfx_SetPixel(U16 posX, U16 posY, U8 colorIndex) {
  U32 bytesPerPlane = (_ScreenWidth / 8) * _ScreenHeight;
  U8 bitMask = 0x80 >> (posX & 7);
  U32 byteOffset = (posY * (_ScreenWidth / 8)) + (posX / 8);

  for (U8 plane = 0; plane < _NumPlanes; plane++) {
    U8* planeBase = _BackBufferAddress + (bytesPerPlane * plane);
    U8* pixelByte = planeBase + byteOffset;
    
    if (colorIndex & (1 << plane))
      *pixelByte |= bitMask;
    else
      *pixelByte &= ~bitMask;
  }
}



static void Gfx_FillHorizontal(U16 posX, U16 row, U16 width, U8 colorIndex) {
  if (width == 0)
    return;

  U16 bytesPerRow = _ScreenWidth / 8;
  U32 bytesPerPlane = bytesPerRow * _ScreenHeight;

  U16 firstByteIndex = posX >> 3;
  U16 lastPixelX = posX + width - 1;
  U16 lastByteIndex = lastPixelX >> 3;

  U8 maskLeft = 0xff >> (posX & 7);
  U8 maskRight = 0xff << (7 - (lastPixelX & 7));

  for (U8 plane = 0; plane < _NumPlanes; plane++) {
    U8 *planeAddress = _BackBufferAddress + (bytesPerPlane * plane);
    U8 *rowAddress = planeAddress + (row * bytesPerRow);
    U8 fillValue = (colorIndex & (1 << plane)) ? 0xff : 0x00;

    if (firstByteIndex == lastByteIndex) {
      U8 mask = maskLeft & maskRight;
      if (fillValue)
	rowAddress[firstByteIndex] |= mask;
      else
	rowAddress[firstByteIndex] &= ~mask;
      continue;
    }

    if (fillValue)
      rowAddress[firstByteIndex] |= maskLeft;
    else
      rowAddress[firstByteIndex] &= ~maskLeft;

    for (U16 byteIndex = firstByteIndex + 1; byteIndex < lastByteIndex; byteIndex++)
      rowAddress[byteIndex] = fillValue;

    if (fillValue)
      rowAddress[lastByteIndex] |= maskRight;
    else
      rowAddress[lastByteIndex] &= ~maskRight;
  }
}



// Draws a filled rectangle
__attribute__((unused))
void Gfx_DrawFilledRect(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex) {
  if (posX + width > _ScreenWidth)
    width = _ScreenWidth - posX;

  if (posY + height > _ScreenHeight)
    height = _ScreenHeight - posY;

  U16 endY = posY + height;
  for (U16 row = posY; row < endY; row++)
    Gfx_FillHorizontal(posX, row, width, colorIndex);
}


