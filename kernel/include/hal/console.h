#ifndef DEIMOS_CONSOLE_H
#define DEIMOS_CONSOLE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <hal/display.h>
#include <math.h>
#include "../font.h"

typedef struct {
    unsigned int fgcolor;
    unsigned int bgcolor;
    display_t* display;
    PSF1_FONT* font;
    point_t cursor_pos;
} console_t;

void console_init(PSF1_FONT* font);
void console_clear();
void console_newline();
void console_putc(const char chr);
void console_puts(const char* str);
void console_move_cursor(unsigned int x, unsigned int y);
void console_backspace();
unsigned int console_height();
unsigned int console_width();

#endif //DEIMOS_CONSOLE_H
