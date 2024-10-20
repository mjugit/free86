OUTPUT_FORMAT("binary")

ENTRY(_kernel_boot)

SECTIONS
{
	. = 0x1000;

	.text :
	{
		*(.kernel_boot)
		*(.text)
	}


	.data :
	{
		*(.gdt)
		*(.data)
		*(.rodata)
	}

	.bss :
	{
		*(COMMON)
		*(.bss)
	}
}
