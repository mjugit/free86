# Heap module
Dynamic memory can be reserved and released by other parts of the program at runtime, which greatly simplifies programming and efficient memory usage. This module hides the details and provides a simple API for use in production code.


## Introduction
Dynamic memory refers to memory that is allocated and freed at runtime, rather than being fixed at compile time. It allows programs to request exactly as much memory as they need, when they need it — which is especially useful when dealing with data structures whose size cannot be known in advance. Without dynamic memory, a system would have to reserve static memory blocks for every possible case, leading to inefficient use of the limited memory available.

## Using the module
To use the heap module in your code, include its header and import the module instance with the `use(...)` macro:

```c
// Includes the namespace definition
#include "Modules/Include/Heap.h"

// Makes the module available
use(Heap);
```

This makes the global Heap namespace available in the current translation unit. You can then call its functions directly, for example:

```c
// Initializes a dynamic memory area at address 0x1234 of 320 * 1024 bytes
HeapMemory* heapArea = Heap.Initialize((void*)0x1234, 320 * 1024);
```

This usage pattern is consistent with all other modules in free86.


## Function reference


### `Initialize`
Prepares a dynamic memory area for later use. This involves creating a header that is used to manage the area and define the first blocks.

```c
HeapMemory* Initialize(void* pointer, U32 totalSize);
```

| Parameter   | Description                        |
|-------------|------------------------------------|
| `pointer`   | Pointer to the dynamic memory area |
| `totalSize` | The total amount of bytes to use   |


### `Allocate`
Allocate a free memory area of a specific size.

```c
void* Allocate(HeapMemory* heapArea, U32 size);
```

| Parameter   | Description                               |
|-------------|-------------------------------------------|
| `heapArea`  | A pointer to the heap area to use         |
| `totalSize` | The total amount of bytes to allocate use |

| Returns | Description         |
| ------- | ------------------- |
| `null`  | There are not enough free bytes or no free continuous memory area of the requested size  |
| pointer | The pointer to the reserved memory area |


### `Free`
Free up a reserved dynamic memory area.

```c
void Free(HeapMemory* heapArea, void* pointer);
```

| Parameter  | Description                          |
|------------|--------------------------------------|
| `heapArea` | A pointer to the heap area to use    |
| `pointer`  | A pointer to the memory area to free |


### `GetBytesFree`
Get the amount of free bytes in the dynamic memory area. Note that allocations can also fail with sufficient free bytes available when no contiguous memory area can be found.

```c
U32 GetBytesFree(HeapMemory* heapArea);
```

| Parameter  | Description                          |
|------------|--------------------------------------|
| `heapArea` | A pointer to the heap area to use    |

| Returns | Description                                 |
|---------|---------------------------------------------|
| numeric | The total amount of bytes marked as 'free'. |


### `GetBytesUsed`
Get the amount of allocated bytes in the dynamic memory area.

```c
U32 GetBytesUsed(HeapMemory* heapArea);
```

| Parameter  | Description                          |
|------------|--------------------------------------|
| `heapArea` | A pointer to the heap area to use    |

| Returns | Description                                 |
|---------|---------------------------------------------|
| numeric | The total amount of bytes marked as 'used'. |
