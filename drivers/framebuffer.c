#include "framebuffer.h"
#include <stdint.h>

// Define the framebuffer memory location (this may vary based on your system)
#define FB_ADDRESS 0xB8000

// Initialize the framebuffer (optional, depending on your system)
void fb_init() {
    // Initialization code (if needed)
}

// Put a character at a specific position (x, y) in the framebuffer
void fb_putc(int x, int y, char c) {
    uint16_t* framebuffer = (uint16_t*)FB_ADDRESS;
    uint16_t color = 0x07;  // White on black (you can modify this as needed)

    framebuffer[(y * FB_WIDTH) + x] = (color << 8) | c;
}

// Clear the framebuffer (optional, if you want to clear the screen at startup)
void fb_clear() {
    uint16_t* framebuffer = (uint16_t*)FB_ADDRESS;
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        framebuffer[i] = (0x07 << 8) | ' ';  // Clear with spaces (white on black)
    }
}

// Clear a specific line in the framebuffer (optional)
void fb_clear_line(int line) {
    uint16_t* framebuffer = (uint16_t*)FB_ADDRESS;
    for (int i = 0; i < FB_WIDTH; i++) {
        framebuffer[(line * FB_WIDTH) + i] = (0x07 << 8) | ' ';
    }
}
