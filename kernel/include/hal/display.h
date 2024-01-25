#ifndef DEIMOS_DISPLAY_H
#define DEIMOS_DISPLAY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <hal/display.h>
#include <math.h>

typedef struct {
    void* base_address;
    size_t buffer_size;
    unsigned int horizontal_resolution;
    unsigned int vertical_resolution;
    unsigned int pixels_per_scan_line;
} framebuffer_t;

typedef struct {
    framebuffer_t* framebuffer;
    unsigned int bytes_per_pixel;
    point_t cursor_pos;
    unsigned int fgcolor;
    unsigned int bgcolor;
} display_t;

void display_init(framebuffer_t* framebuffer);
void display_clear();
unsigned int  display_get_pixel(unsigned int x, unsigned int y);
void display_set_pixel(unsigned int x, unsigned int y, unsigned int color);
unsigned int display_vertical_resolution();
unsigned int display_horizontal_resolution();
unsigned int display_pixels_per_scan_line();
#endif //DEIMOS_DISPLAY_H