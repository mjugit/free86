#include "../Include/Stream.h"

/**
 * @brief Disables interrupts to ensure atomic operations on shared data.
 *
 * This inline function executes the `cli` instruction to disable hardware interrupts,
 * preventing race conditions or data corruption during critical operations.
 */
static inline void _Stream_DisableInterrupts(void) {
  __asm__ volatile ("cli");
}


/**
 * @brief Enables interrupts after atomic operations are complete.
 *
 * This inline function executes the `sti` instruction to re-enable hardware interrupts
 * once critical operations are finished.
 */
static inline void _Stream_EnableInterrupts(void) {
  __asm__ volatile ("sti");
}


/**
 * @brief Reads a single byte from the stream buffer.
 *
 * This function retrieves the next byte from the stream buffer. If the buffer
 * is empty (i.e., the read and write indexes are equal), it returns `0` and
 * does not modify the output parameter. Otherwise, it reads the byte at the
 * current `ReadIndex`, increments the `ReadIndex`, and returns `1`.
 *
 * Interrupts are temporarily disabled during the read operation to prevent
 * concurrent modifications to the buffer.
 *
 * @param stream A pointer to the `Stream` structure from which to read.
 * @param nextByte A pointer to a character where the read byte will be stored.
 * @return `1` if a byte was successfully read, or `0` if the buffer is empty.
 */
char Stream_Read(Stream *stream, char *nextByte) {
  if (stream->ReadIndex == stream->WriteIndex)
    return 0;
  
  _Stream_DisableInterrupts();
  *nextByte = stream->Buffer[stream->ReadIndex++];
  _Stream_EnableInterrupts();

  return 1;
}
