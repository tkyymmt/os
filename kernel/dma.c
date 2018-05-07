#include <stdint.h>

#include "dma.h"
#include "asm.h"


void dma_set_fdd(enum dma_type type, void *paddr)
{
	// mask channel 2
	outb(0x06, 0x000a);

	outb(0x42 | type, 0x000b);

	outb(0xff, 0x000c); // reset flip-flop

	outb((uint32_t)paddr & 0xff, 0x0004);
	outb((uint32_t)paddr >> 8 & 0xff, 0x0004);

	outb(0xff, 0x000c); // reset flip-flop

	outb((512 - 1) & 0xff, 0x0005);
	outb((512 - 1) >> 8 & 0xff, 0x0005);

	outb((uint32_t)paddr >> 16 & 0xff, 0x0081);

	// unmask channel 2
	outb(0x02, 0x000a);
}

void init_dma()
{
	outb(0x0b, 0x000f);
}
