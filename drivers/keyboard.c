#include "keyboard.h"
#include <stdint.h>

// Global variables for buffering and tracking keyboard input
volatile uint8_t ps2_buffer[256];
volatile uint8_t ps2_buffer_index = 0;

// Inline function for port I/O (reading from a port)
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "d"(port));
    return value;
}

// Inline function for port I/O (writing to a port)
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "d"(port));
}

// Initialize the PS/2 controller
void ps2_init() {
    // Disable the PS/2 controller first
    ps2_write_command(PS2_CMD_DISABLE);

    // Perform the self-test for the keyboard
    ps2_write_command(PS2_CMD_SELF_TEST);
    uint8_t test_result = ps2_read_data();
    
    // Check self-test result
    if (test_result == 0x00) {
        // If the test passes, enable the PS/2 controller
        ps2_write_command(PS2_CMD_ENABLE);
    }

    // Set up an interrupt handler for PS/2 keyboard (IRQ1)
    ps2_set_interrupt_handler(ps2_handler);
}

// Wait for data to be available from the PS/2 keyboard
void ps2_wait_for_data() {
    while ((inb(PS2_CMD_PORT) & 0x01) == 0) {
        // Wait until data is available
    }
}

// Read a byte of data from the PS/2 data port
uint8_t ps2_read_data() {
    ps2_wait_for_data();
    return inb(PS2_DATA_PORT);
}

// Write a command to the PS/2 controller
void ps2_write_command(uint8_t command) {
    outb(PS2_CMD_PORT, command);
}

// PS/2 interrupt handler that processes the scancodes
void ps2_handler() {
    uint8_t scancode = ps2_read_data();

    // Store scancode in a buffer for further processing
    if (ps2_buffer_index < sizeof(ps2_buffer)) {
        ps2_buffer[ps2_buffer_index++] = scancode;
    }

    // Example: Printing the received scancode (you may modify this to map to ASCII or another format)
    if (scancode < 0x80) {
        // Handle key press (convert scancode to ASCII, etc.)
    } else {
        // Handle key release (if needed)
    }
}

// Set the interrupt handler for PS/2 keyboard (IRQ1)
void ps2_set_interrupt_handler(void (*handler)(void)) {
    // Assuming an interrupt system is available to register the handler
    // For example, using a PIC (Programmable Interrupt Controller)
    // Register handler for IRQ1 (keyboard interrupt)
    // This depends on your specific interrupt system setup.
}
