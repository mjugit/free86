#include "../Include/Stream.h"

void Stream_Initialize(Stream *stream) {
  stream->WriteIndex = stream->ReadIndex = 0;

  for (int buffIndex = 0; buffIndex < STREAM_BUFFER_SIZE; buffIndex++)
    stream->Buffer[buffIndex] = 0;
}
