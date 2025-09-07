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


	.global _Bitmap_CheckImplementation
	.type _Bitmap_CheckImplementation, @function

_Bitmap_CheckImplementation:

	pushl %ebp
	movl %esp, %ebp

	// Load params

	movl 12(%ebp), %eax	// Bit index
	movl 8(%ebp), %edx	// Pointer to bitmap

	// Calculate offset

	movl %eax, %ecx
	shrl $3, %ecx

	// Calculate bitmask

	andl $7, %eax
	pushl %ebx
	movb $1, %bl
	pushl %ecx		// preserve byte offset (CL will be used as shift count)
	movb %al, %cl
	shlb %cl, %bl
	popl %ecx		// restore byte offset

	// Test bit

	movb (%edx, %ecx, 1), %al
	testb %bl, %al
	setne %al
	movzx %al, %eax
	
	popl %ebx
	popl %ebp
	ret
