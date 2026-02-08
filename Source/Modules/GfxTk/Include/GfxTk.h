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

#include "../../../Kernel/Include/HardwareIO.h"


// ----------------------------------------------------------------------
// FUNDAMENTALS
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



typedef struct VgaConfig {
  Vector2d Resolution;
  U16 PlaneCount;

  void* Backbuffer;
  void* ScreenBuffer;
} VgaConfig;



module (Vga) {

  // Enable the video output
  void (*EnableOutput)(void);

  // Disable the video output
  void (*DisableOutput)(void);

  // Set the mask for the next written bytes
  void (*SetBitmask)(U8 bitmask);

  // Set the plane mask for the next written bytes
  void (*SetPlaneMask)(U8 planeMask);

  // Idle until the next vertical sync
  void (*PauseUntilVSync)(void);

  // Define a palette color (256 color definitions in parallel
  // can be stored simultaneously)
  void (*SetPalette)(U8 index, Rgb18 color);

  // Configure the DAC to use a color from the palette
  // 16 colors are used at at time. Reconfiguring them to another palette
  // color globally effects everything on the screen.
  void (*UseColor)(U8 index, U8 paletteIndex);

};



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


// Represents a prerendered bitmap font
typedef struct Font {
  // The name of the font
  U8 Name[FONT_NAME_LENGTH];

  // The space between chars (pixels)
  U8 CharSpacing;
  // The space between lines (pixels)
  U8 LineSpacing;

  // The glyphs
  RenderChar Char[FONT_CHAR_COUNT];
} Font;

typedef enum {
  Undefined,
  CompressionShort,
  CompressionSquareShort,
  CompressionSquareTall,
  CompressionTall,
  Envious,
  EnviousBold,
  EnviousItalic,
  EnviousSerif,
  EnviousSerifBold,
  Forgotten,
  ForgottenBold,
  Pixie,
  PixieBell,
  PixieBellStretch,
  PixieBold,
  PixieDigital,
  Widget,
  WidgetBold,
  ZxCourier
} FontId;


module(Renderer) {

  // Render a character bitmap for later use. Characters can be rendered
  // as monospace and proportional.
  void (*RenderCharBitmap)(RenderChar *destination,
			   Bitmap8x8 *bitmap,
			   bool monospace);

  // Render all characters of a bitmap font for later use. Characters can
  // be rendered as monospace and proportional.
  void (*RenderFont)(Font* destination,
		     Bitmap8x8 *source,
		     const char *name,
		     bool monospace);

  // Render a colored and filled rectangle at a specific location.
  void (*RenderFilledRect)(VgaConfig *config,
			   Vector2d position,
			   Vector2d size,
			   U8 color);

  // Render a colored rectangle at a specific location. Though it is not
  // filled out, the line thickness can be specified.
  void (*RenderRect)(VgaConfig *config,
		     Vector2d position,
		     Vector2d size,
		     U8 thickness,
		     U8 color);

  // Render a character at a specific location.
  void (*RenderChar)(VgaConfig *config,
		     Vector2d position,
		     RenderChar *glyph,
		     U8 color);

  // Get a font bitmap by its id
  Bitmap8x8* (*GetFontBitmap)(FontId fontId);
  
  // Get the glyph of a certain ASCII character
  RenderChar* (*GetGlyph)(Font *font, char asciiChar);

  // Render a null-terminated string at a specific location.
  void (*RenderAsciiZ)(VgaConfig *config,
		       Vector2d position,
		       char *text,
		       Font *font,
		       U8 color);

  // Fill the whole screen with a color
  void (*FillScreen)(VgaConfig *config, U8 color);

  // Sync front and backbuffer
  void (*Refresh)(VgaConfig *config);
  
};



#endif
