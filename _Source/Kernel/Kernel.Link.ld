OUTPUT_FORMAT("binary")

ENTRY(KernelEntry)

SECTIONS
{
	. = 0x1000;

	.text :
	{
		*(.KernelBoot)
		*(.text)
	}


	.data :
	{
		*(.Gdt)
		*(.data)
		*(.rodata)
	}

	.bss :
	{
		*(COMMON)
		*(.bss)
	}
}
