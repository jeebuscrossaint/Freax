#include <isr.h>
#include <vga.h>

void c_isr_handler(void) {
    print("Interrupt received!\n", 0x0F);
}