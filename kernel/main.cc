#include <stdint.h>
#include <stddef.h>
#include "limine.h"

// Limine requests
static volatile char LIMINE_BASE[0x1000] __attribute__((aligned(0x1000)));

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = NULL
};

// Simple print function that writes to the framebuffer
void print(const char* str) {
    struct limine_framebuffer* fb = framebuffer_request.response->framebuffers[0];
    static size_t x = 0;
    static size_t y = 0;

    for (size_t i = 0; str[i]; i++) {
        // Very basic printing - just writes white pixels for each character
        uint32_t* fb_ptr = (uint32_t*)fb->address;
        fb_ptr[y * fb->pitch / 4 + x] = 0xFFFFFFFF; // White pixel
        x++;
        if (x >= fb->width) {
            x = 0;
            y++;
        }
    }
}

// Kernel entry point
extern "C" void _start(void) {
    // Ensure we got a framebuffer
    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count < 1) {
        // Hang if we don't have a framebuffer
        for (;;) {
            asm ("hlt");
        }
    }

    // Print "Hello World!"
    print("Hello World!");

    // Hang
    for (;;) {
        asm ("hlt");
    }
}
