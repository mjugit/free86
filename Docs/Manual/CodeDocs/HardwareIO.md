# Hardware IO
This header provides low-level access to hardware I/O ports and CPU instructions on x86 systems. It exposes a set of inline functions that allow direct communication with external devices through port-mapped I/O as well as basic interrupt control. All functions are implemented as inline assembly to ensure maximum performance and correctness.

These routines are intended for use in the kernel and system modules that require precise control over the CPU or external hardware.

## Function reference

### `PortWriteByte`
Write a single byte to an I/O port.

```c
void PortWriteByte(U16 port, U8 value);
```

| Parameter | Description      |
| --------- | ---------------- |
| `port`    | I/O port address |
| `value`   | Byte to write    |


### `PortReadByte`
Read a single byte from an I/O port.

```c
U8 PortReadByte(U16 port);
```

| Parameter | Description      |
| --------- | ---------------- |
| `port`    | I/O port address |

| Returns | Description    |
| ------- | -------------- |
| `U8`    | Byte from port |


### `PortWriteWord`
Write a 16-bit word to an I/O port.

```c
void PortWriteWord(U16 port, U16 value);
```

| Parameter | Description      |
| --------- | ---------------- |
| `port`    | I/O port address |
| `value`   | Word to write    |


### `PortReadWord`
Read a 16-bit word from an I/O port.

```c
U16 PortReadWord(U16 port);
```

| Parameter | Description      |
| --------- | ---------------- |
| `port`    | I/O port address |

| Returns | Description    |
| ------- | -------------- |
| `U16`   | Word from port |


### `PortWriteDWord`
Write a 32-bit double word to an I/O port.

```c
void PortWriteDWord(U16 port, U32 value);
```

| Parameter | Description          |
| --------- | -------------------- |
| `port`    | I/O port address     |
| `value`   | Double word to write |


### `PortReadDWord`
Read a 32-bit double word from an I/O port.

```c
U32 PortReadDWord(U16 port);
```

| Parameter | Description      |
| --------- | ---------------- |
| `port`    | I/O port address |

| Returns | Description           |
| ------- | --------------------- |
| `U32`   | Double word from port |


### `PortReadBytes`
Read multiple bytes from an I/O port into a buffer.

```c
void PortReadBytes(U16 port, void *buffer, U32 count);
```

| Parameter | Description             |
| --------- | ----------------------- |
| `port`    | I/O port address        |
| `buffer`  | Pointer to destination  |
| `count`   | Number of bytes to read |


### `PortWriteBytes`
Write multiple bytes from a buffer to an I/O port.

```c
void PortWriteBytes(U16 port, const void *buffer, U32 count);
```

| Parameter | Description              |
| --------- | ------------------------ |
| `port`    | I/O port address         |
| `buffer`  | Pointer to source buffer |
| `count`   | Number of bytes to write |


### `PortReadWords`
Read multiple 16-bit words from an I/O port into a buffer.

```c
void PortReadWords(U16 port, void *buffer, U32 count);
```

| Parameter | Description             |
| --------- | ----------------------- |
| `port`    | I/O port address        |
| `buffer`  | Pointer to destination  |
| `count`   | Number of words to read |


### `PortWriteWords`
Write multiple 16-bit words from a buffer to an I/O port.

```c
void PortWriteWords(U16 port, const void *buffer, U32 count);
```

| Parameter | Description              |
| --------- | ------------------------ |
| `port`    | I/O port address         |
| `buffer`  | Pointer to source buffer |
| `count`   | Number of words to write |


### `EnableInterrupts`
Enable CPU hardware interrupts.

```c
void EnableInterrupts(void);
```

### `DisableInterrupts`
Disable CPU hardware interrupts.

```c
void DisableInterrupts(void);
```

