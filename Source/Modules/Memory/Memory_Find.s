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

	
	.global _Memory_FindImplementation
	.type _Memory_FindImplementation, @function

	
_Memory_FindImplementation:

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

	
