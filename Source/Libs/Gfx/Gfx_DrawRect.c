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


// Draws a non-filled rectangle
__attribute__((unused))
void Gfx_DrawRect(U16 posX, U16 posY, U16 width, U16 height, U8 colorIndex) {
  if (width == 0 || height == 0)
    return;
    
  // Top edge
  for (U16 x = posX; x < posX + width; x++) 
    Gfx.Draw.Pixel(x, posY, colorIndex);
    
  // Bottom edge
  for (U16 x = posX; x < posX + width; x++) 
    Gfx.Draw.Pixel(x, posY + height - 1, colorIndex);
    
  // Left edge
  for (U16 y = posY; y < posY + height; y++) 
    Gfx.Draw.Pixel(posX, y, colorIndex);
    
  // Right edge
  for (U16 y = posY; y < posY + height; y++) 
    Gfx.Draw.Pixel(posX + width - 1, y, colorIndex);
}
