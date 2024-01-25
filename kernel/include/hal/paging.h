#ifndef DEIMOS_PAGING_H
#define DEIMOS_PAGING_H

#define PAGE_SHIFT      12
#define PAGE_SIZE       (1UL << PAGE_SHIFT)
#define PAGE_MASK       (~(PAGE_SIZE-1))

#endif //DEIMOS_PAGING_H