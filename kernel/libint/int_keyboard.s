
	.section .text
	.global isr_keyboard
	.type isr_keyboard, @function
	
	.code32

	.equ MASTER_PIC_CONTROL_PORT, 0x20

isr_keyboard:

	cli
	pusha

	call keyboard_handler

	movb $MASTER_PIC_CONTROL_PORT, %al
	outb %al, $MASTER_PIC_CONTROL_PORT

	popa
	sti
	iret
	
