# Memory module


## Introduction
The memory module provides a set of basic operations for working with raw memory blocks. These functions form the low-level foundation for higher-level subsystems and are essential. The implementations are optimized using assembler to improve performance.


## Using the module

To use the memory module in your code, include its header and import the module instance with the `use(...)` macro:

```c
// Includes the namespace definition
#include "Modules/Include/Memory.h"

// Makes the module available
use(Memory);
```

This makes the `Memory` namespace available in the current translation unit:

```c
// Copy 64 bytes from src to dst
Memory.Copy(dst, src, 64);

// Fill buffer with zero
Memory.Set(buffer, 0, 128);
```

This usage pattern is consistent with all other modules in free86.


## Function reference
The following functions form the public interface of the memory module. All operations assume that the provided memory regions are valid and large enough for the requested operations.


### `Copy`
Copy a block of memory from source to destination. Does not protect against overlapping regions.

```c
void Copy(void *destination, const void *source, U32 count);
```

| Parameter     | Description                    |
| ------------- | ------------------------------ |
| `destination` | Target address for copied data |
| `source`      | Source address to copy from    |
| `count`       | Number of bytes to copy        |


Use `Move` instead if the source and destination regions may overlap.


### `Set`
Fill a block of memory with a specific byte value.

```c
void Set(void *destination, U8 value, U32 count);
```

| Parameter     | Description                       |
| ------------- | --------------------------------- |
| `destination` | Start address of the memory block |
| `value`       | Byte value to write               |
| `count`       | Number of bytes to fill           |


### `Move`
Copy memory safely between potentially overlapping regions.

```c
void Move(void *source, const void *destination, U32 count);
```

| Parameter     | Description                       |
| ------------- | --------------------------------- |
| `source`      | Source address (may overlap)      |
| `destination` | Destination address (may overlap) |
| `count`       | Number of bytes to move           |


Internally ensures correct order of copying if regions overlap.


### `Compare`
Compare two memory blocks byte by byte.

```c
I32 Compare(const void *a, const void *b, U32 count);
```

| Parameter | Description                        |
| --------- | ---------------------------------- |
| `a`       | Pointer to the first memory block  |
| `b`       | Pointer to the second memory block |
| `count`   | Number of bytes to compare         |


| Returns | Description                        |
| ------- | ---------------------------------- |
| `0`     | Blocks are equal                   |
| `!= 0`  | Difference found (value-dependent) |

The return value indicates how the first differing byte compares.


### `Find`
Find the first occurrence of a byte value in a memory block.

```c
void* Find(const void *block, U8 value, U32 count);
```

| Parameter | Description                 |
| --------- | --------------------------- |
| `block`   | Pointer to the memory block |
| `value`   | Byte value to search for    |
| `count`   | Number of bytes to search   |


| Returns   | Description                        |
| --------- | ---------------------------------- |
| `!= NULL` | Pointer to the first matching byte |
| `NULL`    | Value not found in given range     |

