#include "shell.h"
#include "teletype.h"
#include "keyboard.h"    // For keyboard handling
#include "vga.h"   // For VGA output (or fb.h for framebuffer)
#include "framebuffer.h"    // For framebuffer output (if using framebuffer)

void kernel_main() {
    // Initialize TTY (keyboard and screen)
    tty_init();

    // Initialize PS/2 keyboard
    ps2_init();

    // Print kernel version
    tty_put_string("Bunix 0.1\n");

    // Initialize the shell
    shell_init();
}
