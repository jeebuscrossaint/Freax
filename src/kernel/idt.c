#include <idt.h>

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void set_idt_entry(int n, uint64_t handler, uint16_t selector, uint8_t type_attr) {
  idt[n].offset_low = handler & 0xFFFF;
  idt[n].selector = selector;
  idt[n].zero = 0;
  idt[n].type_attr = type_attr;
  idt[n].offset_high = (handler >> 16) & 0xFFFF;
  idt[n].offset_top = (handler >> 32) & 0xFFFFFFFF;
  idt[n].reserved = 0;
}

void init_idt() {
  idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
  idtp.base = (uint64_t)&idt;

  // Set IDT entries here using set_idt_entry()

  load_idt(&idtp);
}