#ifndef SHELL_H
#define SHELL_H

// Initialize the shell (usually called after kernel starts)
void shell_init();

// Function to process a command (simplified for now)
void process_command(const char* command);

#endif
