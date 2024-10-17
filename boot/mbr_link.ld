OUTPUT_FORMAT("binary")

ENTRY(_mbr_main)

SECTIONS
{
	. = 0x7c00;

	.text :
	{
		*(.text)
	}
}
