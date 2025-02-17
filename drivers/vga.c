#include "vga.h"

// Initialize VGA text mode
void vga_init() {
    // VGA text mode does not require much initialization,
    // as it is just writing directly to VGA memory (0xB8000).
    // You could set up the display resolution, or other modes if needed.
}

// Set the VGA color (foreground and background)
void vga_set_color(uint8_t fg, uint8_t bg) {
    // Combine foreground and background colors into one byte
    uint8_t color = (bg << 4) | (fg & 0x0F);
    vga_buffer = (volatile uint16_t*)VGA_MEMORY;
}

// Put a character onto the screen at the current cursor position
void vga_putc(char c) {
    static uint16_t* cursor = (uint16_t*)VGA_MEMORY;
    if (c == '\n') {
        cursor += VGA_WIDTH - (cursor - (uint16_t*)VGA_MEMORY) % VGA_WIDTH;
    } else {
        *cursor++ = (uint8_t)VGA_COLOR_WHITE << 8 | c; // White text on black background
    }
}

// Clear the screen
void vga_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (uint8_t)VGA_COLOR_BLACK << 8 | ' '; // Fill with spaces (black background)
    }
}
