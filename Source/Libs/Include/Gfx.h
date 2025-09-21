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

#ifndef _GFX_H_
#define _GFX_H_

#include "../../Modules/Include/SystemCore.h"
#include "../../Modules/Include/Memory.h"
#include "../../Modules/Include/Heap.h"
#include "../../Kernel/Include/HardwareIO.h"

use(Memory);
use(Heap);

typedef struct Rgb64 {
  U8 Red;
  U8 Green;
  U8 Blue;
} Rgb64;


typedef U8 Char8x8[8];


// Graphics port numbers
struct __Gfx_Port_Namespace {
  const U16 SequencerIndex;
  const U16 SequencerData;
  const U16 GraphicsIndex;
  const U16 GraphicsData;
  const U16 AttributeIndex;
  const U16 AttributeData;
  const U16 CrtIndex;
  const U16 CrtData;
  const U16 DacIndex;
  const U16 DacData;
  const U16 VgaStatus;
};

struct __Gfx_Core_Namespaces {
  void* (*Initialize)(U16 width, U16 height, U8 numPlanes, HeapArea* heap);
  void* (*GetBackBuffer)(void);
  U32 (*GetBackBufferSize)(void);
  U16 (*GetScreenWidth)(void);
  U16 (*GetScreenHeight)(void);
  U8 (*GetNumPlanes)(void);
  void (*RefreshFromBackBuffer)(void);
  void (*SetPlaneMask)(U8 planeMask);
  void (*SetBitMask)(U8 bitMask);
  void (*SetPaletteColor)(U8 paletteIndex, Rgb64 color);
  void (*UsePaletteColor)(U8 colorIndex, U8 paletteIndex);
  void (*DisableVideoOutput)(void);
  void (*EnableVideoOutput)(void);
  void (*WaitForVSync)(void);
};


struct __Gfx_Draw_Namespace {
  void (*Pixel)(U16 posX, U16 posY, U8 colorIndex);
  void (*Rect)(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex);
  void (*FilledRect)(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex);
  void (*Line)(U16 startX, U16 startY, U16 endX, U16 endY, U8 colorIndex);
  void (*Char)(U16 x, U16 y, char character, U8 color);
  void (*String)(U16 x, U16 y, const char *string, U8 color);
};

struct __Gfx_Namespace {
  struct __Gfx_Port_Namespace Port;
  struct __Gfx_Core_Namespaces Core;
  struct __Gfx_Draw_Namespace Draw;

  Rgb64 (*Color)(U8 red, U8 green, U8 blue);
};


extern const struct __Gfx_Namespace Gfx;

#endif
