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


// Core functions
void*	Gfx_Initialize(U16 width, U16 height, U8 numPlanes, HeapArea* heap);
void*	Gfx_GetBackBuffer(void);
U32	Gfx_GetBackBufferSize(void);
U16	Gfx_GetScreenWidth(void);
U16	Gfx_GetScreenHeight(void);
U8	Gfx_GetNumPlanes(void);
void	Gfx_RefreshFromBackBuffer(void);
void	Gfx_SetPlaneMask(U8 planeMask);
void	Gfx_SetBitMask(U8 bitMask);
void	Gfx_SetPaletteColor(U8 paletteIndex, Rgb64 color);
void	Gfx_UsePaletteColor(U8 colorIndex, U8 paletteIndex);
void	Gfx_DisableVideoOutput(void);
void	Gfx_EnableVideoOutput(void);
void	Gfx_WaitForVSync(void);


// Draw functions
void Gfx_SetPixel(U16 posX, U16 posY, U8 colorIndex);
void Gfx_DrawRect(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex);
void Gfx_DrawFilledRect(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex);
void Gfx_DrawLine(U16 startX, U16 startY, U16 endX, U16 endY, U8 colorIndex);
void Gfx_DrawChar(U16 x, U16 y, char character, U8 color);
void Gfx_DrawString(U16 x, U16 y, const char *string, U8 color);


// Helpers
Rgb64 Gfx_Color(U8 red, U8 green, U8 blue);



const struct __Gfx_Namespace Gfx = {
  // Important ports
  .Port = {
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
  },

  // Core functions
  .Core = {
    .Initialize = Gfx_Initialize,
    .GetBackBuffer = Gfx_GetBackBuffer,
    .GetBackBufferSize = Gfx_GetBackBufferSize,
    .GetScreenWidth = Gfx_GetScreenWidth,
    .GetScreenHeight = Gfx_GetScreenHeight,
    .GetNumPlanes = Gfx_GetNumPlanes,
    .RefreshFromBackBuffer = Gfx_RefreshFromBackBuffer,
    .SetPlaneMask = Gfx_SetPlaneMask,
    .SetBitMask = Gfx_SetBitMask,
    .SetPaletteColor = Gfx_SetPaletteColor,
    .UsePaletteColor = Gfx_UsePaletteColor,
    .DisableVideoOutput = Gfx_DisableVideoOutput,
    .EnableVideoOutput = Gfx_EnableVideoOutput,
    .WaitForVSync = Gfx_WaitForVSync
  },

  // Draw functions
  .Draw = {
    .Pixel = Gfx_SetPixel,
    .Rect = Gfx_DrawRect,
    .FilledRect = Gfx_DrawFilledRect,
    .Line = Gfx_DrawLine,
    .Char = Gfx_DrawChar,
    .String = Gfx_DrawString
  },

  // Helpers
  .Color = Gfx_Color
};
