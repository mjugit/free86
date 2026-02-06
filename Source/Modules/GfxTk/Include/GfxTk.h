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


#ifndef _GFXTK_H_
#define _GFXTK_H_

#include "../../Include/CModule.h"
#include "../../Include/SystemCore.h"


// At this point, font bitmaps (8x8, unrendered) are U8 arrays.
typedef U8 Bitmap8x8[8];


// Represents a prerendered character bitmap
typedef struct RenderChar {
  // The character width.
  // If the character was rendered proportional, this is the width minus
  // the sourounding whitespace; otherwise it is set to 8.
  U8 Width;

  // The character height.
  // At the moment this is the bitmap height, which is 8.
  U8 Height;

  // The character bitmap.
  // If the char was rendered proportional, this is the left aligned bitmap
  // (all whitespace is on the right); otherwise it is a 1:1 copy of the
  // original bitmap.
  Bitmap8x8 Bitmap;
} RenderChar;


module(Renderer) {

  // Render a character bitmap for later use. Characters can not only be
  // rendered as monospace, but proportional, too.
  RenderChar (*RenderCharBitmap)(Bitmap8x8 bitmap, bool monospace);
  
};

#endif
