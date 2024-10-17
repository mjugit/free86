	.code16
	
	.section .kernel_boot

	.type kernel_start, @function
	.global kernel_start

	.type kernel_hang, @function
	.type kprint_message, @function
	
kernel_start:

	lea hello_world_msg, %si
	call kprint_message
	call kernel_hang

	.section .text

kernel_hang:

	cli
	hlt

	jmp kernel_hang
	

kprint_message: 

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
	

	.section .data

hello_world_msg:

	.asciz "Hello, world!\n\r"
	
