#ifndef DEIMOS_PAGETABLE_MANAGER_H
#define DEIMOS_PAGETABLE_MANAGER_H

#include <stdint.h>
#include <boot.h>

typedef struct mapping_table {
    uint64_t entries[512];
}__attribute__((packed,aligned(4096))) mapping_table_t;

typedef struct mapping_table pml4_t;

void pagetable_init(pml4_t *pml4, boot_info_t *boot_info);
void pagetable_map(pml4_t *pml4, void *logical_address, void *physical_address);
void pagetable_identity_map(pml4_t *pml4, void *start, size_t page_count);

#endif //DEIMOS_PAGETABLE_MANAGER_H