#include "teletype.h"
#include "framebuffer.h"  // Assuming framebuffer is being used

int tty_cursor_x = 0;
int tty_cursor_y = 0;

// Initialize the terminal
void tty_init() {
    tty_cursor_x = 0;
    tty_cursor_y = 0;
    fb_clear();  // Clear framebuffer at startup
}

// Output a single character to the terminal
void tty_putc(char c) {
    if (c == '\n') {
        tty_newline();
    } else {
        fb_putc(tty_cursor_x, tty_cursor_y, c);  // Use fb_putc to print characters
        tty_cursor_x++;
        if (tty_cursor_x >= TTY_WIDTH) {
            tty_newline();
        }
    }
}

// Output a string to the terminal
void tty_puts(const char* str) {
    while (*str) {
        tty_putc(*str++);
    }
}

// Move to the next line
void tty_newline() {
    tty_cursor_x = 0;
    tty_cursor_y++;
    if (tty_cursor_y >= TTY_HEIGHT) {
        tty_scroll();
    }
}

// Scroll the terminal output (move all lines up)
void tty_scroll() {
    for (int i = 0; i < TTY_HEIGHT - 1; i++) {
        fb_clear_line(i);  // Clear the current line
    }
    fb_clear_line(TTY_HEIGHT - 1);  // Clear the last line
}

// Output a string with newline support
void tty_put_string(const char *str) {
    while (*str) {
        tty_putc(*str++);
    }
}

// Get a character from the user input (this is just a placeholder for now)
char tty_get_char() {
    // You can implement a method to read input, e.g., from keyboard buffer or console
    // This is a very basic placeholder; you would need an interrupt or polling mechanism
    // In this placeholder, we just return a dummy character (e.g., 'A') for now.
    return 'A';  // Modify this with actual input retrieval logic
}

// Output a single character on the screen
void tty_put_char(char c) {
    tty_putc(c);  // Use tty_putc to output a character
}
