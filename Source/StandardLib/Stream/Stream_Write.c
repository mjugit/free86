#include "../Include/Stream.h"

void Stream_Write(Stream *stream, char byte) {
  stream->Buffer[stream->WriteIndex] = byte;
  stream->WriteIndex = (stream->WriteIndex + 1) % STREAM_BUFFER_SIZE;
}
