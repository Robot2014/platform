/*
*
*  add by jundy.chen  
*  mail :jundi0808@cloud.com 
*/

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
/*OUTPUT_FORMAT("elf32-arm", "elf32-arm", "elf32-arm")*/
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)


/* default stack sizes.

These are used by the startup in order to allocate stacks for the different modes.
*/

__stack_size = 0x400 ;

__stack_start = __stack_top  - __stack_size ;

/*"PROVIDE" allows to easily override these values from an object file or the commmand line.*/
PROVIDE ( _Stack_Init = __stack_start ) ;

/*
There will be a link error if there is not this amount of RAM free at the end.
*/
_min_stack_size = 0x100 ;


MEMORY {
/*Adust LENGTH to RAMsize of target MCU:*/
/*STM32F103RBT --> 20K*/
/*RAM (RWX) : ORIGIN = 0x20000000 , LENGTH = 20K*/
/*STM32F103RET --> 64K*/
    SRAM (RWX) : ORIGIN = 0x20000000 , LENGTH = 64K
    EXTSRAM (RWX) : ORIGIN = 0x68000000 , LENGTH = 0

/*Adust LENGTH to (FLASHsize - FeePROMsize) of target MCU:*/
/*STM32F103RBT --> 128k*/
	FLASH (RX) : ORIGIN = 0x08000000 , LENGTH = 128K

/*STM32F103RET --> 508K*/
/*FLASH (RX) : ORIGIN = 0x08000000 , LENGTH = 508K*/

/*STM32F103ZET --> 508K*/
/*FLASH (RX) : ORIGIN = 0x08000000 , LENGTH = 508K */

/*Adust ORIGIN to (0x08000000 + (FLASHsize-FeePROMsize)) of target MCU*/
/*and adust LENGTH to FeePROMsize allocated:*/
/*STM32F103RBT --> 0x08000000+126K, 2K*/
EEMUL (RWX) : ORIGIN = 0x08000000+126K, LENGTH = 2K

/*STM32F103RET --> 0x08000000+508K, 4K*/
/*EEMUL (RWX) : ORIGIN = 0x08000000+508K, LENGTH = 4K*/
}

/* stack end is ram end  base + 64k */
__stack_top = 20010000 ;

SECTIONS
{
    /* for Cortex devices, the beginning of the startup code is stored in the .isr_vector section, which goes to FLASH */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))            /* Startup code */
        . = ALIGN(4);
    } >FLASH

  	/* the program code is stored in the .text section, which goes to Flash */
    .text :
    {
        . = ALIGN(4);
            
        *(.text)                   /* remaining code */
        *(.text.*)                   /* remaining code */
        *(.rodata)                 /* read-only data (constants) */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)

        . = ALIGN(4);
         __text_end = .;
        /* This is used by the startup in order to initialize the .data secion */
         __sidata = __text_end;
    } >FLASH

    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
	.data : AT ( __sidata )	
	{ 
		__data_start = .;
	  	*(.data)
	  	. = ALIGN(4);
	  	__data_end = . ;
	} > SRAM
	
	/* .bss section which is used for uninitialized data */
	.bss (NOLOAD) :
	{
	  __bss_start = . ;
	  *(.bss)
	  . = ALIGN(4);
	  __bss_end = . ;
	} > SRAM
	
    PROVIDE ( end = __bss_end );
    PROVIDE ( _end = __bss_end );

	/* This is the user stack section
    This is just to check that there is enough RAM left for the User mode stack
    It should generate an error if it's full.
    */
    ._usrstack :
    {
		. = ALIGN(4);
        _susrstack = . ;
        
        . = . + _min_stack_size ;
        
        . = ALIGN(4);
        _eusrstack = . ;
    } >SRAM

}
