	.code16
	.section .kernel_boot

	.type _kernel_boot, @function
	.global _kernel_boot

	# Kernel boot messages
	.type _kernel_msg__boot_error, @object
	
_kernel_boot:

	cli

	call check_a20
	jnz kernel_start
	call enable_a20

kernel_start:

	# Set graphics mode
	
	movb $0x13, %al
	movb $0x00, %ah
	int $0x10

	# Load GDT
	
	lgdt [gdt_descriptor]

	# Enable protected mode
	
	movl %cr0, %eax
	orl $0x1, %eax
	movl %eax, %cr0
	
	ljmp $0x08, $_kernel_boot__protected_mode


check_a20:

	# Check A20 state

	in $0x92, %al       # Read port 0x92
	test $0x2, %al      # Check second bit
	jz a20_disabled

	# Return 1 in eax

	mov $1, %eax

	ret

	
a20_disabled:

	# Return 0 in eax

	xor %eax, %eax

	ret

enable_a20:

	in $0x92, %al       # Read port 0x92
	or $0x2, %al        # Set second bit
	out %al, $0x92      # Write port 0x92

	ret

	
	
	.section .text
	.align 4

	.type _kernel_boot__protected_mode, @function
_kernel_boot__protected_mode:

	.code32
	
	mov $0x10, %ax
	
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	mov %ax, %ss

	call kernel_main
	
hang:
	
	hlt
	jmp hang
	



	
