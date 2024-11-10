	# File: boot_sector.s
	#
	# This file contains the assembler code of the first stage
	# bootloader, which runs in 16-bit Real Mode. It loads the
	# kernel into memory and passes control to it.
	# 

	.code16
	
	.section .text


	# Stack addresses
	.equ StackBottom, 0x7e00
	.equ StackTop, 0x7fff

	# Kernel addresses
	.equ KernelMainStart, 0x1000
	.equ KernelLoadSegment, 0x0100
	
	# MBR entry point
	.global BootloaderMain
	.type BootloaderMain, @function


	# Early boot video operations
	.equ Bootloader_ScreenColorAttributes, 0x07

	.type Bootloader_ResetCursor, @function
	.type Bootloader_ClearScreen, @function
	.type Bootloader_PrintString, @function

	# Early boot helper methods
	.type Bootloader_LoadKernel, @function
	.type Bootloader_HaltWithError, @function

	# Early boot messages
	.type BootMessage_LoadError, @object
	.type BootMessage_LoadSuccess, @object
	.type BootMessage_Title, @object


	# Entry point
	
BootloaderMain:

	cli

	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $StackTop, %sp

	sti

	call Bootloader_ClearScreen
	call Bootloader_ResetCursor

	lea BootMessage_Title, %si
	call Bootloader_PrintString

	call Bootloader_LoadKernel
	
	jmp KernelMainStart





	# Video helper methods
	

Bootloader_ResetCursor: 

	pusha

	# BIOS function call:
	# Set cursor position
	
	movb $0x02, %ah		# Function id
	movb $0x00, %bh		# Page number
	movb $0x00, %dh		# Row number
	movb $0x00, %dl		# Column number
	int $0x10

	popa
	ret
	

Bootloader_ClearScreen: 

	pusha

	# BIOS function call:
	# Scroll window
	
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

	# BIOS function call:
	# Print in TTY mode
	
	movb $0x0e, %ah		# Function id
1:
	lodsb			# Write a char at a time until null
	test %al, %al
	jz 2f
	int $0x10
	
	jmp 1b
2:	
	popa
	ret


	
	# Startup helper methods

	
Bootloader_LoadKernel: 
	pusha
	
	# Set load destination
	
	xorw %ax, %ax
	movw $KernelLoadSegment, %ax
	movw %ax, %es
	xorw %bx, %bx

	# BIOS Function:
	# Read sectors from disk

	movb $0x02, %ah		# Function code
	movb $16, %al		# Number of sectors to read
	movb $0, %ch		# Cylinder number
	movb $2, %cl		# Sector number
	movb $0, %dh		# Head number
	movb $0, %dl		# Drive number (0 = floppy)

	int $0x13

	jc 1f
	jmp 2f

1:
	# Print error message
	
	lea BootMessage_LoadError, %si
	call Bootloader_PrintString

	# Run into trap
	
	call Bootloader_HaltWithError
	
2:	

	# Return successfully

	lea BootMessage_LoadSuccess, %si
	call Bootloader_PrintString
	
	popa
	ret
	

Bootloader_HaltWithError: 

	lea BootMessage_LoadError, %si
	call Bootloader_PrintString
	
	cli

1:
	hlt
	jmp 1



	
	
	#
	# REMARK on the string constants below:
	#
	# Since the compiled code has to fit into a single section of
	# 512 bytes on disk, you MUST NOT make use of more than the
	# .text section. This is because the linker will align the
	# sections at certain boundaries causing the resulting file to
	# exceed this limit.
	#

BootMessage_LoadError:

	.asciz "Unable to load the kernel from disk.\n\r"

BootMessage_LoadSuccess:

	.asciz "Kernel loaded. Handing control.\n\r"
	
BootMessage_Title: 

	.asciz "[free86]\n\n\r"


	.org 510
	
	# BOOT SIGNATURE
	# This word is required for the disk to be recognized as bootable.
	#
	
	.word 0xaa55
