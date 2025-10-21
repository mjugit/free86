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


	.global _Isr_Keyboard
	.type _Isr_Keyboard, @function

	.global _KeyboardBuffer_Read
	.type _KeyboardBuffer_Read, @function

	.equ KeyboardDataPort, 0x60
	.equ ExtendedPrefix, 0xe0


	

	.section .bss
	.align 4

	// Global 16 ring buffer for keyboard scancodes
	
_KeyboardBuffer:
	
	.skip 64 		// 32 Entries * 2 bytes = 64 bytes

_KeyboardBuffer_WriteIndex:

	.long 0

_KeyboardBuffer_ReadIndex:

	.long 0

_LastScanCode:

	.byte 0

	

	
	.section .text

	// Keyboard interrupt service routine

_Isr_Keyboard:

	pusha

	// Read scancode
	inb $KeyboardDataPort, %al
	movzbl %al, %eax

	cmp $ExtendedPrefix, %al
	jne 1f

1:	
	// Check if previous was extended prefix
	movb _LastScanCode, %bl
	cmp $ExtendedPrefix, %bl
	jne 2f

	// Combine prefix with current byte
	xor %ebx, %ebx
	movb %al, %bl
	or $0xe000, %bx
	movb $0, _LastScanCode
	jmp _Store_ScanCode

2:
	// Normal scancode
	xor %ebx, %ebx
	movb %al, %bl
	movb %al, _LastScanCode
	

_Store_ScanCode:

	push %ebx
	call _KeyboardBuffer_Write
	add $4, %esp
	

_Isr_End:	

	/* Send EOI */
	movb $0x20, %al
	outb %al, $0x20

	popa
	iret
	



_KeyboardBuffer_Write:

	push %eax
	push %ecx
	push %edx

	// Load current index
	mov _KeyboardBuffer_WriteIndex, %ecx

	// Calculate address
	movl %ecx, %edx
	shll $1, %edx
	movl $_KeyboardBuffer, %eax
	addl %edx, %eax
	
	// Store value
	movw %bx, (%eax)

	// Increment and wrap modulo 32
	inc %ecx
	and $31, %ecx
	mov %ecx, _KeyboardBuffer_WriteIndex


	pop %edx
	pop %ecx
	pop %eax
	ret

	


_KeyboardBuffer_Read:

	push %ecx
	push %edx

	// Load indices
	movl _KeyboardBuffer_ReadIndex, %ecx
	movl _KeyboardBuffer_WriteIndex, %edx

	// Check if empty
	cmpl %ecx, %edx
	je _Buffer_Empty

	// Calculate address
	movl %ecx, %edx
	shll $1, %edx
	lea _KeyboardBuffer(,%edx), %edx

	// Read value
	movw (%edx), %ax

	// Increment and wrap read index
	incl %ecx
	andl $31, %ecx
	movl %ecx, _KeyboardBuffer_ReadIndex
	jmp _Buffer_Done
	
_Buffer_Empty:
	xor %ax, %ax			// Return zero
	
_Buffer_Done:	
	pop %edx
	pop %ecx
	ret
	
