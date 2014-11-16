ENTRY(__entry_point)

VIRT_BASE = 0xFFFFFFFF80000000;

SECTIONS
{
	. = 0x100000;

	.boot :
	{
		*(.mbhdr)
		_loadStart = .;
		*(.boot)
		. = ALIGN(4096);
		__boot_pml4 = .;
		. += 0x1000;
		__boot_pml3 = .;
		. += 0x1000;
		__boot_pml2 = .;
		. += 0x1000;

		. += 0x8000;
		__stack = .;
	}

	. += VIRT_BASE;

	.text ALIGN(0x1000) : AT(ADDR(.text) - VIRT_BASE)
	{
		*(.text*)
		*(.gnu.linkonce.t*)
	}

	.data ALIGN(0x1000) : AT(ADDR(.data) - VIRT_BASE)
	{
		*(.data*)
		*(.gnu.linkonce.d*)
	}

	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - VIRT_BASE)
	{
		*(.rodata*)
		*(.gnu.linkonce.r*)
	}

	__load_end = . - VIRT_BASE;

	.bss ALIGN(0x1000) : AT(ADDR(.bss) - VIRT_BASE)
	{
		*(COMMON)
		*(.bss*)
		*(.gnu.linkonce.b*)
	}

	__bss_end = . - VIRT_BASE;

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
	}
}

