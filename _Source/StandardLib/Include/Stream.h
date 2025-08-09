#ifndef _STREAM_H_
#define _STREAM_H_

#include "CoreTypes.h"

/**
 * @brief The size of the stream buffer.
 *
 * This constant defines the maximum number of characters the stream buffer
 * can hold. It determines the size of the `Buffer` array in the `Stream` structure.
 */
#define STREAM_BUFFER_SIZE 256


/**
 * @brief Represents a data stream with a circular buffer.
 *
 * This structure is used to manage a stream of data, supporting sequential
 * writing and reading with circular indexing. The structure consists of:
 * - `WriteIndex`: Tracks where the next write operation will occur.
 * - `ReadIndex`: Tracks where the next read operation will occur.
 * - `Buffer`: The array that holds the data for the stream.
 */
typedef struct {
  unsigned short WriteIndex;        ///< Index for the next write operation.
  unsigned short ReadIndex;         ///< Index for the next read operation.
  char Buffer[STREAM_BUFFER_SIZE];  ///< The data buffer.
} Stream;


/**
 * @brief Initializes a stream structure by resetting its indexes and clearing its buffer.
 *
 * This function sets both the write and read indexes of the stream to zero
 * and clears the stream buffer by filling it with zeroes.
 *
 * @param stream A pointer to the `Stream` structure to initialize.
 */
extern void Stream_Initialize(Stream *stream);


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
extern void Stream_Write(Stream *stream, char byte);


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
extern char Stream_Read(Stream *stream, char *nextByte);


#endif
