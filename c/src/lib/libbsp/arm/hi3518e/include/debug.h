#ifndef _DEBUG_H
#define _DEBUG_H

static inline void phyreg_write(unsigned int addr, unsigned int val){
	*(volatile unsigned int *)(addr) = val;
}

static inline unsigned int phyreg_read(unsigned int addr){
	return *(volatile unsigned int *)(addr);
}


#endif /* _DEBUG_H */
