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


#ifndef __STREAM_H__
#define __STREAM_H__

#include "SystemCore.h"

#define __STREAM_BUFFERSIZE 256

typedef struct stream {
  U8 Buffer[__STREAM_BUFFERSIZE];
  
  U16 ReadIndex;
  U16 WriteIndex;
} stream;



module(Stream) {

  // Return the amount of pending bytes in the buffer
  U16 (*GetPendingBytes)(stream* stream);

  // Write data to a stream (returns the amount of bytes written)
  U16 (*Write)(stream* stream, void* data, U16 size);

  // Read data from a stream (returns the amount of bytes read)
  U16 (*Read)(stream* stream, void* buffer, U16 size);
  
};


#endif
