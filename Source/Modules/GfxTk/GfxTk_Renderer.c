/*
	
  Copyright © 2026 Maximilian Jung

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


#include "Include/GfxTk.h"

import(Renderer);


static inline void _CopyBitmap8x8(Bitmap8x8 *bitmap, U8* destination) {
  for (U8 lineIndex = 0; lineIndex < sizeof(Bitmap8x8); lineIndex++)
    destination[lineIndex] = (*bitmap)[lineIndex];
}


static inline U8 _OrBitmap8x8(Bitmap8x8 *bitmap) {
  U8 result = 0;
  for (U8 lineIndex = 0; lineIndex < sizeof(Bitmap8x8); lineIndex++)
    result |= (*bitmap)[lineIndex];

  return result;
}


static inline U8 _GetFreeBitsLeft(U8 input) {
  const U8 checkMask = 0b10000000;
  if (input & checkMask)
    return 0;

  const U8 maxShifts = 7;
  U8 totalShifts = 0;
  
  while ((++totalShifts) < maxShifts) {
    input <<= 1;

    if (input & checkMask)
      return totalShifts;
  }
  
  return totalShifts;
}


static inline void _ShiftBitmap(Bitmap8x8 *bitmap, U8 width) {
  for (U8 lineIndex = 0; lineIndex < sizeof(Bitmap8x8); lineIndex++) {
    U8 line = ((U8*)bitmap)[lineIndex];
    line <<= width;
  }
}


static inline U8 _GetFreeBitsRight(U8 input) {
  if (input & 1)
    return 0;

  const U8 maxShifts = 8;
  U8 totalShifts = 0;

  while ((++totalShifts) < maxShifts) {
    input >>= 1;

    if (input & 1)
      return totalShifts;
  }

  return totalShifts;
}



void _GfxTk_RenderCharBitmap(RenderChar *destination, Bitmap8x8 *bitmap, bool monospace) {
  if (!destination || !bitmap)
    return;
  
  destination->Size.Y = sizeof(Bitmap8x8);
  
  if (monospace) {
    // Just copy and use a fixed width
    _CopyBitmap8x8(bitmap, destination->Bitmap);
    destination->Size.X = 8;
    return;
  }

  // Check how much whitespace we have
  U8 bitmapSig = _OrBitmap8x8(bitmap);
  U8 whitespaceLeft = bitmapSig ? _GetFreeBitsLeft(bitmapSig) : 0;
  U8 whitespaceRight = bitmapSig ? _GetFreeBitsRight(bitmapSig) : 0;

  // Align left
  if (bitmapSig)
    _ShiftBitmap(bitmap, whitespaceLeft);
  _CopyBitmap8x8(bitmap, destination->Bitmap);

  // Set width without whitespace
  destination->Size.X = 8 - whitespaceLeft - whitespaceRight;
}



void _GfxTk_RenderFont(Font* destination, Bitmap8x8 *source, const char *name, bool monospace) {
  if (!destination || !source || !name)
    return;

  for (U16 nameIndex = 0; nameIndex < FONT_NAME_LENGTH && name[nameIndex]; nameIndex++)
    destination->Name[nameIndex] = name[nameIndex];

  destination->CharSpacing = FONT_DEFAULT_CHARSPACING;
  destination->LineSpacing = FONT_DEFAULT_LINESPACING;

  for (U8 charIndex = 0; charIndex < FONT_CHAR_COUNT; charIndex++) {
    RenderChar *nextChar = &destination->Char[charIndex];
    Bitmap8x8 *glyph = &source[charIndex];
    Renderer.RenderCharBitmap(nextChar, glyph, monospace);
  }
}




static inline void _FillRow(VgaConfig *config, Vector2d position, U16 width, U8 color) {
  if (!width)
    return;

  const U16	bytesPerRow   = config->Resolution.X / 8;
  const U32	bytesPerPlane = bytesPerRow * config->Resolution.Y;

  const U16	firstByteIndex = position.X >> 3;
  const U16	lastPixelX     = position.X + position.X - 1;
  const U16	lastByteIndex  = lastPixelX >> 3;

  const U8	maskLeft  = 0xff >> (position.X & 7);
  const U8	maskRight = 0xff << (7 - (lastPixelX & 7));

  for (U8 plane = 0; plane < config->PlaneCount; plane++) {
    U8 *planeAddress = config->Backbuffer + (bytesPerPlane * plane);
    U8 *rowAddress = planeAddress + (position.Y * bytesPerRow);
    U8 fillValue = (color & (1 << plane))
      ? 0xff
      : 0x00;

    if (firstByteIndex == lastByteIndex) {
      U8 mask = maskLeft & maskRight;
      if (fillValue)
	rowAddress[firstByteIndex] |= mask;
      else
	rowAddress[firstByteIndex] &= ~mask;
      continue;
    }

    if (fillValue) {
      if (fillValue)
	rowAddress[firstByteIndex] |= maskLeft;
      else 
	rowAddress[firstByteIndex] &= ~maskLeft;
    }

    for (U16 byteIndex = firstByteIndex + 1; byteIndex < lastByteIndex; byteIndex++)
      rowAddress[byteIndex] = fillValue;

    if (fillValue) {
      if (fillValue)
	rowAddress[firstByteIndex] |= maskRight;
      else
	rowAddress[firstByteIndex] &= ~maskRight;
    }
  }
}

void _GfxTk_RenderFilledRect(VgaConfig *config, Vector2d position, Vector2d size, U8 color) {
  const U16 width = (position.X + size.X > config->Resolution.X)
    ? config->Resolution.X - position.X
    : size.X;

  const U16 height = (position.Y + size.Y > config->Resolution.Y)
    ? config->Resolution.Y - position.Y
    : size.Y;

  const U16 lastRow = position.Y + height;
  for (U16 row = position.Y; row < lastRow; row++)
    _FillRow(config, (Vector2d) { .X = position.X, .Y = row }, width, color);
}

