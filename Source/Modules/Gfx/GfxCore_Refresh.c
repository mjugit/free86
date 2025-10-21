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

use(GfxCore);


extern GfxData _Gfx_Data;
static void _Vram_Copy(void *dest, const void *src, U32 count);



void _GfxCore_RefreshImplementation(void) {
  // Skip if not initialized
  if (!_Gfx_Data.BackBufferAddress)
    return;

  GfxCore.SetBitMask(0xff);
  GfxCore.WaitForVSync();

  U32 bytesPerPlane = (_Gfx_Data.ScreenWidth / 8) * _Gfx_Data.ScreenHeight;

  for (U8 plane = 0; plane < _Gfx_Data.NumberOfPlanes; plane++) {
    GfxCore.SetPlaneMask(1 << plane);
    U8* buffer = _Gfx_Data.BackBufferAddress + (bytesPerPlane * plane);
    _Vram_Copy(_Gfx_Data.VRamAddress, buffer, bytesPerPlane);
  }
}



static void _Vram_Copy(void *dest, const void *src, U32 count) {
  unsigned int dwords = count / 4;
  unsigned int bytes  = count % 4;

  __asm__ __volatile__ (
			"cld\n\t" 
			"rep movsd\n\t" 
			: "+D"(dest), "+S"(src), "+c"(dwords)
			:
			: "memory"
			);

  __asm__ __volatile__ (
			"movl %2, %%ecx\n\t"
			"rep movsb\n\t"
			: "+D"(dest), "+S"(src)
			: "r"(bytes)
			: "ecx", "memory"
			);
}
