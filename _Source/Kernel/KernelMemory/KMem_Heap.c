/**
 * @file KernelMemory.c
 * @brief Simple heap memory allocator for the kernel.
 *
 * This module provides basic memory management functions, including initialization,
 * allocation, deallocation, and querying free memory. It uses a bitmap to manage
 * fixed-size memory blocks for simplicity and efficiency.
 */

#include "../Include/KernelMemory.h"
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Size of each memory block in bytes.
 */
#define KMEM_BLOCK_SIZE 64

/**
 * @brief Start address of the heap.
 */
static void *_HeapStart;


/**
 * @brief End address of the heap.
 */
static void *_HeapEnd;


/**
 * @brief Bitmap to track allocated and free blocks.
 */
static uint8_t *_AllocationBitmap;


/**
 * @brief Total number of memory blocks in the heap.
 */
static uint32_t _TotalBlocks;


/**
 * @brief Macro to set a specific bit in the bitmap.
 */
#define SET_BIT(bitmap, n)    ((bitmap)[(n) / 8] |= (1 << ((n) % 8)))


/**
 * @brief Macro to clear a specific bit in the bitmap.
 */
#define CLEAR_BIT(bitmap, n)  ((bitmap)[(n) / 8] &= ~(1 << ((n) % 8)))


/**
 * @brief Macro to check if a specific bit is set in the bitmap.
 */
#define IS_BIT_SET(bitmap, n) ((bitmap)[(n) / 8] & (1 << ((n) % 8)))


/**
 * @brief Initializes the kernel memory manager.
 *
 * Sets up the memory manager by allocating a bitmap to track memory block usage
 * and preparing the heap for allocations.
 *
 * @param heapStart The starting address of the heap.
 * @param heapSize  The total size of the heap in bytes.
 */
void KMem_Initialize(void *heapStart, uint32_t heapSize) {
    _HeapStart = heapStart;
    _HeapEnd = (uint8_t *)heapStart + heapSize;

    _TotalBlocks = heapSize / KMEM_BLOCK_SIZE;
    uint32_t bitmapSize = (_TotalBlocks + 7) / 8;

    // Initialize the allocation bitmap
    _AllocationBitmap = (uint8_t *)heapStart;
    for (uint32_t i = 0; i < bitmapSize; i++) {
        _AllocationBitmap[i] = 0; 
    }

    // Adjust heap start to skip the bitmap area
    _HeapStart = (uint8_t *)heapStart + bitmapSize;
}


/**
 * @brief Allocates a chunk of memory from the heap.
 *
 * Finds a contiguous block of memory of at least the requested size and marks it as allocated.
 *
 * @param size The size of the memory to allocate, in bytes.
 * @return Pointer to the allocated memory, or NULL if allocation fails.
 */
void *KMem_Allocate(uint32_t size) {
    if (size == 0) return NULL;

    uint32_t blocksNeeded = (size + KMEM_BLOCK_SIZE - 1) / KMEM_BLOCK_SIZE;
    uint32_t currentRun = 0;

    for (uint32_t i = 0; i < _TotalBlocks; i++) {
        if (!IS_BIT_SET(_AllocationBitmap, i)) {
            currentRun++;
            if (currentRun == blocksNeeded) {
                for (uint32_t j = i + 1 - blocksNeeded; j <= i; j++)
                    SET_BIT(_AllocationBitmap, j);
                
                return (uint8_t *)_HeapStart + (i + 1 - blocksNeeded) * KMEM_BLOCK_SIZE;
            }
        } else {
            currentRun = 0;
        }
    }

    return NULL; // Allocation failed
}


/**
 * @brief Frees a previously allocated chunk of memory.
 *
 * Marks the memory blocks starting at the specified pointer as free.
 *
 * @param chunk A pointer to the memory chunk to free.
 */
void KMem_Free(void *chunk) {
    if (chunk == NULL || chunk < _HeapStart || chunk >= _HeapEnd) return;

    uint32_t blockIndex = ((uint8_t *)chunk - (uint8_t *)_HeapStart) / KMEM_BLOCK_SIZE;

    while (blockIndex < _TotalBlocks && IS_BIT_SET(_AllocationBitmap, blockIndex)) {
        CLEAR_BIT(_AllocationBitmap, blockIndex);
        blockIndex++;
    }
}


/**
 * @brief Returns the total amount of free memory.
 *
 * Iterates through the allocation bitmap and calculates the total amount of unused memory.
 *
 * @return The number of free bytes in the heap.
 */
uint32_t KMem_GetBytesFree(void) {
    uint32_t freeBlocks = 0;

    for (uint32_t i = 0; i < _TotalBlocks; i++) {
        if (!IS_BIT_SET(_AllocationBitmap, i)) {
            freeBlocks++;
        }
    }

    return freeBlocks * KMEM_BLOCK_SIZE;
}
