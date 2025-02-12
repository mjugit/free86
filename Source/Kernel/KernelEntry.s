
	.code16

	.section .KernelBoot

	.type KernelEntry, @function
	.global KernelEntry

	.type KernelEntry_InitVideo, @function
	.type KernelEntry_GetLowMemoryInfo, @function
	.type KernelEntry_InitA20, @function
	.type KernelEntry_Hang, @function
	.type KernelEntry_ProtectedModeStart, @function
	.type KernelEntry_CheckA20State, @function
	.type KernelEntry_EnableA20Gate, @function

	.type SystemLowMemoryKiB, @object
	.global SystemLowMemoryKiB

	
	#
	# FUNCTION: KernelEntry
	#
	# The entry point of the second boot stage.
	#
	
KernelEntry:

	cli

	call KernelEntry_InitA20
	call KernelEntry_GetLowMemoryInfo
	call KernelEntry_InitVideo
	
	# Switch to protected mode
	
	lgdt [GdtDescriptor]
	
	movl %cr0, %eax
	orl $0x1, %eax
	movl %eax, %cr0
	
	ljmp $0x08, $KernelEntry_ProtectedModeStart



KernelEntry_InitA20:
	
	call KernelEntry_CheckA20State
	jnz 1f
	call KernelEntry_EnableA20Gate
	
1:
	
	ret


KernelEntry_GetLowMemoryInfo:

	clc
	
	int $0x12
	movw %ax, SystemLowMemoryKiB
	jz KernelEntry_Hang
	
	ret

KernelEntry_InitVideo:
	
	# BIOS Function
	# Set video mode
	
	movb $0x00, %ah
	movb $0x12, %al

	int $0x10

	ret
	
	

	#
	# FUNCTION: KernelEntry_CheckA20State
	#
	# Check whether the A20 gate is enabled.
	# Returns the enabled state in %ax.
	#
	
KernelEntry_CheckA20State:

	inb $0x92, %al
	test $0x2, %al
	jz 1f

	movw $1, %ax
	ret
	
1:
	xor %ax, %ax
	ret


	
	
	#
	# FUNCTION KernelEntry_EnableA20Gate
	#
	# Enable the A20 gate.
	#

KernelEntry_EnableA20Gate:

	inb $0x92, %al
	or $0x2, %al
	outb %al, $0x92

	ret


	

	

	.section .text
	.align 4


	#
	# FUNCTION: KernelEntry_ProtectedModeStart
	#
	# The entry point of the protected mode.
	#
	
KernelEntry_ProtectedModeStart:

	.code32

	# Initialize registers
	
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	# Hand control to kernel_main

	call KernelMain

	# KernelEntry_Hang forever if kernel_main returns.
	
	cli
	
KernelEntry_Hang:
	
	hlt
	jmp KernelEntry_Hang
	


	.section .data


	
	# Holds the size of the system's low memory in KiB
	
SystemLowMemoryKiB:

	.word 0x0

	
	
