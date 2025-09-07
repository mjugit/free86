# Bitmap module
This module provides a compact and efficient way to manage sets of binary flags using bit-level operations. It exposes a consistent interface that can be used across the system for tasks like allocation tracking and resource management.


## Introduction
A bitmap is one of the most fundamental data structures in low-level system programming. At its core, it represents a compact array of bits, where each bit corresponds to a binary state — set or cleared. This makes bitmaps extremely memory-efficient for tracking large numbers of simple conditions.

Typical use cases include:
- Managing allocation tables (e.g., free and used blocks in memory or storage).
- Tracking resource availability (such as process slots, device handles, or descriptors).
- Representing feature flags or state masks in a compact form.

Because each bit can be accessed individually, bitmaps are a natural fit for operating systems, where efficient use of limited resources is critical. On hardware with only a few hundred kilobytes of RAM available, such compactness is not just convenient but essential.


## Using the module
To use the bitmap module in your code, include its header and import the module instance with the `use(...)` macro:

```c
// Includes the namespace definition
#include "Modules/Include/Bitmap.h"

// Makes the module available
use(Bitmap);
```

This makes the global Bitmap namespace available in the current translation unit. You can then call its functions directly, for example:

```c
// Set bit 5
Bitmap.Set(myBitmap, 5);

// Check if bit 12 is set
if (Bitmap.Check(myBitmap, 12)) {
    // ...
}
```

This usage pattern is consistent with all other modules in free86.


## Function reference
The following functions form the public interface of the bitmap module. All operations assume that the provided memory region is large enough to contain the requested bits.

### `Set`
Set a single bit in the bitmap.

```c
void Set(void *bitmap, U32 bit);
```

| Parameter | Description                        |
|-----------|------------------------------------|
| `bitmap`  | Pointer to the bitmap storage      |
| `bit`     | Zero-based index of the bit to set |


### `Clear`
Clear a single bit in the bitmap.

```c
void Clear(void *bitmap, U32 bit);
```

| Parameter | Description                          |
| --------- | ------------------------------------ |
| `bitmap`  | Pointer to the bitmap storage        |
| `bit`     | Zero-based index of the bit to clear |


### `Check`
Check whether a specific bit is set.

```c
bool Check(void *bitmap, U32 bit);
```

| Parameter | Description                         |
| --------- | ----------------------------------- |
| `bitmap`  | Pointer to the bitmap storage       |
| `bit`     | Zero-based index of the bit to test |


| Returns | Description         |
| ------- | ------------------- |
| `true`  | If the bit is set   |
| `false` | If the bit is clear |


### `Wipe`
Clear multiple bits starting from index `0`. Commonly used to reset the entire bitmap.

```c
void Wipe(void *bitmap, U32 bitCount);
```

| Parameter  | Description                            |
| ---------- | -------------------------------------- |
| `bitmap`   | Pointer to the bitmap storage          |
| `bitCount` | Amount of bits to clear                |


### `NextFree`
Find the index of the next free range of bits of the given length.

```c
I32 NextFree(void *bitmap, U32 bitsFree, U32 size);
```

| Parameter  | Description                             |
| ---------- | --------------------------------------- |
| `bitmap`   | Pointer to the bitmap storage           |
| `bitsFree` | Number of contiguous free bits required |
| `size`     | Total number of bits in the bitmap      |

| Returns | Description                           |
| ------- | ------------------------------------- |
| `>= 0`  | Starting index of the next free range |
| `-1`    | No free range found                   |

