#include <hal/display.h>

display_t display;

void display_init(framebuffer_t* framebuffer)
{
    display.framebuffer = framebuffer;
    display.fgcolor = 0xFFFFFFFF;
    display.bgcolor = 0x00000000;

    display_clear();
}

void display_clear()
{
    for (int y = 0; y <= display.framebuffer->vertical_resolution; y++) 
    {
        for (int x = 0; x <= display.framebuffer->horizontal_resolution; x++)
        {
            display_set_pixel(x, y, display.bgcolor);
        }
    }
}

unsigned int display_get_pixel(unsigned int x, unsigned int y)
{
    unsigned int* ptr = (unsigned int*)display.framebuffer->base_address;
    return *(unsigned int*)(ptr + x + (y * display.framebuffer->pixels_per_scan_line));
}

void display_set_pixel(unsigned int x, unsigned int y, unsigned int color)
{
    unsigned int* ptr = (unsigned int*)display.framebuffer->base_address;
    *(unsigned int*)(ptr + x + (y * display.framebuffer->pixels_per_scan_line)) = color;
}

unsigned int display_vertical_resolution()
{
    return display.framebuffer->vertical_resolution;
}

unsigned int display_horizontal_resolution()
{
    return display.framebuffer->horizontal_resolution;
}

unsigned int display_pixels_per_scan_line()
{
    return display.framebuffer->pixels_per_scan_line;
}