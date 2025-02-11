#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <stddef.h>

#define IDT_ENTRIES 256

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_high;
  uint32_t offset_top;
  uint32_t reserved;
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

extern struct idt_entry idt[IDT_ENTRIES];
extern struct idt_ptr idtp;

void set_idt_entry(int n, uint64_t handler, uint16_t selector, uint8_t type_attr);
extern void load_idt(struct idt_ptr *idt_p);
void init_idt();

#endif // IDT_H