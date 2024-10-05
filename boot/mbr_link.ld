OUTPUT_FORMAT("binary")

ENTRY(boot_sector_start)

SECTIONS
{
	. = 0x7c00;

	.text :
	{
		*(.text)
	}
}
