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


	.global Bitmap

	// Represents the namespace
	.type Bitmap, @object
	
	// Namespace members
	.type _Bitmap_SetBit, @function
	.type _Bitmap_ClearBit, @function
	.type _Bitmap_Check, @function
	.type _Bitmap_Wipe, @function
	.type _Bitmap_NextFree, @function

	
	.section .text


	
	/*
	Set a single bit in a bitmap.
	*/

_Bitmap_SetBit:

	pushl %ebp
	movl %esp, %ebp

	// Load params

	movl 12(%ebp), %eax	// Bit index
	movl 8(%ebp), %edx	// Bitmap pointer

	// Calculate offset
	
	movl %eax, %ecx
	shrl $3, %ecx

	// Calculate bitmask
	
	andl $7, %eax
	pushl %ebx
	movb $1, %bl
	pushl %ecx		// save byte offset (ECX will be clobbered via CL)
	movb %al, %cl		// Move is necessary for shlb to work
	shlb %cl, %bl
	popl %ecx		// restore byte offset

	// Apply

	movb (%edx, %ecx, 1), %al
	orb %bl, %al
	movb %al, (%edx, %ecx, 1)

	popl %ebx
	popl %ebp
	ret



	/*
	Clear a single bit in a bitmap
	*/

_Bitmap_ClearBit:	

	pushl %ebp
	movl %esp, %ebp
	
	// Load params

	movl 12(%ebp), %eax	// Bit index
	movl 8(%ebp), %edx	// Bitmap pointer

	// Calculate offset

	movl %eax, %ecx
	shrl $3, %ecx

	// Calculate bitmask

	andl $7, %eax
	pushl %ebx
	movb $1, %bl
	pushl %ecx		// preserve byte offset (CL will be used as shift count)
	movb %al, %cl		// Move is necessary for shl to work
	shlb %cl, %bl
	notb %bl
	popl %ecx		// restore byte offset

	// Apply

	movb (%edx, %ecx, 1), %al
	andb %bl, %al
	movb %al, (%edx, %ecx, 1)

	popl %ebx
	popl %ebp
	ret



	/*
	Test, whether a specific bit is set.
	*/

_Bitmap_Check:

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



	/*
	Clears multiple bits in a bitmap.
	*/

_Bitmap_Wipe:

	pushl %ebp
	movl %esp, %ebp

	// Load params

	movl 8(%ebp), %edi	// Pointer to bitmap
	movl 12(%ebp), %ecx	// Bit count

	// Calculate bytes

	addl $7, %ecx
	shrl $3, %ecx

	xorl %eax, %eax
	cld			// Clear direction flag (forward)

	pushl %edi		// preserve EDI (callee-saved)

	._Bitmap_Wipe__Clear_Loop:

	testl %ecx, %ecx
	jz ._Bitmap_Wipe__Done
	stosb
	decl %ecx
	jmp ._Bitmap_Wipe__Clear_Loop

	._Bitmap_Wipe__Done:

	popl %edi
	popl %ebp
	ret



	.section .rodata

Bitmap:
	.long _Bitmap_SetBit
	.long _Bitmap_ClearBit
	.long _Bitmap_Check
	.long _Bitmap_Wipe
	.long _Bitmap_NextFree
	

