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


#include "../Include/Stream.h"

use(Stream);


U16 _Stream_ReadImplementation(stream* stream, void* buffer, U16 size) {
  U8* bufferPtr = (U8*)buffer;
  U16 bytesRead = 0;

  for (U16 bufferIndex = 0; bufferIndex < size; bufferIndex++) {
    if (stream->ReadIndex == stream->WriteIndex)
      return bytesRead;

    bufferPtr[bufferIndex] = stream->Buffer[stream->ReadIndex];
    bytesRead++;
    stream->ReadIndex = (stream->ReadIndex + 1) % __STREAM_BUFFERSIZE;
  }

  return bytesRead;
}
