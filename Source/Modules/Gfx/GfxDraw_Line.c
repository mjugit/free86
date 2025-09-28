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


// Draws a line using Bresenham's algorithm
void _GfxDraw_LineImplementation(U16 startX, U16 startY, U16 endX, U16 endY, U8 colorIndex) {
  I16 dx = (endX > startX)
    ? (endX - startX)
    : (startX - endX);
    
  I16 sx = (startX < endX)
    ? 1
    : -1;
    
  I16 dy = (endY > startY)
    ? (startY - endY)
    : (endY - startY);
    
  I16 sy = (startY < endY)
    ? 1
    : -1;
    
  I16 err = dx + dy;
    
  while (1) {
    GfxDraw.Pixel(startX, startY, colorIndex);
    if (startX == endX && startY == endY)
      break;
	
    I16 e2 = 2 * err;
    if (e2 >= dy){
      err += dy;
      startX += sx;
    }
    
    if (e2 <= dx) {
      err += dx;
      startY += sy;
    }
  }
}
