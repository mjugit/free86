
	.section .Gdt
	.align 8

GdtHead:

	# Null segment
	
	.quad 0x0000000000000000

	# Kernel code segment

	.word 0x1fff		# Segment limit (8K - 1)
	.word 0x0000		# Base address (low)
	.byte 0x00 		# Base address (middle)
	.byte 0x9a		# Access byte (Executable, Readable, Code-Segment)
	.byte 0xcf		# Flags
	.byte 0x00		# Base address (high)

	# Kernel data segment

	.word 0xffff		# Segment limit (1024K - 1)
	.word 0x0000 		# Base address (low)
	.byte 0x00		# Base address (middle)
	.byte 0x92		# Access byte (Readable, Writable, Data-Segment)
	.byte 0xcf		# Flags
	.byte 0x00 		# Base address (high)

GdtTail:


	.global GdtDescriptor
	
GdtDescriptor:

	.word GdtTail - GdtHead - 1
	.long GdtHead
	
