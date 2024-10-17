OUTPUT_FORMAT("binary")

ENTRY(kernel_start)

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
		*(.data)
		*(.rodata)
	}

	.bss :
	{
		*(COMMON)
		*(.bss)
	}
}
