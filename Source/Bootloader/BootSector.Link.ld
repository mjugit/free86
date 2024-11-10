OUTPUT_FORMAT("binary")

ENTRY(BootloaderMain)

SECTIONS
{
	. = 0x7c00;

	.text :
	{
		*(.text)
	}
}
