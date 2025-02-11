#include <kernel.h>
#include <stddef.h>
#include <stdint.h>

#include <vga.h>

void kmain(void)
{

    // Clear the screen
    clear();

	// Print welcome message
	print("Freax OS Booting...\n", 0x0F);
	print("Welcome to ", 0x0E);
	print("Freax", 0x0A);
	print("!\n", 0x0E);

	// Halt the CPU
	while (1) {
		__asm__ volatile("hlt");
	}
}
