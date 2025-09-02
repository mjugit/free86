	/*
	
	Copyright © 2025 Maximilian Jung

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the “Software”), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the
	Software.

	THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
	KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
	*/


	.section .GlobalDescriptorTable
	.align 8

	.global GdtDescriptor
	.type GdtDescriptor, @object

	

GlobalDescriptorTable_Head:

	// Null segment

	.quad 0x0000000000000000


	// Kernel code segment

	.word 0xffff		// Segment limit
	.word 0x0000		// Base address (low)
	.byte 0x00 		// Base address (middle)
	.byte 0x9a		// Access byte (Executable, Readable, Code-Segment)
	.byte 0x4f		// Flags
	.byte 0x00		// Base address (high)


	// Kernel data segment

	.word 0xffff		// Segment limit
	.word 0x0000 		// Base address (low)
	.byte 0x00		// Base address (middle)
	.byte 0x92		// Access byte (Readable, Writable, Data-Segment)
	.byte 0x4f		// Flags
	.byte 0x00 		// Base address (high)



GlobalDescriptorTable_Tail:


GdtDescriptor:

	.word GlobalDescriptorTable_Tail - GlobalDescriptorTable_Head - 1
	.long GlobalDescriptorTable_Head
	
