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


	.global _Bitmap_WipeImplementation
	.type _Bitmap_WipeImplementation, @function

_Bitmap_WipeImplementation:

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
