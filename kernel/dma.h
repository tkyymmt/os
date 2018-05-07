#ifndef DMA_H
#define DMA_H


enum dma_type {
	DMA_WRITE = 0x4,
	DMA_READ = 0x8,
};


extern void dma_set_fdd(enum dma_type type, void *paddr);
extern void init_dma();


#endif // DMA_H
