#include <hal/console.h>
#include <hal/display.h>
#include <math.h>

// private method declarations
static void __put_char(const char chr, point_t* pos);
static void __clear_char(point_t* pos);
static void __scroll();

unsigned int __font_height;
unsigned int __font_width;
unsigned int __width;
unsigned int __height;

console_t console;
void console_init(PSF1_FONT* font)
{
    console.font = font;
    console.fgcolor = 0xFFFFFFFF;
    console.bgcolor = 0x00000000;
    console.cursor_pos = (point_t){ 0, 0 };

    __font_height = font->header->size;
    __font_width = font->header->size / 2;
    __height = display_vertical_resolution() / __font_height;
    __width = display_horizontal_resolution() / __font_width;

    console_clear();
}

void console_clear()
{
    display_clear();
    console.cursor_pos = (point_t){ 0, 0 };
}

void console_putc(const char chr)
{
    if (chr == '\n')
    {
        console_newline();
        return;
    }

    __put_char(chr, &console.cursor_pos);

    console.cursor_pos.x = console.cursor_pos.x + 1;
    if (console.cursor_pos.x >= __width) {
        console_newline();
    }
}

void console_puts(const char* str)
{
    for (const char* chr = str; *chr != 0; chr++)
    {
        console_putc(*chr);
    }
}

void console_newline()
{
    console.cursor_pos.x = 0;
    console.cursor_pos.y ++;

    if (console.cursor_pos.y >= __height)
    {
        console.cursor_pos.y = __height - 1;
        __scroll();
    }
}

void console_backspace()
{
    const point_t pos = console.cursor_pos;

    if (pos.x == 0 && pos.y == 0) return;

    if (pos.x == 0) {
        console.cursor_pos.x = __width;
        console.cursor_pos.y--;
    } else {
        console.cursor_pos.x--;
    }

    __clear_char(&console.cursor_pos);
}

void console_move_cursor(unsigned int x, unsigned int y) 
{
    console.cursor_pos = (point_t) { .x = clamp(x, 0, __width), .y = clamp(y, 0, __height) };

}

unsigned int console_height() 
{
    return __height;
}

unsigned int console_width()
{
    return __width;
}

// private method definitions
static void __put_char(const char chr, point_t* pos)
{
    PSF1_FONT* font = console.font;
    char *glyph = (char *)font->buffer + (chr * font->header->size); // index of glyph within glyph array
    unsigned long start_y = pos->y * __font_height;
    unsigned long stop_y = start_y + __font_height;
    unsigned long start_x = pos->x * __font_width;
    unsigned long stop_x = start_x + __font_width;

    for (unsigned long y = start_y; y < stop_y; y++) {
        for (unsigned long x = start_x; x < stop_x; x++) {
            if ((*glyph & (0b10000000 >> (x - start_x))) > 0) {
                display_set_pixel(x, y, console.fgcolor);
            }
        }
        glyph++;
    }
}

static void __clear_char(point_t* pos)
{
    unsigned long start_y = pos->y * __font_height;
    unsigned long stop_y = start_y + __font_height;
    unsigned long start_x = pos->x * __font_width;
    unsigned long stop_x = start_x + __font_width;

    for (unsigned long y = start_y; y < stop_y; y++){
        for (unsigned long x = start_x; x < stop_x; x++){
            display_set_pixel(x, y, console.bgcolor);
        }
    }
}

void __scroll()
{
    for (int y = __font_height; y <= display_vertical_resolution(); y++)
    {
        for (int x = 0; x <= display_horizontal_resolution(); x++)
        {
            unsigned int p = display_get_pixel(x, y);
            display_set_pixel(x, y - __font_height, p);
        }
    }

    for (int y = display_vertical_resolution() - __font_height; y <= display_vertical_resolution(); y++)
    {
        for (int x = 0; x <= display_horizontal_resolution(); x++)
        {
            display_set_pixel(x, y, console.bgcolor);
        }
    }
}
