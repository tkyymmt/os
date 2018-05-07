#ifndef ASM_H
#define ASM_H


#include <stdint.h>


static inline
uint8_t inb(uint16_t port)
{
	uint8_t data;

	asm volatile("inb	%1, %0"
				: "=a" (data)
				: "d" (port));

	return data;
}

static inline
void outb(uint8_t data, uint16_t port)
{
	asm volatile("outb	%0, %1"
				:: "a" (data), "d" (port));
}

static inline
void sti()
{
	asm volatile("sti");
}

static inline
void cli()
{
	asm volatile("cli");
}


#endif // ASM_H
