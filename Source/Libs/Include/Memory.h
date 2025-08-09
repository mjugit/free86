#ifndef _MEMORY_H_
#define _MEMORY_H_


typedef struct _Memory_Namespace MemoryNamespace;


/*
  Represents a the namespace for low level memory operations. The
  actual implementation is in assembler, since it is far easier to
  optimize.
 */

struct _Memory_Namespace {
  
  // Copy a block of memory without overlap protection
  void (*Copy)(void *destination, const void *source, unsigned int count);

  // Flood a block of memory with a certain value
  void (*Set)(void *destination, unsigned char value, unsigned int count);

  // Copy a block of memory with overlap protection
  void (*Move)(void *destination, const void *source, unsigned int count);

  // Compare two blocks of memory
  void (*Compare)(const void *a, const void *b, unsigned int count);

  // Find the first occurrence of a value in a block of memory
  void* (*Find)(const void *block, unsigned char value, unsigned int count);
  
};


/*
  As stated above, the actual namespace definition is a struct
  definition in assembler. Direct access to the concrete
  implementation is not foreseen; use this constant to access
  functions via pointers.
 */

extern const MemoryNamespace Memory;

#endif
