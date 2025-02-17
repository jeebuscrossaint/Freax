#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define PS2_CMD_PORT 0x64 // command port for controller
#define PS2_DATA_PORT 0x60 // Data port for keyboard data

#define PS2_CMD_WRITE_TO_MOUSE 0xD4
#define PS2_CMD_ENABLE 0xAE
#define PS2_CMD_DISABLE 0xAD
#define PS2_CMD_SELF_TEST 0xAA

void ps2_init();
void ps2_wait_for_data();
uint8_t ps2_read_data();
void ps2_write_command(uint8_t command);
void ps2_handler();
void ps2_set_interrupt_handler(void (*handler)(void));

#endif // KEYBOARD_H
