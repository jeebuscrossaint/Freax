#include "shell.h"
#include "teletype.h"

void shell_init() {
	while (1) {
		char c = tty_get_char();
		if (c == '\b') {
			tty_put_char('\b');
		} else {
			tty_put_char(c);
		}
	}
}
