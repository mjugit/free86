/*
	
  Copyright © 2025 Maximilian Jung

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the “Software”), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
*/

#ifndef _HARDWARE_IO_H_
#define _HARDWARE_IO_H_

#include "CoreTypes.h"



// Writes a byte to the specified I/O port
__attribute__((unused))
static inline void PortWriteByte(U16 port, U8 value) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Reads a byte from the specified I/O port
__attribute__((unused))
static inline U8 PortReadByte(U16 port) {
    U8 value;
    __asm__ volatile ("inb %1, %0" : "=a" (value) : "dN" (port));
    return value;
}

// Writes a word to the specified I/O port
__attribute__((unused))
static inline void PortWriteWord(U16 port, U16 value) {
    __asm__ volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

// Reads a word from the specified I/O port
__attribute__((unused))
static inline U16 PortReadWord(U16 port) {
    U16 value;
    __asm__ volatile ("inw %1, %0" : "=a" (value) : "dN" (port));
    return value;
}

// Writes a double word to the specified I/O port
__attribute__((unused))
static inline void PortWriteDWord(U16 port, U32 value) {
    __asm__ volatile ("outl %1, %0" : : "dN" (port), "a" (value));
}

// Reads a double word from the specified I/O port
__attribute__((unused))
static inline U32 PortReadDWord(U16 port) {
    U32 value;
    __asm__ volatile ("inl %1, %0" : "=a" (value) : "dN" (port));
    return value;
}

// Reads multiple bytes from an I/O port into a buffer
__attribute__((unused))
static inline void PortReadBytes(U16 port, void *buffer, U32 count) {
    __asm__ volatile ("rep insb"
                      : "=D"(buffer), "=c"(count)
                      : "d"(port), "0"(buffer), "1"(count));
}

// Writes multiple bytes from a buffer to an I/O port
__attribute__((unused))
static inline void PortWriteBytes(U16 port, const void *buffer, U32 count) {
    __asm__ volatile ("rep outsb"
                      : "=S"(buffer), "=c"(count)
                      : "d"(port), "0"(buffer), "1"(count));
}

// Reads multiple words from an I/O port into a buffer
__attribute__((unused))
static inline void PortReadWords(U16 port, void *buffer, U32 count) {
    __asm__ volatile ("rep insw"
                      : "=D"(buffer), "=c"(count)
                      : "d"(port), "0"(buffer), "1"(count));
}

// Writes multiple words from a buffer to an I/O port
__attribute__((unused))
static inline void PortWriteWords(U16 port, const void *buffer, U32 count) {
    __asm__ volatile ("rep outsw"
                      : "=S"(buffer), "=c"(count)
                      : "d"(port), "0"(buffer), "1"(count));
}



// Writes an 8-bit value to a memory-mapped I/O address
__attribute__((unused))
static inline void MmioWrite8(volatile void *address, U8 value) {
    *(volatile U8 *)address = value;
}

// Reads an 8-bit value from a memory-mapped I/O address
__attribute__((unused))
static inline U8 MmioRead8(volatile const void *address) {
    return *(volatile const U8 *)address;
}

// Writes a 16-bit value to a memory-mapped I/O address
__attribute__((unused))
static inline void MmioWrite16(volatile void *address, U16 value) {
    *(volatile U16 *)address = value;
}

// Reads a 16-bit value from a memory-mapped I/O address
__attribute__((unused))
static inline U16 MmioRead16(volatile const void *address) {
    return *(volatile const U16 *)address;
}

// Writes a 32-bit value to a memory-mapped I/O address
__attribute__((unused))
static inline void MmioWrite32(volatile void *address, U32 value) {
    *(volatile U32 *)address = value;
}

// Reads a 32-bit value from a memory-mapped I/O address
__attribute__((unused))
static inline U32 MmioRead32(volatile const void *address) {
    return *(volatile const U32 *)address;
}



// Enables CPU interrupts
__attribute__((unused))
static inline void EnableInterrupts(void) {
    __asm__ volatile ("sti");
}

// Disables CPU interrupts
__attribute__((unused))
static inline void DisableInterrupts(void) {
    __asm__ volatile ("cli");
}

// Saves current interrupt flags and disables interrupts
__attribute__((unused))
static inline U32 SaveAndDisableInterrupts(void) {
    U32 flags;
    __asm__ volatile ("pushf\n\t"
                      "pop %0\n\t"
                      "cli"
                      : "=r"(flags));
    return flags;
}

// Restores interrupt flags
__attribute__((unused))
static inline void RestoreInterrupts(U32 flags) {
    __asm__ volatile ("push %0\n\t"
                      "popf"
                      : : "r"(flags));
}


#endif
