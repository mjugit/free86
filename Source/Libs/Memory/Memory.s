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

	

	.global Memory

	// Represents the namespace
	.type Memory, @object
	

	// Namespace members
	.type _Memory_Copy, @function
	.type _Memory_Set, @function
	.type _Memory_Move, @function
	.type _Memory_Compare, @function
	.type _Memory_Find, @function


	
	.section .text

	/*
	Copy a certain amount of bytes to another location. This
	function copies whole dwords (if possible) to optimize
	performance. This method has no overlap protection!
	*/

_Memory_Copy:

	pushl %ebp
	movl %esp, %ebp

	// Load params

	movl 16(%ebp), %ecx	// Byte count
	movl 8(%ebp), %edi	// Destination
	movl 12(%ebp), %esi	// Source

	// Ensure ES=DS to prevent copy errors
	push %ds
	pop  %es

	// Copy whole dwords
	
	cld			// Clear direction flag (forward)
	movl %ecx, %eax
	shrl $2, %ecx		// Calculate dwords to copy
	rep movsl		// Copy dwords

	// Copy remaining bytes
	
	movl %eax, %ecx
	andl $3, %ecx		// Calculate remaining bytes
	rep movsb

	popl %ebp
	ret



	/*
	Set a certain amount of bytes to a specific value. This
	function uses whole dwords (if possible) to optimize
	performance.
	*/

_Memory_Set:	

	pushl %ebp
	movl  %esp, %ebp

	// Load params
	movl  8(%ebp), %edi    // Destination
	movzbl 12(%ebp), %eax     // Value (low byte of eax)
	movl 16(%ebp), %ecx    // Byte count

	cld

	// Construct a dword pattern in eax
	imull  $0x01010101, %eax  // replicate byte to all four bytes

	// Save original count
	movl %ecx, %ebx

	// Prepare for dword writes
	movl %edx, %eax        // stosl uses eax
	shrl $2, %ecx
	rep stosl

	// Write remaining bytes
	movl %ebx, %ecx
	andl $3, %ecx
	movb 12(%ebp), %al
	rep stosb

	popl %ebp
	ret



	/*
	Copy a certain amount of bytes to another location. This
	function copies whole dwords (if possible) to optimize
	performance. This method provides overlap protection and will
	reverse the copy direction if required.
	*/

_Memory_Move:

	pushl %ebp
	movl  %esp, %ebp

	// Load params

	movl  8(%ebp), %esi    // Source
	movl 12(%ebp), %edi    // Destination
	movl 16(%ebp), %ecx    // Byte count

	cld                    // Clear direction flag (forward)

	// Check for overlap
	cmpl %esi, %edi        // If destination is before source
	jb   1f                // => forward copy

	movl %esi, %eax
	addl %ecx, %eax        // eax = source + length
	cmpl %eax, %edi        // If destination is inside source range
	jb   2f                // => backward copy

1:  // Forward copy

	movl %ecx, %eax

	// Copy dwords
	shrl $2, %ecx
	rep movsl

	// Copy remaining bytes
	movl %eax, %ecx
	andl $3, %ecx
	rep movsb

	jmp 3f


2:  // Backward copy

	// Set pointers to the end of the block
	addl %ecx, %esi
	addl %ecx, %edi

	std                    // Set direction flag (backward)

	// Copy dwords
	movl %ecx, %eax
	shrl $2, %ecx
	subl $4, %esi
	subl $4, %edi
	rep movsl

	// Copy remaining bytes
	movl %eax, %ecx
	andl $3, %ecx
	subl $1, %esi
	subl $1, %edi
	rep movsb

	cld                    // Clear direction flag

3:
	popl %ebp
	ret


	
	/*
	Compare to blocks of memory. This method will return 0 if they
	are equal and non-zero otherwise. To optimize performance,
	whole dwords are compared (if possible).
	*/

_Memory_Compare:

	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %esi	// Pointer to block A
	movl 12(%ebp), %edi	// Pointer to block B
	movl 16(%ebp), %ecx	// Byte count

	cld			// Clear direction flag (forward)


	movl %ecx, %eax
	shrl $2, %ecx		// Calculate dwords to compare
	jz ._Memory_Compare_Check_Bytes

	
	._Memory_Compare__Dword_Loop:

	movl (%esi), %edx
	movl (%edi), %ebx
	cmpl %ebx, %edx
	jne ._Memory_Compare__Dword_Diff

	addl $4, %esi
	addl $4, %edi
	decl %ecx
	jnz ._Memory_Compare__Dword_Loop


	._Memory_Compare_Check_Bytes:

	movl %eax, %ecx		// Calculate remaining bytes
	andl $3, %ecx
	jz ._Memory_Compare__Equal

	
	._Memory_Compare__Byte_Loop:

	movzb (%esi), %eax
	movzb (%edi), %edx
	cmp %al, %dl
	jne ._Memory_Compare__Byte_Diff

	inc %esi
	inc %edi
	decl %ecx
	jnz ._Memory_Compare__Byte_Loop

	
	._Memory_Compare__Equal:

	xorl %eax, %eax
	jmp ._Memory_Compare__Done
	
	
	._Memory_Compare__Dword_Diff:

	// If a difference is found, reset and go to byte comparison
	// (to find the *first* difference)
	
	movl 8(%ebp), %esi
	movl 12(%ebp), %edi
	movl 16(%ebp), %ecx
	jmp ._Memory_Compare__Byte_Loop


	._Memory_Compare__Byte_Diff:

	subl %edx, %eax
	jmp ._Memory_Compare__Done
	

	._Memory_Compare__Done:

	popl %ebp
	ret



	/*		
	Find the first occurrence of a value in a block of memory. If
	found, this function will return a pointer to the value
	location or null otherwise.
	*/

_Memory_Find:

	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %esi	// Block address
	movzbl 12(%ebp), %eax	// Search Value
	movl 16(%ebp), %ecx	// Byte count

	cld			// Clear direction flag (forward)

	
	._Memory_Find__Find_Loop:

	test %ecx, %ecx
	je ._Memory_Find__Not_Found

	movzb (%esi), %edx	// Load Byte
	cmp %dl, %al
	je ._Memory_Find__Found

	inc %esi
	dec %ecx
	jmp ._Memory_Find__Find_Loop


	._Memory_Find__Found:

	movl %esi, %eax
	jmp ._Memory_Find__Done


	._Memory_Find__Not_Found:

	xorl %eax, %eax


	._Memory_Find__Done:

	popl %ebp
	ret
	



	

	.section .rodata

	
	/*		
	This is the namespace definition for the functions implemented
	in this file. The code below represents a struct of function
	pointers hiding the actual function labels. See the
	corresponding header file for more details.
	*/
	
Memory:
	.long _Memory_Copy
	.long _Memory_Set
	.long _Memory_Move
	.long _Memory_Compare
	.long _Memory_Find

	
