// typedef unsigned long long size_t;

#include <stdint.h>
#include <hal/serial.h>
#include <hal/gdt.h>
#include <hal/idt.h>
#include <hal/memory.h>
#include <hal/pageframe_allocator.h>
#include <hal/pagetable_manager.h>
#include <util/bitmap.h>
#include <hal/console.h>

#include "kernel.h"
#include "cstr.h"
#include "boot.h"
#include <stdio.h>

pml4_t *g_pml4 = NULL;

void setup_paging(boot_info_t* boot_info);
void display_banner(boot_info_t* boot_info);

void _start(boot_info_t* boot_info){
    serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_write(SERIAL_COM1_BASE, 'Z');

    display_init(boot_info->framebuffer);
    console_init(boot_info->font);
    gdt_init();
    idt_init();
    setup_paging(boot_info);

    display_banner(boot_info);
    
    while(true);
}

void setup_paging(boot_info_t* boot_info)
{
    pageframe_allocator_init(boot_info->memory_info);
    g_pml4 = (pml4_t*)pageframe_request();
    pagetable_init(g_pml4, boot_info);
}

void display_banner(boot_info_t* boot_info)
{
    uint64_t memorySize = system_memory_size(boot_info->memory_info);
    printf("Welcomen to DEIMOS\n");
    printf("Total Memory: %u KB\n", memorySize / 1024);
    printf(" Free Memory: %u KB\n", pageframe_memory_free() / 1024);
    printf(" Used Memory: %u KB\n", pageframe_memory_used() / 1024);
    printf(" Rsvd Memory: %u KB\n", pageframe_memory_reserved() / 1024);
}