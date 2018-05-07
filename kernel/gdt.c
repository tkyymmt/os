#include <stdint.h>


#define SEG_TYPE(x)	((x) << 0)
#define SEG_S(x)	((x) << 4)
#define SEG_DPL(x)	((x) << 5)
#define SEG_P(x)	((x) << 7)

#define SEG_AVL(x)	((x) << 0)
#define SEG_DB(x)	((x) << 2)
#define SEG_G(x)	((x) << 3)


#define GDT_ACC_KERNEL_CS	(SEG_TYPE(0xA) | SEG_S(1)  | SEG_DPL(0) | SEG_P(1))
#define GDT_ACC_KERNEL_DS	(SEG_TYPE(0x2) | SEG_S(1)  | SEG_DPL(0) | SEG_P(1))
#define GDT_ACC_USER_CS		(SEG_TYPE(0xA) | SEG_S(1)  | SEG_DPL(3) | SEG_P(1))
#define GDT_ACC_USER_DS		(SEG_TYPE(0x2) | SEG_S(1)  | SEG_DPL(3) | SEG_P(1))
#define GDT_ACC_TSS			(SEG_TYPE(0x9) | SEG_S(0)  | SEG_DPL(0) | SEG_P(1))
#define GDT_FLG_KERNEL		(SEG_AVL(0) | SEG_DB(1) | SEG_G(1))
#define GDT_FLG_USER		(SEG_AVL(0) | SEG_DB(1) | SEG_G(1))
#define GDT_FLG_TSS			(SEG_AVL(0) | SEG_DB(0) | SEG_G(1))


struct gdt_entry {
	uint16_t limit_lo;
	uint16_t base_lo;
	uint8_t base_mid;
	uint8_t access;
	unsigned int limit_hi : 4;
	unsigned int flags: 4;
	uint8_t base_hi;
} __attribute__((packed));

struct gdt_register {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct tss {
    uint16_t link, :16;
    uint32_t esp0;
    uint16_t ss0, :16;
    uint32_t esp1;
    uint16_t ss1, :16;
    uint32_t esp2;
    uint16_t ss2, :16;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint16_t es, :16;
    uint16_t cs, :16;
    uint16_t ss, :16;
    uint16_t ds, :16;
    uint16_t fs, :16;
    uint16_t gs, :16;
    uint16_t ldtr, :16;
    uint16_t trace, iopb_off;
} __attribute__((packed));


struct gdt_entry gdt[6];
struct gdt_register gdtr;
struct tss tss;


static void set_gdt_entry(int idx, uint32_t base, unsigned int limit,
						  uint8_t access, unsigned int flags)
{
	gdt[idx].base_lo = base & 0xFFFF;
	gdt[idx].base_mid = (base >> 16) & 0xFF;
	gdt[idx].base_hi = (base >> 24) & 0xFF;
	gdt[idx].limit_lo = limit & 0xFFFF;
	gdt[idx].limit_hi = (limit >> 16) & 0xF;
	gdt[idx].access = access;
	gdt[idx].flags = flags & 0xf;
}

void init_gdt()
{
	set_gdt_entry(0, 0, 0, 0, 0);
	set_gdt_entry(1, 0, 0xffffffff, GDT_ACC_KERNEL_CS, GDT_FLG_KERNEL);
	set_gdt_entry(2, 0, 0xffffffff, GDT_ACC_KERNEL_DS, GDT_FLG_KERNEL);
	set_gdt_entry(3, 0, 0xffffffff, GDT_ACC_USER_CS, GDT_FLG_USER);
	set_gdt_entry(4, 0, 0xffffffff, GDT_ACC_USER_DS, GDT_FLG_USER);
	set_gdt_entry(5, (uint32_t)&tss, sizeof(tss), GDT_ACC_TSS, GDT_FLG_TSS);

	gdtr.limit = sizeof(gdt) - 1;
	gdtr.base = (uint32_t)gdt;

	tss.esp0 = 0xfff0;
	tss.ss0 = 0x10;

	asm volatile ("lgdt %0" :: "m" (gdtr));
	asm volatile("ltr	%%ax" :: "a" (0x28));
}
