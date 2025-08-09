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

#ifndef _BITMAP_H_
#define _BITMAP_H_

struct __Bitmap_Namespace {

  // Set a single bit in a bitmap
  void (*Set)(void *bitmap, unsigned int bit);

  // Clear a single bit in a bitmap
  void (*Clear)(void *bitmap, unsigned int bit);

  // Check whether a specific bit is set
  int (*Check)(void *bitmap, unsigned int bit);

  // Clear multiple bits in a bitmap
  void (*Wipe)(void *bitmap, unsigned int bitCount);

  // Find the index of the next free bit
  int (*NextFree)(unsigned char *bitmap, unsigned int bitsFree, unsigned int size);
  
};


extern const struct __Bitmap_Namespace Bitmap;


#endif
