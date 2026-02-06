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


#define _GFXTK_PORT_VGASTATUS 0x3da

#define _GFXTK_PORT_ATTRIBUTE_INDEX 0x3c0

#define _GFXTK_PORT_ATTRIBUTE_DATA 0x3c1


void _GfxTk_EnableOutput(void) {
  // Switch to index mode
  PortReadByte(_GFXTK_PORT_VGASTATUS);          
  PortWriteByte(_GFXTK_PORT_ATTRIBUTE_INDEX, 0x10);

  // Bit 5 = 1 -> Video on
  PortWriteByte(_GFXTK_PORT_ATTRIBUTE_INDEX, 0x20); 
}


void _GfxTk_DisableOutput(void) {
  // Switch to index mode
  PortReadByte(_GFXTK_PORT_VGASTATUS);          
  PortWriteByte(_GFXTK_PORT_ATTRIBUTE_INDEX, 0x10);

  // Bit 5 = 0 -> Video off
  PortWriteByte(_GFXTK_PORT_ATTRIBUTE_INDEX, 0x00); 
}



#define _GFXTK_PORT_GRAPHICS_INDEX 0x3ce

#define _GFXTK_PORT_GRAPHICS_DATA 0x3cf

#define _GFXTK_INDEX_BITMASK 0x08


void _GfxTk_SetBitmask(U8 bitmask) {
  PortWriteByte(_GFXTK_PORT_GRAPHICS_INDEX, _GFXTK_INDEX_BITMASK);
  PortWriteByte(_GFXTK_PORT_GRAPHICS_DATA, bitmask);
}



#define _GFXTK_PORT_SEQUENCER_INDEX 0x3c4

#define _GFXTK_PORT_SEQUENCER_DATA 0x3c5

#define _GFXTK_INDEX_PLANEMASK 0x02


void _GfxTk_SetPlaneMask(U8 planeMask) {
  PortWriteByte(_GFXTK_PORT_SEQUENCER_INDEX, _GFXTK_INDEX_PLANEMASK);
  PortWriteByte(_GFXTK_PORT_SEQUENCER_DATA, planeMask);
}


void _GfxTk_PauseUntilVSync(void) {
  while (!(PortReadByte(_GFXTK_PORT_VGASTATUS) & 0x08))
    ;
}



#define _GFXTK_PORT_DAC_INDEX 0x3c8

#define _GFXTK_PORT_DAC_DATA 0x3c9


void _GfxTk_SetPalette(U8 index, Rgb18 color) {
  // Trim to 6 bit
  U8 red = color.Red % 64;
  U8 green = color.Green % 64;
  U8 blue = color.Blue % 64;

  PortWriteByte(_GFXTK_PORT_DAC_INDEX, index);
  PortWriteByte(_GFXTK_PORT_DAC_DATA, red);
  PortWriteByte(_GFXTK_PORT_DAC_DATA, green);
  PortWriteByte(_GFXTK_PORT_DAC_DATA, blue);
}


void _GfxTk_UseColor(U8 index, U8 paletteIndex) {
  // Switch to index mode
  PortReadByte(_GFXTK_PORT_VGASTATUS);
  
  // Redefine mapping
  PortWriteByte(_GFXTK_PORT_ATTRIBUTE_INDEX, index & 0x1f);
  PortWriteByte(_GFXTK_PORT_ATTRIBUTE_INDEX, paletteIndex & 0xff);

  // Set video enable bit
  PortReadByte(_GFXTK_PORT_VGASTATUS);
  PortWriteByte(_GFXTK_PORT_ATTRIBUTE_INDEX, 0x20);
}




