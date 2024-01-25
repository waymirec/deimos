#ifndef DEIMOS_BOOT_H
#define DEIMOS_BOOT_H

#include <hal/efimem.h>
#include <hal/display.h>
#include "font.h"

typedef struct {
    framebuffer_t* framebuffer;
    PSF1_FONT* font;
    memory_info_t* memory_info;
} boot_info_t;

#endif //DEIMOS_BOOT_H