/************************************************************************/
/*                                                                      */
/*  Linker Script File                                                  */
/*                                                                      */
/************************************************************************/
/* We have the following memory area's. 							    */
MEMORY	{ RAM		(rwx) : ORIGIN = 0x4000000, LENGTH = 0x0006C00	}
MEMORY	{ FLASH		(rx)  : ORIGIN = 0x6000000, LENGTH = 0x007FFFF	}

SECTIONS
{
	.version 0x06000000 : 
	{
		*(.version)
	} > FLASH
	
	.checksum 0x06000010 : 
	{
		*(APP_INFO)
		__cs_end = .;
	} > FLASH

	PROGRAM __cs_end :
	{
		__text = .;
		*(APP_START)
		*(.OS_HEADER)
		*(.text)
		*(.text.*)
		*(P)
		*(.rodata)
		*(.rodata.*)
		*(C_1)
		*(C_2)
		*(C)
		__etext = . ;
	} > FLASH
	. = ALIGN(4);
	
	.data 0x04000000 : AT (__etext)
	{
		__data = .;
		*(.data)
		*(.data.*)
		*(D)
		*(D_1)
		*(D_2)
		__edata = .;
	} > RAM
	
	.bss (NOLOAD):
	{
		__bss = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		*(B)
		*(B_1)
		*(B_2)
		__ebss = .;
	} > RAM

}





