#ifndef _DEBUG_H
#define _DEBUG_H

static inline void phyreg_write(uint32_t addr, uint32_t val){
	*(volatile unsigned int *)(addr) = val;
}

static inline uint32_t phyreg_read(uint32_t addr){
	return *(volatile unsigned int *)(addr);
}


#endif /* _DEBUG_H */
