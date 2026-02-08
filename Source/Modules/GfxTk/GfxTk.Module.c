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

extern void _GfxTk_RenderCharBitmap(RenderChar *destination,
				    Bitmap8x8 *bitmap,
				    bool monospace);

extern void _GfxTk_RenderFont(Font* destination,
			      Bitmap8x8 *source,
			      const char *name,
			      bool monospace);

extern void _GfxTk_RenderFilledRect(VgaConfig *config,
				    Vector2d position,
				    Vector2d size,
				    U8 color);

extern void _GfxTk_RenderRect(VgaConfig *config,
			      Vector2d position,
			      Vector2d size,
			      U8 thickness,
			      U8 color);

extern void _GfxTk_RenderChar(VgaConfig *config,
			      Vector2d position,
			      RenderChar *glyph,
			      U8 color);

extern RenderChar* _GfxTk_GetGlyph(Font *font, char asciiChar);

extern void _GfxTk_RenderAsciiZ(VgaConfig *config,
				Vector2d position,
				char *text,
				Font *font,
				U8 color);

extern void _GfxTk_FillScreen(VgaConfig *config, U8 color);

extern void _GfxTk_Refresh(VgaConfig *config);

extern Bitmap8x8* _GfxTk_GetFontBitmap(FontId fontId);


members(Renderer) {
    .RenderCharBitmap = _GfxTk_RenderCharBitmap,
    .RenderFont = _GfxTk_RenderFont,
    .RenderFilledRect = _GfxTk_RenderFilledRect,
    .RenderRect = _GfxTk_RenderRect,
    .RenderChar = _GfxTk_RenderChar,
    .GetGlyph = _GfxTk_GetGlyph,
    .RenderAsciiZ = _GfxTk_RenderAsciiZ,
    .FillScreen = _GfxTk_FillScreen,
    .Refresh = _GfxTk_Refresh,
    .GetFontBitmap = _GfxTk_GetFontBitmap
};




extern void _GfxTk_EnableOutput(void);

extern void _GfxTk_DisableOutput(void);

extern void _GfxTk_SetBitmask(U8 bitmask);

extern void _GfxTk_SetPlaneMask(U8 planeMask);

extern void _GfxTk_PauseUntilVSync(void);

extern void _GfxTk_SetPalette(U8 index, Rgb18 color);

extern void _GfxTk_UseColor(U8 index, U8 paletteIndex);


members(Vga) {
    .EnableOutput = _GfxTk_EnableOutput,
    .DisableOutput = _GfxTk_DisableOutput,
    .SetBitmask = _GfxTk_SetBitmask,
    .SetPlaneMask = _GfxTk_SetPlaneMask,
    .PauseUntilVSync = _GfxTk_PauseUntilVSync,
    .SetPalette = _GfxTk_SetPalette,
    .UseColor = _GfxTk_UseColor
};
