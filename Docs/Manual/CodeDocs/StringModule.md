# String module
The String module provides a small set of core string manipulation utilities for use throughout the system. Since free86 operates in a freestanding environment without access to the standard C library, these functions implement essential text-handling features such as formatting, length calculation, and character order reversal.

## Introduction
C strings are simple arrays of characters terminated by a null byte (`'\0'`). While the concept is straightforward, the lack of higher-level string operations in freestanding environments requires dedicated implementations.
This module offers lightweight routines that are safe to use in low-level code and kernel components, avoiding unnecessary dependencies or heap allocations.

The focus is on predictable performance and direct control — each function operates on raw pointers and assumes the caller provides valid memory regions for input and output.


## Using the module
To use the string module in your code, include its header and import the module instance with the `use(...)` macro:

```c
#include "Modules/Include/String.h"
use(String);
```
This makes the global `String` namespace available in the current translation unit. You can then call its functions directly:

```c
char buffer[64];
String.Format(buffer, "Memory used: %d KB", 128);
String.Reverse(buffer);
U32 length = String.GetLength(buffer);
```



## Data types

### `string`
The string type is defined as a simple alias for a C-style character pointer:

```c
typedef char* string;
```

The type itself does not introduce ownership or allocation behavior; it merely represents a pointer to an existing memory region that contains a readable and writable string.
For example:

```c
string text = "Hello, world!";
U32 length = String.GetLength(text);
```



## Function reference

### `Format`
Writes formatted text into a destination buffer using a format string and variadic arguments. This function is similar to `sprintf()` from the C standard library but limited to a minimal subset of format specifiers supported by free86.

```c
void Format(string destination, const string formatStr, ...);
```

| Parameter     | Description                                        |
| ------------- | -------------------------------------------------- |
| `destination` | Pointer to the target buffer to write into         |
| `formatStr`   | The format string containing text and placeholders |
| `...`         | Variable argument list of values to insert         |



### `Reverse`
Reverses the order of characters in a given null-terminated string in place.

```c
void Reverse(string pointer);
```

Reverses the order of characters in a given null-terminated string in place.

| Parameter | Description                      |
| --------- | -------------------------------- |
| `pointer` | Pointer to the string to reverse |


### `GetLength`
Calculates the length of a null-terminated string, excluding the terminator itself.


```c
U32 GetLength(string pointer);
```

| Parameter | Description                      |
| --------- | -------------------------------- |
| `pointer` | Pointer to the string to measure |

