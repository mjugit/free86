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


// ----------------------------------------------------------------------
// FUNDAMENTAL TYPES AND STRUCTURES
// ----------------------------------------------------------------------


// A RGB color definition
// The allowed value range for each subcolor is 0-63 (6 bit).
typedef struct Rgb18 {
  U8 Red;
  U8 Green;
  U8 Blue;
} Rgb18;


// Represents 2-dimensional vector
// This could for example be a location in a coordinate system or the size
// of a display item.
typedef struct Vector2d {
  // The value of the X-dimension (e.g. horizontal position)
  U16 X;
  // The value of the Y-dimension (e.g. vertical position)
  U16 Y;
} Vector2d;



// ----------------------------------------------------------------------
// BITMAPS AND RENDERING
// ----------------------------------------------------------------------


// At this point, font bitmaps (8x8, unrendered) are U8 arrays.
typedef U8 Bitmap8x8[8];


// Represents a prerendered character bitmap
typedef struct RenderChar {
  // The size of the character.
  Vector2d Size;

  // The character bitmap.
  // If the char was rendered proportional, this is the left aligned bitmap
  // (all whitespace is on the right); otherwise it is a 1:1 copy of the
  // original bitmap.
  Bitmap8x8 Bitmap;
} RenderChar;



// The maximum length of a font name
#define FONT_NAME_LENGTH 40

// The amount of characters in a font
#define FONT_CHAR_COUNT 95


// The default spacing between chars in pixels
#define FONT_DEFAULT_CHARSPACING 1

// The default spacing between lines in pixels
#define FONT_DEFAULT_LINESPACING 1


typedef struct Font {
  U8 Name[FONT_NAME_LENGTH];
  
  U8 CharSpacing;
  U8 LineSpacing;
  
  RenderChar Char[FONT_CHAR_COUNT];
} Font;



module(Renderer) {

  // Render a character bitmap for later use. Characters can not only be
  // rendered as monospace, but proportional, too.
  void (*RenderCharBitmap)(RenderChar *destination, Bitmap8x8 *bitmap, bool monospace);

  void (*RenderFont)(Font* destination, Bitmap8x8 *source, const char *name, bool monospace);
  
};

#endif
