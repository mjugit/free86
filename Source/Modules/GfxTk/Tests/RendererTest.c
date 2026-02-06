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


#include "MinUnit.h"
#include "../Include/GfxTk.h"
#include "../Include/Font_ZXCourier.h"


import(Renderer);


// References

MU_TEST(CModuleSetup__RendererMethods__HaveValidRef) {
  mu_assert(Renderer.RenderCharBitmap != null, "RenderChar contains invalid pointer.");
}

MU_TEST_SUITE(CModuleSetup) {
  MU_RUN_TEST(CModuleSetup__RendererMethods__HaveValidRef);
}



// Font renderer

MU_TEST(RenderCharBitmap__Monospace__SetsFixedDimensions) {
  Bitmap8x8 charBitmap[] = {
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101
  };
  
  RenderChar result = { };
  Renderer.RenderCharBitmap(&result, charBitmap, true);

  mu_assert_int_eq(8, result.Size.Y);
  mu_assert_int_eq(8, result.Size.X);
  for (U8 bitmapIndex = 0; bitmapIndex < sizeof(Bitmap8x8); bitmapIndex++)
    mu_check(((U8*)charBitmap)[bitmapIndex] == result.Bitmap[bitmapIndex]);
}

MU_TEST(RenderCharBitmap__NonMonospace__SetsProportionalDimensions) {
  Bitmap8x8 charBitmap[] = {
    0b00001100,
    0b00001100,
    0b00001100,
    0b00001100,
    0b00001110,
    0b00001100,
    0b00001100,
    0b00011100
  };
  
  RenderChar result = { };
  Renderer.RenderCharBitmap(&result, charBitmap, false);

  mu_assert_int_eq(8, result.Size.Y);
  mu_assert_int_eq(4, result.Size.X);
  for (U8 bitmapIndex = 0; bitmapIndex < sizeof(Bitmap8x8); bitmapIndex++)
    mu_check(((U8*)charBitmap)[bitmapIndex] == result.Bitmap[bitmapIndex]);
}

MU_TEST(RenderCharBitmap__InvalidInput__ReturnsDefault) {
  RenderChar result = { };
  Renderer.RenderCharBitmap(&result, null, false);

  mu_assert_int_eq(0, result.Size.Y);
  mu_assert_int_eq(0, result.Size.X);
  for (U8 bitmapIndex = 0; bitmapIndex < sizeof(Bitmap8x8); bitmapIndex++)
    mu_check(result.Bitmap[bitmapIndex] == 0);
}

MU_TEST_SUITE(RenderCharBitmap) {
  MU_RUN_TEST(RenderCharBitmap__Monospace__SetsFixedDimensions);
  MU_RUN_TEST(RenderCharBitmap__NonMonospace__SetsProportionalDimensions);
  MU_RUN_TEST(RenderCharBitmap__InvalidInput__ReturnsDefault);
}



MU_TEST(RenderFont__Proportional__RendersFont) {
  Font result = { };
  Bitmap8x8* bitmap = (Bitmap8x8*)FONT_ZX_COURIER_BITMAP;
  const char name[] = "Name";

  Renderer.RenderFont(&result, bitmap, name, false);

  mu_assert_int_eq(FONT_DEFAULT_CHARSPACING, result.CharSpacing);
  mu_assert_int_eq(FONT_DEFAULT_LINESPACING, result.LineSpacing);
  mu_assert_string_eq(name, (char*)result.Name);
}

MU_TEST(RenderFont__Monospace__RendersFont) {
  Font result = { };
  Bitmap8x8* bitmap = (Bitmap8x8*)FONT_ZX_COURIER_BITMAP;
  const char name[] = "Name";

  Renderer.RenderFont(&result, bitmap, name, true);

  mu_assert_int_eq(FONT_DEFAULT_CHARSPACING, result.CharSpacing);
  mu_assert_int_eq(FONT_DEFAULT_LINESPACING, result.LineSpacing);
  mu_assert_string_eq(name, (char*)result.Name);
}

MU_TEST_SUITE(RenderFont) {
  MU_RUN_TEST(RenderFont__Proportional__RendersFont);
  MU_RUN_TEST(RenderFont__Monospace__RendersFont);
}



int main(void) {
  // Verify, that the module is properly set up
  MU_RUN_SUITE(CModuleSetup);

  // Character bitmap renderer
  MU_RUN_SUITE(RenderCharBitmap);
  MU_RUN_SUITE(RenderFont);
  
  MU_REPORT();
  return MU_EXIT_CODE;
}
