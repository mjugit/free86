

	.code16
	.section .text

	.equ STACK_BOTTOM, 0x7e00
	.equ STACK_TOP, 0x7fff

	.equ SCREEN_COLOR, 0x1f

	.global boot_sector_start
	.type boot_sector_start, @function

	.type reset_cursor, @function
	.type clear_screen, @function
	.type print_message, @function

	.type startup_message, @object

boot_sector_start:

	cli

	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $STACK_TOP, %sp

	sti

	call clear_screen
	call reset_cursor

	lea startup_message, %si
	call print_message

	lea loading_message, %si
	call print_message

boot_sector_catch:

	cli

1:
	hlt
	jmp 1


	#
	# Helper methods
	#


reset_cursor: 

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


clear_screen: 

	pusha

	# BIOS function call:
	# Scroll window
	
	mov $6, %ah
	mov $0, %al
	mov $SCREEN_COLOR, %bh
	mov $0x0, %ch
	mov $0x0, %cl
	mov $25, %dh
	mov $80, %dl

	int $0x10

	popa
	ret


print_message: 

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


	#
	# REMARK on the string constants below:
	#
	# These MUST reside in the .text section, because of the harsh
	# space limitiations of the MBR (512 Bytes total).
	#
	# Using an additional section would exceed this limit due to
	# the section alignment of the linker!
	#

startup_message: 

	.asciz "[free86 bootloader]\n\r"


loading_message: 

	.asciz "Loading kernel...\n\r"


	# BOOT SIGNATURE
	# This word is required for the disk to be recognized as bootable.
	#

	.org 510
	.word 0xaa55
