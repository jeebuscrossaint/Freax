#ifndef VGA_H
#define VGA_H

//
// Created by amarnath on 2/11/25.
//

#include <stddef.h>
#include <stdint.h>
// Simple function to write a string
void print(const char *str, uint8_t color);
void putchar(char c, uint8_t color, size_t x, size_t y);
void clear();

#endif // VGA_H
