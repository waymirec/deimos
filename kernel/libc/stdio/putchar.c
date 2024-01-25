#include <stdio.h>
#include <hal/console.h>

int putchar(int ic) {
	char c = (char) ic;
    console_putc(c);
	return ic;
}