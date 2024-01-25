#ifndef DEIMOS_FONT_H
#define DEIMOS_FONT_H

#include <stddef.h>

/*
typedef struct {
    uint32_t count;
    uint32_t size;
    uint32_t height;
    uint32_t width;
    void* buffer;
} font_t;
*/

typedef struct {
    unsigned char magic[2];
    unsigned char mode;
    unsigned char size;
} PSF1_HEADER;

typedef struct {
    PSF1_HEADER *header;
    void *buffer;
} PSF1_FONT;

#endif //DEIMOS_FONT_H