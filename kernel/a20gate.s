
	.code16

	.section .text

	.global a20_check_state
	.type a20_check_state, @function

	.global a20_enable
	.type a20_enable, @function


	
	#
	# FUNCTION: a20_check_state
	#
	# Check whether the A20 gate is enabled.
	# Returns the enabled state in %ax.
	#
	
a20_check_state:

	inb $0x92, %al
	test $0x2, %al
	jz 1f

	movw $1, %ax
	ret
	
1:
	xor %ax, %ax
	ret


	
	#
	# FUNCTION a20_enable
	#
	# Enable the A20 gate.
	#

a20_enable:

	inb $0x92, %al
	or $0x2, %al
	outb %al, $0x92

	ret
	
