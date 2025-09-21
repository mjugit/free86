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
#include "../../Kernel/Include/FontZXCourier.h"



extern U8* _BackBufferAddress;

__attribute__((unused))
void Gfx_DrawChar(U16 x, U16 y, char character, U8 color) {
  if ((U8)character < 0x20 || (U8)character > 0x7F)
    return;

  U16 glyph = (U8)character - 0x20;
  const U8* bitmap = &FONT_ZX_COURIER_BITMAP[glyph * 8];

  for (U16 row = 0; row < 8; row++) {
    U8 bits = bitmap[row];
    for (U16 col = 0; col < 8; col++) {
      if (bits & (1 << (7 - col))) {
	U16 px = x + col;
	U16 py = y + row;
	if (px >= 640 || py >= 480)
	  continue;

	U8 planeMask = 1;
	for (U8 plane = 0; plane < 4; plane++, planeMask <<= 1) {
	  U8 planeBit = (color & planeMask) ? 1 : 0;

	  U32 byte_offset = (py * 80) + (px / 8);
	  U8  bit_mask = 0x80 >> (px & 7);

	  U8* dest = _BackBufferAddress + (plane * 38400) + byte_offset;
	  if (planeBit)
	    *dest |= bit_mask;
	  else
	    *dest &= ~bit_mask;
	}
      }
    }
  }
}
