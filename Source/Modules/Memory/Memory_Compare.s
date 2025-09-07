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

	
	.global _Memory_CompareImplementation
	.type _Memory_CompareImplementation, @function

	
_Memory_CompareImplementation:

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
