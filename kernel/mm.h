#ifndef MM_H
#define MM_H


#include <stdint.h>


extern void alloc_page(uint32_t vaddr);
extern void take_pages(uint8_t pf_nrs[]);
extern void set_pages(uint8_t pf_nrs[]);
extern void init_mm();


#endif // MM_H
