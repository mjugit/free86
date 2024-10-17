	# File: boot_sector.s
	#
	# This file contains the assembler code of the first stage
	# bootloader, which runs in 16-bit Real Mode. It loads the
	# kernel into memory and passes control to it.
	# 

	.code16
	
	.section .text


	# Stack addresses
	.equ STACK_BOTTOM_ADDRESS, 0x7e00
	.equ STACK_TOP_ADDRESS, 0x7fff

	# Kernel addresses
	.equ KERNEL_ENTRY_ADDRESS, 0x1000
	.equ KERNEL_LOAD_SEGMENT, 0x0100
	
	# MBR entry point
	.global _mbr_main
	.type _mbr_main, @function


	# Early boot video operations
	.equ _MBR_VIDEO__COLORS, 0x07
	
	.type _mbr_video__reset_cursor, @function
	.type _mbr_video__clear_screen, @function
	.type _mbr_video__print_string, @function

	# Early boot helper methods
	.type _mbr_start__load_kernel, @function
	.type _mbr_start__halt_with_error, @function

	# Early boot messages
	.type _mbr_msg__load_error, @object
	.type _mbr_msg__load_success, @object
	.type _mbr_msg__startup, @object



	
	# Entry point
	
_mbr_main:

	cli

	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $STACK_TOP_ADDRESS, %sp

	sti

	call _mbr_video__clear_screen
	call _mbr_video__reset_cursor

	lea _mbr_msg__startup, %si
	call _mbr_video__print_string

	call _mbr_start__load_kernel
	
	jmp 0x1000





	# Video helper methods
	

_mbr_video__reset_cursor: 

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
	

_mbr_video__clear_screen: 

	pusha

	# BIOS function call:
	# Scroll window
	
	mov $6, %ah
	mov $0, %al
	mov $_MBR_VIDEO__COLORS, %bh
	mov $0x0, %ch
	mov $0x0, %cl
	mov $25, %dh
	mov $80, %dl

	int $0x10

	popa
	ret


_mbr_video__print_string: 

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

	
_mbr_start__load_kernel: 
	pusha
	
	# Set load destination
	
	xorw %ax, %ax
	movw $KERNEL_LOAD_SEGMENT, %ax
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
	
	lea _mbr_msg__load_error, %si
	call _mbr_video__print_string

	# Run into trap
	
	call _mbr_start__halt_with_error
	
2:	

	# Return successfully

	lea _mbr_msg__load_success, %si
	call _mbr_video__print_string
	
	popa
	ret
	

_mbr_start__halt_with_error: 

	lea _mbr_msg__load_error, %si
	call _mbr_video__print_string
	
	cli

1:
	hlt
	jmp 1



	
	
	#
	# REMARK on the string constants below:
	#
	# These MUST reside in the .text section, because of the harsh
	# space limitiations of the MBR (512 Bytes total).
	#
	# Using an additional section would exceed this limit due to
	# the section alignment of the linker!
	#

	
_mbr_msg__load_error:

	.asciz "Unable to load the kernel from disk.\n\r"

_mbr_msg__load_success:

	.asciz "Kernel loaded. Handing control.\n\r"
	
_mbr_msg__startup: 

	.asciz "[free86]\n\n\r"




	# BOOT SIGNATURE
	# This word is required for the disk to be recognized as bootable.
	#

	.org 510
	.word 0xaa55
