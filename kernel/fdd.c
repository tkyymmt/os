#include <stdbool.h>
#include <stdint.h>

#include "dma.h"
#include "asm.h"


volatile bool fdd_irq_flag;


static void fdd_wait_irq()
{
	while (!fdd_irq_flag);
	
	fdd_irq_flag = false;
}

static uint8_t read_stts()
{
	while (!(inb(0x03f4) & 0x80));

	return inb(0x03f5);
}

static void write_cmd(uint8_t cmd)
{
	while (!(inb(0x03f4) & 0x80));

	outb(cmd, 0x03f5);
}

static void read_sense_intr_stts()
{
	write_cmd(0x08);
	read_stts(); // ST0
	read_stts(); // PCN
}

static void lba_to_chs(int lba, uint8_t *c, uint8_t *h, uint8_t *s)
{
	*c = lba / (2 * 18);
	*h = ((lba % (2 * 18))) / 18;
	*s = ((lba % (2 * 18))) % 18 + 1;
}

static void seek(uint8_t cylinder, uint8_t head)
{
	write_cmd(0xf);
	write_cmd(head << 2);
	write_cmd(cylinder);

	fdd_wait_irq();
	read_sense_intr_stts();
}

void read_sect(int lba, void *paddr)
{
	uint8_t cylinder, head, sector;

	dma_set_fdd(DMA_WRITE, paddr);

	lba_to_chs(lba, &cylinder, &head, &sector);

	outb(0x1c, 0x03f2); // motor on

	seek(cylinder, head);

	// read data command
	write_cmd(0x46);
	write_cmd(head << 2);
	write_cmd(cylinder);
	write_cmd(head);
	write_cmd(sector);
	write_cmd(2);
	write_cmd(18);
	write_cmd(27);
	write_cmd(0xff);

	fdd_wait_irq();
	for (int i = 0; i < 7; ++i)
		read_stts();

	read_sense_intr_stts();

	outb(0x0c, 0x03f2); // motor off
}

void write_sect(int lba, void *paddr)
{
	uint8_t cylinder, head, sector;

	dma_set_fdd(DMA_READ, paddr);

	lba_to_chs(lba, &cylinder, &head, &sector);

	outb(0x1c, 0x03f2); // motor on

	seek(cylinder, head);

	// write data command
	write_cmd(0x45);
	write_cmd(head << 2);
	write_cmd(cylinder);
	write_cmd(head);
	write_cmd(sector);
	write_cmd(2);
	write_cmd(18);
	write_cmd(27);
	write_cmd(0xff);

	fdd_wait_irq();
	for (int i = 0; i < 7; ++i)
		read_stts();

	read_sense_intr_stts();

	outb(0x0c, 0x03f2); // motor off
}

void init_fdd()
{
	sti();

	// reset
	outb(0x00, 0x03f2);
	outb(0x0c, 0x03f2);

	fdd_wait_irq();
	for (int i = 0; i < 4; ++i)
		read_sense_intr_stts();

/*
	// configure command
	write_cmd(0x13);
	write_cmd(0x00);
	write_cmd(0x57);
	write_cmd(0x00);
*/

	// set data transfer rate
	outb(0x00, 0x03f7);

	// specify command
	write_cmd(0x03);
	write_cmd((0xd << 4) | 0x2);
	write_cmd((0x10 << 1) | 0x0);

	outb(0x1c, 0x03f2); // motor on

	// recalibrate command
	write_cmd(0x07);
	write_cmd(0x00);

	fdd_wait_irq();
	read_sense_intr_stts();

	outb(0x0c, 0x03f2); // motor off

	cli();
}
