# Stream module


## Using the module
To use the heap module in your code, include its header and import the module instance with the `use(...)` macro:

```c
// Includes the namespace definition
#include "Modules/Include/Stream.h"

// Makes the module available
use(Stream);
```

This makes the global Stream namespace available in the current translation unit. You can then call its functions directly, for example:

```c
// Copy 1234 bytes (at most) from myStream to destBuffer
Stream.Read(&myStream, destBuffer, 1234);
```

This usage pattern is consistent with all other modules in free86.


## Macro reference

### `__STREAM_BUFFERSIZE`
Holds the maximum amount of bytes that any stream will buffer. This value defaults to 256.


## Function reference

### `GetPendingBytes`
Returns the amount of unread bytes in the stream buffer.

```c
U16 GetPendingbytes(stream* streamPtr);
```

| Parameter   | Description                          |
|-------------|--------------------------------------|
| `streamPtr` | A pointer to the stream to evaluate. |

| Returns | Description                                     |
|---------|-------------------------------------------------|
| numeric | The total amount of unread bytes in the buffer. |


### `Write`
Write data to the stream. Bytes are written until the stream buffer is full
or there is no data left to write (size limit).

```c
U16 Write(stream* streamPtr, void* dataPtr, U16 size);
```

| Parameter   | Description                          |
|-------------|--------------------------------------|
| `streamPtr` | A pointer to the stream.             |
| `dataPtr`   | A pointer to the data to write.      |
| `size`      | The amount of bytes to write at max. |

| Returns | Description                        |
|---------|------------------------------------|
| numeric | The total amount of bytes written. |


### `Read`
Read data from the stream. Bytes are read until the stream buffer is empty
or there is no data left to read (size limit).

```c
U16 Read(stream* streamPtr, void* buffer, U16 size);
```

| Parameter   | Description                          |
|-------------|--------------------------------------|
| `streamPtr` | A pointer to the stream.             |
| `buffer`    | A pointer to the destination buffer. |
| `size`      | The amount of bytes to read at max.  |

| Returns | Description                     |
|---------|---------------------------------|
| numeric | The total amount of bytes read. |
