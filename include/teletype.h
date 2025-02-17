#ifndef TELETYPE_H
#define TELETYPE_H

#include <stdint.h>

// Console dimensions
#define TTY_WIDTH 80
#define TTY_HEIGHT 25

// Function declarations
void tty_init();
void tty_putc(char c);
void tty_puts(const char* str);
void tty_newline();
void tty_scroll();
void tty_put_string(const char *str);
char tty_get_char();  // For getting a character from the terminal
void tty_put_char(char c);  // For putting a character on the screen

extern int tty_cursor_x;
extern int tty_cursor_y;

#endif // TELETYPE_H
