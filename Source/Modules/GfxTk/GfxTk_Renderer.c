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

import(Vga);
import(Renderer);


static inline void _CopyBitmap8x8(const Bitmap8x8 *bitmap, U8* destination) {
  for (U8 lineIndex = 0; lineIndex < sizeof(Bitmap8x8); lineIndex++)
    destination[lineIndex] = (*bitmap)[lineIndex];
}


static inline U8 _OrBitmap8x8(const Bitmap8x8 *bitmap) {
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
  for (U8 lineIndex = 0; lineIndex < sizeof(Bitmap8x8); lineIndex++)
    ((U8*)bitmap)[lineIndex] <<= width;
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



void _GfxTk_RenderCharBitmap(RenderChar *destination,
			     Bitmap8x8 *bitmap,
			     bool monospace) {
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


void _GfxTk_RenderFont(Font* destination,
		       Bitmap8x8 *source,
		       const char *name,
		       bool monospace) {
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



static inline void _FillRow(VgaConfig *config,
			    Vector2d position,
			    U16 width,
			    U8 color) {
  if (!width)
    return;

  const U16	bytesPerRow   = config->Resolution.X / 8;
  const U32	bytesPerPlane = bytesPerRow * config->Resolution.Y;

  const U16	firstByteIndex = position.X >> 3;
  const U16	lastPixelX     = position.X + width - 1;
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


static inline U16 _GetWidth(VgaConfig *config, Vector2d position, Vector2d size) {
  return (position.X + size.X > config->Resolution.X)
    ? config->Resolution.X - position.X
    : size.X;
}


static inline U16 _GetHeight(VgaConfig *config, Vector2d position, Vector2d size) {
  return (position.Y + size.Y > config->Resolution.Y)
    ? config->Resolution.Y - position.Y
    : size.Y;
}



void _GfxTk_RenderFilledRect(VgaConfig *config,
			     Vector2d position,
			     Vector2d size,
			     U8 color) {
  const U16 width = _GetWidth(config, position, size);
  const U16 height = _GetHeight(config, position, size);

  const U16 lastRow = position.Y + height;
  for (U16 row = position.Y; row < lastRow; row++)
    _FillRow(config, (Vector2d) { .X = position.X, .Y = row }, width, color);
}


void _GfxTk_RenderRect(VgaConfig *config,
		       Vector2d position,
		       Vector2d size,
		       U8 thickness,
		       U8 color) {
  if (!size.X || !size.Y)
    return;

  const U16 width = _GetWidth(config, position, size);
  const U16 height = _GetHeight(config, position, size);
  const U16 lastRow = position.Y + height;
  
  // Draw top row
  const U16 topEnd = position.Y + thickness;
  for (U16 row = position.Y; row < topEnd; row++)
    _FillRow(config, (Vector2d) { .X = position.X, .Y = row }, width, color);

  // Draw bottom row
  const U16 bottomStart = position.Y + height - thickness;
  for (U16 row = bottomStart; row < lastRow; row++)
    _FillRow(config, (Vector2d) { .X = position.X, .Y = row }, width, color);

  // Draw left border
  const U16 borderStart = position.Y + thickness;
  for (U16 row = borderStart; row < bottomStart; row++) {
    _FillRow(config, (Vector2d) { .X = position.X, .Y = row}, thickness, color);
    _FillRow(config, (Vector2d) { .X = position.X + width - thickness, .Y = row}, thickness, color);
  }
}


void _GfxTk_RenderChar(VgaConfig *config,
		       Vector2d position,
		       RenderChar *glyph,
		       U8 color) {

  const U16	bytesPerRow   = config->Resolution.X / 8;
  const U32	bytesPerPlane = bytesPerRow * config->Resolution.Y;
  
  for (U16 rowIndex = 0; rowIndex < glyph->Size.Y; rowIndex++) {
    U8 bitmapRow = glyph->Bitmap[rowIndex];

    for (U16 columnIndex = 0; columnIndex < 8; columnIndex++) {
      if ((bitmapRow & (1<< (7 - columnIndex))) == 0)
	continue;
      
      U16 positionX = position.X + columnIndex;
      U16 positionY = position.Y + rowIndex;

      if (positionX >= config->Resolution.X ||
	  positionY >= config->Resolution.Y)
	continue;

      U8 planeMask = 1;
      for (U8 plane = 0; plane < config->PlaneCount; plane++, planeMask <<= 1) {
	U8 planeBit = (color & planeMask)
	  ? 1
	  : 0;

	U32 byteOffset = (positionY * (config->Resolution.X / 8)) + (positionX / 8);
	U8 bitmask = 0x80 >> (positionX & 7);

	U8* destination = config->Backbuffer + (plane * bytesPerPlane) + byteOffset;
	if (planeBit)
	  *destination |= bitmask;
	else
	  *destination &= ~bitmask;
      }
      
    }
  }
}


RenderChar* _GfxTk_GetGlyph(Font *font, char asciiChar) {
  if (!font)
    return null;

  return &font->Char[asciiChar - 32];
}


void _GfxTk_RenderAsciiZ(VgaConfig *config,
			 Vector2d position,
			 char *text,
			 Font *font,
			 U8 color) {
  U16 offset = 0;
  for (char *textPtr = text; *textPtr; textPtr++) {
    Vector2d nextPos = {position.X + offset, position.Y };
    RenderChar *nextGlyph = Renderer.GetGlyph(font, *textPtr);
    Renderer.RenderChar(config, nextPos, nextGlyph, color);
    offset += nextGlyph->Size.X + font->CharSpacing;
  }
}


void _GfxTk_FillScreen(VgaConfig *config, U8 color) {
  for (U16 line = 0; line < config->Resolution.Y; line++)
    _FillRow(config, (Vector2d) { 0, line }, config->Resolution.X, color);
}


static void _Vram_Copy(void *dest, const void *src, U32 count) {
  U32 dwords = count / 4;
  U32 bytes  = count % 4;

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


void _GfxTk_Refresh(VgaConfig *config) {
  if (!config->Backbuffer)
    return;

  U32 bytesPerPlane = (config->Resolution.X / 8) * config->Resolution.Y;
  
  Vga.SetBitmask(0xff);
  Vga.PauseUntilVSync();

  for (U8 plane = 0; plane < config->PlaneCount; plane++) {
    Vga.SetPlaneMask(1 << plane);
    U8* buffer = config->Backbuffer + (bytesPerPlane * plane);
    _Vram_Copy(config->ScreenBuffer, buffer, bytesPerPlane);
  }
}




#include "Include/Font_CompressionShort.h"
#include "Include/Font_CompressionSquareShort.h"
#include "Include/Font_CompressionSquareTall.h"
#include "Include/Font_CompressionTall.h"
#include "Include/Font_Envious.h"
#include "Include/Font_EnviousBold.h"
#include "Include/Font_EnviousItalic.h"
#include "Include/Font_EnviousSerif.h"
#include "Include/Font_EnviousSerifBold.h"
#include "Include/Font_Forgotten.h"
#include "Include/Font_ForgottenBold.h"
#include "Include/Font_Pixie.h"
#include "Include/Font_PixieBell.h"
#include "Include/Font_PixieBellStretch.h"
#include "Include/Font_PixieBold.h"
#include "Include/Font_PixieDigital.h"
#include "Include/Font_Widget.h"
#include "Include/Font_WidgetBold.h"
#include "Include/Font_ZXCourier.h"


Bitmap8x8* _GfxTk_GetFontBitmap(FontId fontId) {
  switch (fontId) {
  case CompressionShort:
    return (Bitmap8x8*)FONT_COMPRESSION_SHORT_BITMAP;

  case CompressionSquareShort:
    return (Bitmap8x8*)FONT_COMPRESSION_SQUARE_SHORT_BITMAP;

  case CompressionSquareTall:
    return (Bitmap8x8*)FONT_COMPRESSION_SQUARE_TALL_BITMAP;

  case CompressionTall:
    return (Bitmap8x8*)FONT_COMPRESSION_TALL_BITMAP;

  case EnviousBold:
    return (Bitmap8x8*)FONT_ENVIOUS_BOLD_BITMAP;

  case Envious:
    return (Bitmap8x8*)FONT_ENVIOUS_BITMAP;

  case EnviousItalic:
    return (Bitmap8x8*)FONT_ENVIOUS_ITALIC_BITMAP;

  case EnviousSerif:
    return (Bitmap8x8*)FONT_ENVIOUS_SERIF_BITMAP;
    
  case EnviousSerifBold:
    return (Bitmap8x8*)FONT_ENVIOUS_SERIF_BOLD_BITMAP;

  case Forgotten:
    return (Bitmap8x8*)FONT_FORGOTTEN_BITMAP;

  case ForgottenBold:
    return (Bitmap8x8*)FONT_FORGOTTEN_BOLD_BITMAP;

  case Pixie:
    return (Bitmap8x8*)FONT_PIXIE_BITMAP;
    
  case PixieBell:
    return (Bitmap8x8*)FONT_PIXIE_BELL_BITMAP;
    
  case PixieBellStretch:
    return (Bitmap8x8*)FONT_PIXIE_BELL_STRETCH_BITMAP;
    
  case PixieBold:
    return (Bitmap8x8*)FONT_PIXIE_BOLD_BITMAP;
    
  case PixieDigital:
    return (Bitmap8x8*)FONT_PIXIE_DIGITAL_BITMAP;
    
  case Widget:
    return (Bitmap8x8*)FONT_WIDGET_BITMAP;
    
  case WidgetBold:
    return (Bitmap8x8*)FONT_WIDGET_BOLD_BITMAP;
    
  case ZxCourier:
    return (Bitmap8x8*)FONT_ZX_COURIER_BITMAP;

  default:
    return null;
  }
}

