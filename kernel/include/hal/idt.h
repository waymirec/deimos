#ifndef DEIMOS_IDT_H
#define DEIMOS_IDT_H

#include <stdint.h>

#define MAX_NUM_IDT_ENTRIES             256
#define IDT_FLAGS_INTERRUPT_GATE        0b10001110
#define IDT_FLAGS_CALL_GATE             0b10001100
#define IDT_FLAGS_TRAP_GATE             0b10001111

#define IDT_BASE_LOW(addr)              (uint16_t)(addr & 0xFFFF)
#define IDT_BASE_MIDDLE(addr)           (uint16_t)((addr >> 16) & 0xFFFF)
#define IDT_BASE_HIGH(addr)             (uint16_t)((addr >> 32) & 0xFFFFFFFF)
#define IDT_BASE(idt_entry)             ((uint64_t)idt_entry.base_low | ((uint64_t)idt_entry.base_middle << 16) | ((uint64_t)idt_entry.base_high << 32))

typedef enum {
    IDT_FLAG_INT_GATE                   = 0xE,
    IDT_FLAG_TRAP_GATE                  = 0xF,
    IDT_FLAG_PRESENT                    = 0X80
} IDT_FLAGS;

typedef struct {
    uint16_t limit;
    uint64_t base;
}__attribute__((packed)) idt_descriptor_t;

typedef struct {
      uint16_t base_low ;       // offset bits 0..15
      uint16_t selector;        // a code segment selector in GDT or LDT
      uint8_t ist;              // bites 0..2 hold Interrupt Stack Table offset, rest of bits is zero
      uint8_t flags;            // type and attributes
      uint16_t base_middle;     // offset bits 16..31
      uint32_t base_high;       // offset bits 32..63
      uint32_t reserved;        // zero
}__attribute__((packed)) idt_entry_t;

extern void load_idt(idt_descriptor_t* idt_descriptor);
void idt_init();
void idt_set_gate(uint8_t offset, void* handler, uint8_t selector, uint8_t flags);
void idt_enable_gate(uint8_t interrupt);
void idt_disable_gate(uint8_t interrupt);

#endif //DEIMOS_IDT_H
