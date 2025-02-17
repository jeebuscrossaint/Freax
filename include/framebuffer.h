#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

// Define the framebuffer width and height
#define FB_WIDTH  80
#define FB_HEIGHT 25

// Function prototypes
void fb_init();
void fb_putc(int x, int y, char c);  // Function to output a character to the framebuffer
void fb_clear();
void fb_clear_line(int line);

#endif // FRAMEBUFFER_H
