#ifndef _KERNEL_MEMORY_H_
#define _KERNEL_MEMORY_H_

#include <stdint.h>
#include "../../StandardLib/Include/CoreTypes.h"

/**
 * @brief Initializes the kernel memory manager.
 *
 * Sets up the memory manager by allocating a bitmap to track memory block usage
 * and preparing the heap for allocations.
 *
 * @param heapStart The starting address of the heap.
 * @param heapSize  The total size of the heap in bytes.
 */
extern void KMem_Initialize(void *heapStart, uint32_t heapSize);


/**
 * @brief Allocates a chunk of memory from the heap.
 *
 * Finds a contiguous block of memory of at least the requested size and marks it as allocated.
 *
 * @param size The size of the memory to allocate, in bytes.
 * @return Pointer to the allocated memory, or NULL if allocation fails.
 */
extern void *KMem_Allocate(uint32_t size);


/**
 * @brief Frees a previously allocated chunk of memory.
 *
 * Marks the memory blocks starting at the specified pointer as free.
 *
 * @param chunk A pointer to the memory chunk to free.
 */
extern void KMem_Free(void *chunk);


/**
 * @brief Returns the total amount of free memory.
 *
 * Iterates through the allocation bitmap and calculates the total amount of unused memory.
 *
 * @return The number of free bytes in the heap.
 */
extern uint32_t KMem_GetBytesFree(void);


#endif
