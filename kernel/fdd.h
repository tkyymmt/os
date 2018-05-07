#ifndef FDD_H
#define FDD_H


#include <stdbool.h>


extern volatile bool fdd_irq_flag;


extern void read_sect(int lba, void *paddr);
extern void write_sect(int lba, void *paddr);
extern void init_fdd();


#endif // FDD_H
