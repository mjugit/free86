#include "../Include/Stream.h"

/**
 * @brief Writes a single byte to the stream buffer.
 *
 * This function stores the given byte at the current `WriteIndex` in the stream buffer
 * and increments the `WriteIndex`, wrapping around if it reaches the end of the buffer
 * (circular buffer behavior). It does not handle overwriting unread data; callers must
 * ensure that writes do not exceed the buffer capacity.
 *
 * @param stream A pointer to the `Stream` structure to write to.
 * @param byte The byte to write into the stream buffer.
 */
void Stream_Write(Stream *stream, char byte) {
    // Write the byte to the current write index
    stream->Buffer[stream->WriteIndex] = byte;

    // Increment the write index, wrapping around if necessary
    stream->WriteIndex = (stream->WriteIndex + 1) % STREAM_BUFFER_SIZE;
}
