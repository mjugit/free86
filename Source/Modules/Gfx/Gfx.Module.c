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


use(GfxDraw);


members(GfxPorts) {
    .SequencerIndex = 0x3c4,
    .SequencerData  = 0x3c5,
    .GraphicsIndex  = 0x3ce,
    .GraphicsData   = 0x3cf,
    .AttributeIndex = 0x3c0,
    .AttributeData  = 0x3c1,
    .CrtIndex	    = 0x3d4,
    .CrtData	    = 0x3d5,
    .DacIndex	    = 0x3c8,
    .DacData	    = 0x3c9,
    .VgaStatus	    = 0x3da
};


extern void _GfxDraw_PixelImplementation(U16 posX, U16 posY, U8 colorIndex);
extern void _GfxDraw_RectImplementation(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex);
extern void _GfxDraw_FilledRectImplementation(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex);
extern void _GfxDraw_LineImplementation(U16 startX, U16 startY, U16 endX, U16 endY, U8 colorIndex);
extern void _GfxDraw_CharImplementation(U16 x, U16 y, char character, U8 color, Font font);
extern void _GfxDraw_StringImplementation(U16 x, U16 y, const char *string, U8 color, Font font);
extern void _GfxDraw_PartialStringImplementation(U16 x, U16 y, const char *string, U32 length, U8 color, Font font);


members(GfxDraw) {
    .Pixel	     = _GfxDraw_PixelImplementation,
    .Rect	     = _GfxDraw_RectImplementation,
    .FilledRect	     = _GfxDraw_FilledRectImplementation,
    .Line	     = _GfxDraw_LineImplementation,
    .Char	     = _GfxDraw_CharImplementation,
    .String	     = _GfxDraw_StringImplementation,
    .PartialString   = _GfxDraw_PartialStringImplementation
};



extern void* _GfxCore_InitializeImplementation(U16 width, U16 height, U8 numberOfPlanes, HeapArea* heap);
extern void _GfxCore_RefreshImplementation(void);
extern void _GfxCore_SetBitMaskImplementation(U8 bitMask);
extern void _GfxCore_SetPlaneMaskImplementation(U8 planeMask);
extern void _GfxCore_EnableVideoOutputImplementation(void);
extern void _GfxCore_DisableVideoOutputImplementation(void);
extern void _GfxCore_WaitForVSyncImplementation(void);
extern void _GfxCore_UsePaletteColorImplementation(U8 colorIndex, U8 paletteIndex);
extern void _GfxCore_SetPaletteColorImplementation(U8 paletteIndex, Rgb64 color);


members(GfxCore) {
    .Initialize		= _GfxCore_InitializeImplementation,
    .Refresh		= _GfxCore_RefreshImplementation,
    .SetPlaneMask	= _GfxCore_SetPlaneMaskImplementation,
    .SetBitMask		= _GfxCore_SetBitMaskImplementation,
    .DisableVideoOutput = _GfxCore_DisableVideoOutputImplementation,
    .EnableVideoOutput	= _GfxCore_EnableVideoOutputImplementation,
    .WaitForVSync	= _GfxCore_WaitForVSyncImplementation,
    .UsePaletteColor = _GfxCore_UsePaletteColorImplementation,
    .SetPaletteColor = _GfxCore_SetPaletteColorImplementation      
};


members(Gfx) {
    .Ports = GfxPorts,
    .Core  = GfxCore,
    .Draw  = GfxDraw
};

