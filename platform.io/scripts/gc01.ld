MEMORY
{
	rom (rx) : ORIGIN = 0x08004000, LENGTH = 48K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

INCLUDE cortex-m-generic.ld
