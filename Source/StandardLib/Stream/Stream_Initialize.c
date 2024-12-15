#include "../Include/Stream.h"

/**
 * @brief Initializes a stream structure by resetting its indexes and clearing its buffer.
 *
 * This function sets both the write and read indexes of the stream to zero
 * and clears the stream buffer by filling it with zeroes.
 *
 * @param stream A pointer to the `Stream` structure to initialize.
 */
void Stream_Initialize(Stream *stream) {
    // Reset write and read indexes
    stream->WriteIndex = stream->ReadIndex = 0;

    // Clear the buffer
    for (int buffIndex = 0; buffIndex < STREAM_BUFFER_SIZE; buffIndex++) {
        stream->Buffer[buffIndex] = 0;
    }
}
