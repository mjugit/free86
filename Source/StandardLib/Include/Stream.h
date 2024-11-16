#ifndef _STREAM_H_
#define _STREAM_H_

#include "CoreTypes.h"

#define STREAM_BUFFER_SIZE 256


typedef struct {
  unsigned short WriteIndex;
  unsigned short ReadIndex;
  char Buffer[STREAM_BUFFER_SIZE];
} Stream;


/**
 * Initialize the properties of a stream (this is equivalent to a total reset).
 * @param stream A pointer to the stream which to manipulate.
 */
extern void Stream_Initialize(Stream *stream);

/**
 * Write a single byte to a stream.
 * @param stream A pointer to the stream on which the data shall be written.
 * @param byte The byte to write.
 */
extern void Stream_Write(Stream *stream, char byte);

/**
 * Read a single byte from a stream.
 * @param stream A pointer to the stream from which to write the data.
 * @param nextByte Destination pointer.
 * @returns The amount of bytes read (0 or 1);
 */
extern char Stream_Read(Stream *stream, char *nextByte);


#endif
