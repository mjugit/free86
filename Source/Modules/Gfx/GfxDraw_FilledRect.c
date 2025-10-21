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

use(Gfx);


extern GfxData _Gfx_Data;
static void _FillHorizontal(U16 posX, U16 row, U16 width, U8 colorIndex);


void _GfxDraw_FilledRectImplementation(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex) {
  if (posX + width > _Gfx_Data.ScreenWidth)
    width = _Gfx_Data.ScreenWidth - posX;

  if (posY + height > _Gfx_Data.ScreenHeight)
    height = _Gfx_Data.ScreenHeight - posY;

  U16 endY = posY + height;
  for (U16 row = posY; row < endY; row++)
    _FillHorizontal(posX, row, width, colorIndex);
}


static void _FillHorizontal(U16 posX, U16 row, U16 width, U8 colorIndex) {
  if (width == 0)
    return;

  U16 bytesPerRow = _Gfx_Data.ScreenWidth / 8;
  U32 bytesPerPlane = bytesPerRow * _Gfx_Data.ScreenHeight;

  U16 firstByteIndex = posX >> 3;
  U16 lastPixelX = posX + width - 1;
  U16 lastByteIndex = lastPixelX >> 3;

  U8 maskLeft = 0xff >> (posX & 7);
  U8 maskRight = 0xff << (7 - (lastPixelX & 7));

  for (U8 plane = 0; plane < _Gfx_Data.NumberOfPlanes; plane++) {
    U8 *planeAddress = _Gfx_Data.BackBufferAddress + (bytesPerPlane * plane);
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
