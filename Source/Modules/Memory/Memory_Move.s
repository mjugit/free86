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

	
	.global _Memory_MoveImplementation
	.type _Memory_MoveImplementation, @function

	
_Memory_MoveImplementation:

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
