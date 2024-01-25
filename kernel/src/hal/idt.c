#include <hal/idt.h>
#include <hal/interrupt_handlers.h>
#include <util/binary.h>
#include <string.h>

idt_entry_t g_idt[256];
idt_descriptor_t _idtr = { sizeof(g_idt) -1, (uint64_t)g_idt };

void idt_init()
{
    memzero((void *)_idtr.base, _idtr.limit);

    idt_set_gate(0xE, int_handler_pagefault, 0x08, IDT_FLAGS_INTERRUPT_GATE);
    idt_set_gate(0x08, int_handler_double_fault, 0x08, IDT_FLAGS_INTERRUPT_GATE);
    idt_set_gate(0x0D, int_handler_general_protection, 0x08, IDT_FLAGS_INTERRUPT_GATE);
    idt_set_gate(0x21, int_handler_keyboard, 0x08, IDT_FLAGS_INTERRUPT_GATE);
    idt_set_gate(0x2C, int_handler_mouse, 0x08, IDT_FLAGS_INTERRUPT_GATE);
    idt_set_gate(0x20, int_handler_pit, 0x08, IDT_FLAGS_INTERRUPT_GATE);

    load_idt(&_idtr);
}

void idt_set_gate(uint8_t interrupt, void *handler, uint8_t selector, uint8_t flags)
{
    uint64_t h = (uint64_t)handler;
    g_idt[interrupt].base_low = IDT_BASE_LOW(h);
    g_idt[interrupt].selector = selector;
    g_idt[interrupt].flags = flags;
    g_idt[interrupt].base_middle = IDT_BASE_MIDDLE(h);
    g_idt[interrupt].base_high = IDT_BASE_HIGH(h);
}

void idt_enable_gate(uint8_t interrupt)
{
    FLAG_SET(g_idt[interrupt].flags, IDT_FLAG_PRESENT);
}

void idt_disable_gate(uint8_t interrupt)
{
    FLAG_UNSET(g_idt[interrupt].flags, IDT_FLAG_PRESENT);
}