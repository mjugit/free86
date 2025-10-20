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


	.global _Memory_SetImplementation
	.type _Memory_SetImplementation, @function

	
_Memory_SetImplementation:

	pushl %ebp
	movl  %esp, %ebp
	pushl %ebx

	// Load params
	movl  8(%ebp), %edi    // Destination
	movzbl 12(%ebp), %eax  // Value (low byte of eax)
	movl 16(%ebp), %ecx    // Byte count

	cld

	// Construct a dword pattern in eax
	imull  $0x01010101, %eax  // replicate byte to all four bytes

	// Save original count
	movl %ecx, %ebx

	// Prepare for dword writes
	shrl $2, %ecx
	rep stosl

	// Write remaining bytes
	movl %ebx, %ecx
	andl $3, %ecx
	movb 12(%ebp), %al
	rep stosb
	
	popl %ebx
	popl %ebp
	ret

