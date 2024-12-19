#include "../Include/KernelMemory.h"

/**
 * @brief Copies a specified number of bytes from a source to a destination memory location.
 *
 * This function uses optimized assembly instructions (`movsd` and `movsb`) to perform the
 * copy operation in two stages:
 * - First, it copies 4-byte words (dwords) using `rep movsd`.
 * - Then, it copies any remaining bytes using `rep movsb`.
 *
 * @param dest Pointer to the destination memory where data will be copied.
 * @param src  Pointer to the source memory from where data will be copied.
 * @param count The number of bytes to copy.
 *
 * @note The memory regions pointed to by `dest` and `src` should not overlap.
 *       If overlapping memory regions are possible, use a function like `memmove`.
 */
void KMem_Copy(void *dest, const void *src, unsigned int count) {
  unsigned int dwords = count / 4;
  unsigned int bytes  = count % 4;

  __asm__ __volatile__ (
			"cld\n\t" 
			"rep movsd\n\t" 
			: "+D"(dest), "+S"(src), "+c"(dwords)
			:
			: "memory"
			);

  __asm__ __volatile__ (
			"movl %2, %%ecx\n\t"
			"rep movsb\n\t"
			: "+D"(dest), "+S"(src)
			: "r"(bytes)
			: "ecx", "memory"
			);
}
