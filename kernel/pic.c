#include "pic.h"
#include "asm.h"


#define MPIC_COMMAND_PORT 	0x20
#define MPIC_DATA_PORT		0x21
#define SPIC_COMMAND_PORT 	0xA0
#define SPIC_DATA_PORT		0xA1

#define ICW1_ICW4(x) 		((x) << 0)
#define ICW1_RESERVED(x)	((x) << 4)
#define MPIC_ICW1			(ICW1_ICW4(1) | ICW1_RESERVED(1))
#define SPIC_ICW1			(ICW1_ICW4(1) | ICW1_RESERVED(1))

#define ICW2_IDT(x)	((x) << 0)
#define MPIC_ICW2	(ICW2_IDT(32))
#define SPIC_ICW2	(ICW2_IDT(40))

#define ICW3_S2(x)	((x) << 2)
#define ICW3_ID(x)	((x) << 0)
#define MPIC_ICW3	(ICW3_S2(1))
#define SPIC_ICW3	(ICW3_ID(2))

#define ICW4_uPM(x)	((x) << 0)
#define MPIC_ICW4	(ICW4_uPM(1))
#define SPIC_ICW4	(ICW4_uPM(1))

#define MPIC_OCW1	(~0x40)
#define SPIC_OCW1	(~0)

#define OCW2_EOI	(1 << 5)


void send_eoi(int num)
{
	outb(OCW2_EOI, MPIC_COMMAND_PORT);

	if (num >= 8)
		outb(OCW2_EOI, SPIC_COMMAND_PORT);
}

void init_pic()
{
	outb(MPIC_ICW1, MPIC_COMMAND_PORT);
	outb(SPIC_ICW1, SPIC_COMMAND_PORT);

	outb(MPIC_ICW2, MPIC_DATA_PORT);
	outb(SPIC_ICW2, SPIC_DATA_PORT);

	outb(MPIC_ICW3, MPIC_DATA_PORT);
	outb(SPIC_ICW3, SPIC_DATA_PORT);

	outb(MPIC_ICW4, MPIC_DATA_PORT);
	outb(SPIC_ICW4, SPIC_DATA_PORT);

	outb(MPIC_OCW1, MPIC_DATA_PORT);
	outb(SPIC_OCW1, SPIC_DATA_PORT);
}
