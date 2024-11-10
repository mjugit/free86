
	.section .text
	.global ISR_Keyboard
	.type ISR_Keyboard, @function
	
	.code32

	.equ MASTER_PIC_CONTROL_PORT, 0x20

ISR_Keyboard:

	cli
	pusha

	call keyboard_handler

	movb $MASTER_PIC_CONTROL_PORT, %al
	outb %al, $MASTER_PIC_CONTROL_PORT

	popa
	sti
	iret
	