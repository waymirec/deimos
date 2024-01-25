#ifndef DEIMOS_PAGEFRAME_ALLOCATOR_H
#define DEIMOS_PAGEFRAME_ALLOCATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <hal/memory.h>
#include <hal/paging.h>
#include <util/bitmap.h>


void pageframe_allocator_init(memory_info_t *memory_info);
bool pageframe_free(void *address);
void pageframe_nfree(void *address, size_t page_count);
bool pageframe_lock(void *address);
void pageframe_nlock(void *address, size_t page_count);
void* pageframe_request(void);
uint64_t pageframe_memory_free(void);
uint64_t pageframe_memory_used(void);
uint64_t pageframe_memory_reserved(void);

#endif //DEIMOS_PAGEFRAME_ALLOCATOR_H