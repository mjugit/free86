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


extern GfxData _Gfx_Data;


void _GfxDraw_PixelImplementation(U16 posX, U16 posY, U8 colorIndex) {
  U32 bytesPerPlane = (_Gfx_Data.ScreenWidth / 8) * _Gfx_Data.ScreenHeight;
  U8 bitMask = 0x80 >> (posX & 7);
  U32 byteOffset = (posY * (_Gfx_Data.ScreenWidth / 8)) + (posX / 8);

  for (U8 plane = 0; plane < _Gfx_Data.NumberOfPlanes; plane++) {
    U8* planeBase = _Gfx_Data.BackBufferAddress + (bytesPerPlane * plane);
    U8* pixelByte = planeBase + byteOffset;
    
    if (colorIndex & (1 << plane))
      *pixelByte |= bitMask;
    else
      *pixelByte &= ~bitMask;
  }
}
