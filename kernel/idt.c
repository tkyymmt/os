#include <stdint.h>

#include "isr.h"


#define IDT_GATE(x)		((x) << 0)
#define IDT_D(x)		((x) << 3)
#define IDT_DPL(x)		((x) << 5)
#define IDT_P(x)		((x) << 7)

#define IDT_EXCEPTION	(IDT_GATE(6) | IDT_D(1) | IDT_DPL(0) | IDT_P(1))
#define IDT_IRQ			(IDT_GATE(6) | IDT_D(1) | IDT_DPL(0) | IDT_P(1))
#define IDT_SYSCALL		(IDT_GATE(6) | IDT_D(1) | IDT_DPL(3) | IDT_P(1))


struct idt_entry {
	uint16_t low_offset;
	uint16_t seg_sel;
	uint8_t unused;
	uint8_t flags;
	uint16_t high_offset;
} __attribute__((packed));

struct idt_register {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));


struct idt_entry idt[256];
struct idt_register idtr;


static void set_idt_entry(int isr_nr, void *isr, uint16_t seg_sel, uint8_t flags)
{
	idt[isr_nr].low_offset = (uint16_t)((uint32_t)isr & 0xFFFF);
	idt[isr_nr].seg_sel = seg_sel;
	idt[isr_nr].unused = 0x0;
	idt[isr_nr].flags = flags;
	idt[isr_nr].high_offset = (uint16_t)((uint32_t)isr >> 16);
}

void init_idt()
{
	//set_idt_entry(0, divide_by_zero, 0x08, IDT_EXCEPTION);
	set_idt_entry(14, page_fault, 0x08, IDT_EXCEPTION);
	set_idt_entry(38, irq_fdd, 0x08, IDT_IRQ);
	set_idt_entry(0x80, syscall, 0x08, IDT_SYSCALL);

	idtr.limit = sizeof(idt) - 1;
	idtr.base = (uint32_t)idt;

	asm volatile("lidt %0" :: "m" (idtr));
}
