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


/*
  Find the index of the next run of free bits.
  Returns the start index of the first run of `bitsFree` consecutive free (0) bits
  within the first `bitCount` bits of `bitmap`, or -1 if none exists.
  Matches the assembly namespace table entry: _Bitmap_NextFree
*/

int _Bitmap_NextFree(unsigned char *bitmap, unsigned int bitsFree, unsigned int bitCount) {
  // Handle degenerate cases

  if (bitsFree == 0)
    return 0;

  if (bitmap == 0 || bitCount == 0 || bitsFree > bitCount)
    return -1;

  // Initialize state

  unsigned int runStart = 0;
  unsigned int runLen   = 0;

  // Scan bits

  for (unsigned int index = 0; index < bitCount; index++) {
    unsigned int byteIndex = index >> 3;          // Byte offset
    unsigned int bitInByte = index & 7;           // Bit position within byte
    unsigned char byte     = bitmap[byteIndex];

    // Check current bit (0 = free)

    unsigned char occupied = (unsigned char)((byte >> bitInByte) & 1U);
    if (occupied == 0) {
      if (runLen == 0)
        runStart = index;
      runLen++;
      if (runLen >= bitsFree)
        return (int)runStart;
    } else {
      runLen = 0;
    }
  }

  // Not found

  return -1;
}
