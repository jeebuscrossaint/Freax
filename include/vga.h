#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_MEMORY 0xB8000

#define VGA_COLOR_BLACK 0x0
#define VGA_COLOR_WHITE 0xF

static volatile uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;

void vga_init();

void vga_putc(char c);

void vga_clear();

void vga_set_color(uint8_t fg, uint8_t bg);

#endif // VGA_H
