#include "../Include/Stream.h"

static inline void DisableInterrupts(void) {
  __asm__ volatile ("cli");
}

static inline void EnableInterrupts(void) {
  __asm__ volatile ("sti");
}

char Stream_Read(Stream *stream, char *nextByte) {
  if (stream->ReadIndex == stream->WriteIndex)
    return 0;
  
  DisableInterrupts();
  *nextByte = stream->Buffer[stream->ReadIndex++];
  EnableInterrupts();

  return 1;
}
