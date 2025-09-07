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


	/*
	
	This file contains the source code of the bootloader running
	in 16-bit Real Mode. It is located in the MBR, which is the
	first sector of the disk (with a maximum size of 512 bytes).

	When the computer is powered up, this sector is loaded into
	memory at 0x7c00 and executed. Note, that the last two bytes
	of the sector must contain the boot signature for the medium
	to be considered as 'bootable'.

	Essentially this code does nothing more than loading the
	kernel from disk into memory and passing control to it.

	*/


	.code16
	.section .text


	// Stack address definitions
	.equ StackBottom, 0x7e00
	.equ StackTop, 0x7fff

	// Kernel address definitions
	.equ KernelEntryAddress, 0x1000
	.equ KernelLoadSegment, 0x0100


	// MBR entry point
	.global Bootloader_EntryPoint
	.type Bootloader_EntryPoint, @function


	// Screen colors for text output
	.equ Bootloader_ScreenColorAttributes, 0x07

	// Video functions	
	.type Bootloader_ResetCursor, @function
	.type Bootloader_ClearScreen, @function
	.type Bootloader_PrintString, @function

	// Loading functions
	.type Bootloader_LoadKernel, @function
	.type Bootloader_HaltWithError, @function


	// Text resources
	.type BootMessage_LoadError, @object
	.type BootMessage_LoadSuccess, @object
	.type BootMessage_Title, @object



Bootloader_EntryPoint:

	// Clear registers and set up the stack
	
	cli

	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $StackTop, %sp

	sti


	call Bootloader_ResetCursor
	call Bootloader_ClearScreen

	lea BootMessage_Title, %si
	call Bootloader_PrintString

	call Bootloader_LoadKernel
	jmp KernelEntryAddress



Bootloader_ResetCursor:

	pusha

	// BIOS function call:
	// Set cursor position
	
	movb $0x02, %ah		// Function id
	movb $0x00, %bh		// Page number
	movb $0x00, %dh		// Row number
	movb $0x00, %dl		// Column number
	int $0x10

	popa
	ret



Bootloader_ClearScreen:

	pusha

	// BIOS function call:
	// Scroll window
	
	mov $6, %ah
	mov $0, %al
	mov $Bootloader_ScreenColorAttributes, %bh
	mov $0x0, %ch
	mov $0x0, %cl
	mov $25, %dh
	mov $80, %dl

	int $0x10

	popa
	ret



Bootloader_PrintString:

	pusha

	// BIOS function call:
	// Print in TTY mode
	
	movb $0x0e, %ah		// Function id
1:
	lodsb			// Write a char at a time until null
	test %al, %al
	jz 2f
	int $0x10
	
	jmp 1b
2:	
	popa
	ret

	pusha



Bootloader_LoadKernel:

	pusha
	
	// Set load destination
	
	xorw %ax, %ax
	movw $KernelLoadSegment, %ax
	movw %ax, %es
	xorw %bx, %bx

	// BIOS Function:
	// Read sectors from disk

	movb $0x02, %ah		// Function code
	movb $32, %al		// Number of sectors to read
	movb $0, %ch		// Cylinder number
	movb $2, %cl		// Sector number
	movb $0, %dh		// Head number
	movb $0, %dl		// Drive number (0 = floppy)

	int $0x13

	jc 1f
	jmp 2f

1:
	// Print error message
	
	lea BootMessage_LoadError, %si
	call Bootloader_PrintString

	// Run into trap
	
	call Bootloader_HaltWithError
	
2:	

	// Return successfully

	lea BootMessage_LoadSuccess, %si
	call Bootloader_PrintString
	
	popa
	ret



Bootloader_HaltWithError:

	lea BootMessage_LoadError, %si
	call Bootloader_PrintString

	// Halt forever
	
	cli

1:
	hlt
	jmp 1



	/*

	Since the compiled code has to fit into a single sector of 512
	bytes, there is no space for a separate .data section. All
	additional text resources must hence be placed in the same
	section as the actual source code. Otherwise the linker will
	align the .data section at the 512 byte boundary, exceeding
	the size limit.

	*/

BootMessage_LoadError:

	.asciz "Unable to load the kernel from disk.\n\r"


	
BootMessage_LoadSuccess:

	.asciz "Kernel loaded. Handing control.\n\r"


	
BootMessage_Title: 

	.asciz "[free86]\n\n\r"



	/*

	The boot signature (see below) must be placed at the end of
	the sector for the disk to be recognized as bootable.
	
	*/

	.org 510
	.word 0xaa55
	
