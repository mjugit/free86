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


	.code16

	.section .Kernel_EarlyBoot


	// Kernel entry point
	.global Kernel_EntryPoint
	.type Kernel_EntryPoint, @function

	// Port number of the A20 register
	.equ A20GateRegisterPort, 0x92

	// High address support
	.type _Kernel_EnableHighAddressSupport, @function
	.type _Kernel_CheckIfA20GateEnabled, @function
	.type _Kernel_EnableA20Gate, @function

	// Video mode setup
	.type _Kernel_EnablePlanarGraphicsMode, @function

	// Memory size
	.type _Kernel_GetLowMemorySize, @function
	.global _Kernel_LowMemoryInfoKiB
	.type _Kernel_LowMemoryInfoKiB, @object
	
	.type _Kernel_GetHighMemorySize, @function
	.global _Kernel_HighMemoryInfoKiB
	.type _Kernel_HighMemoryInfoKiB, @object

	// Equipment
	.type _Kernel_GetEquipmentInfo @function
	.global _Kernel_EquipmentWord
	.type _Kernel_EquipmentWord, @object
	


	// Protected Mode
	.type _Kernel_ProtectedMode, @function
	.type _Kernel_Hang, @function
	


	/*
	This is the kernel entry point. The code below is executed
	immediately after the bootloader finished loading the kernel
	code. Note, that this file belongs to another binary than the
	bootloader! They are not compiled or linked together.
	*/
	
Kernel_EntryPoint:

	cli

	
	call _Kernel_EnableHighAddressSupport
	call _Kernel_GetLowMemorySize
	call _Kernel_GetHighMemorySize
	call _Kernel_GetEquipmentInfo
	call _Kernel_EnablePlanarGraphicsMode

	// Switch to Protected Mode

	lgdt [GdtDescriptor]

	movl %cr0, %eax
	orl $0x1, %eax
	movl %eax, %cr0

	ljmp $0x08, $_Kernel_ProtectedMode

	


	/*
	Make sure, that the A20 gate is enabled and addresses above
	1MB are supported.
	*/

_Kernel_EnableHighAddressSupport:

	call _Kernel_CheckIfA20GateEnabled
	jnz 1f

	call _Kernel_EnableA20Gate

1:
	ret
	


	/*
	Check, whether the A20 gate is enabled (support for addresses
	above 1MB). The AX register will be set to 1 if it is enabled,
	otherwise it is set to zero.
	*/
	
_Kernel_CheckIfA20GateEnabled:

	inb $A20GateRegisterPort, %al
	test $0x2, %al

	jz 1f

	movw $1, %ax
	ret

1:
	xor %ax, %ax
	ret



	/*
	Switch on the A20 gate by flipping the respective bit. This
	enables addresses above 1 MB to be used.
	*/

_Kernel_EnableA20Gate:

	inb $A20GateRegisterPort, %al
	or $0x2, %al
	outb %al, $A20GateRegisterPort

	ret



	/*
	Enable the VGA 'Planar Graphics Mode' with a color depth of
	4bit, which supports 16 distinct colors. The palette can be
	dynamically redefined.
	*/
	
_Kernel_EnablePlanarGraphicsMode:

	movb $0x00, %ah
	movb $0x12, %al

	int $0x10

	ret



	/*
	Get the size of the low memory area (below 1MB) and write it
	to the global variable _LowMemoryInfoKiB.
	*/
	
_Kernel_GetLowMemorySize:

	clc

	int $0x12
	movw %ax, _Kernel_LowMemoryInfoKiB
	jz _Kernel_Hang

	ret

	
	/*
	Get the size of the high memory area (above 1MB) and write it
	to the global variable _HighMemoryInfoKiB.
	*/

_Kernel_GetHighMemorySize:

	clc

	mov $0x88, %ah
	int $0x15

	movw %ax, _Kernel_HighMemoryInfoKiB
	jc _Kernel_Hang

	ret


	

	.section .text
	.align 4


_Kernel_GetEquipmentInfo:

	int $0x11
	movw %ax, _Kernel_EquipmentWord

	ret


_Kernel_ProtectedMode:

	.code32

	// Initialize registers
	
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	// Place stack somewhere in low memory
	movl $0x80000, %esp

	call KernelMain

	// KernelMain usually never returns. If it does anyway, hang
	// forever

_Kernel_Hang:

	cli

1:	
	hlt
	jmp 1b

	


	.section .data


	// Holds the size of the machine's low memory in KiB
	
_Kernel_LowMemoryInfoKiB:

	.word 0x0

	
	// Holds the size of the machine's high memory (>1MB) in KiB
	
_Kernel_HighMemoryInfoKiB:

	.word 0x0


	// Holds the bitmask describing the installed hardware

_Kernel_EquipmentWord:

	.word 0x0
