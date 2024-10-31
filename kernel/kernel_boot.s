
	.code16

	.section .kernel_boot

	.type _kernel_boot, @function
	.global _kernel_boot

	.type _kernel_boot__init_video, @function
	.type _kernel_boot__init_memory_map, @function
	.type _kernel_boot__init_a20, @function
	.type _kernel_boot__init_paging, @function

	.type _system_memory_map, @object
	.global _system_memory_map

	.type _system_page_table, @object
	.global _system_page_table

	
	#
	# FUNCTION: _kernel_boot
	#
	# The entry point of the second boot stage.
	#
	
_kernel_boot:

	cli

	call _kernel_boot__init_a20
	call _kernel_boot__init_memory_map
	# call _kernel_boot__init_paging
	call _kernel_boot__init_video
	
	# Switch to protected mode
	
	lgdt [gdt_descriptor]
	
	movl %cr0, %eax
	orl $0x1, %eax
	movl %eax, %cr0
	
	ljmp $0x08, $_kernel_boot__protected_mode


	
_kernel_boot__init_a20:
	
	call a20_check_state
	jnz 1f
	call a20_enable
	
1:
	
	ret


_kernel_boot__init_memory_map:



	# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	# TODO: Memory map does not init properly


	
	# BIOS Function
	# Get System Memory Map

	xor %eax, %eax
	movw $0xe820, %ax
	mov $0, %ebx
	mov $_system_memory_map, %di
	movw $16, %cx
	mov $0x534D4150, %edx

.memory_map_loop:

	int $0x15
	jc .memory_map_loop

	cmp $0x534D4150, %eax
	jne .memory_map_end

	cmp $16, %cl
	je .memory_map_move_next

	add $4, %di

.memory_map_move_next:
	add $16, %di

	test %ebx, %ebx
	jnz .memory_map_loop
	
.memory_map_end:

	ret

	.break:

	cli
	hlt


_kernel_boot__init_paging:

	mov $_system_page_directory, %eax
	mov %eax, %cr3

	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0

	ret
	

_kernel_boot__init_video:
	
	# BIOS Function
	# Set video mode
	
	movb $0x00, %ah
	movb $0x12, %al

	int $0x10

	ret
	
	
	

	.section .text
	.align 4

	.type _kernel_boot__protected_mode, @function



	#
	# FUNCTION: _kernel_boot__protected_mode
	#
	# The entry point of the protected mode.
	#
	
_kernel_boot__protected_mode:

	.code32

	# Initialize registers
	
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	# Hand control to kernel_main

	call kernel_main

	# Hang forever if kernel_main returns.
	
	cli
hang:
	
	hlt
	jmp hang
	


	.section .data

	
_system_memory_map:	

	.space 1024


	.section .bss

	.align 4096
_system_page_directory:

	.space 4096

	
	.align 4096
_system_page_table:
	
	.space 4096
	
