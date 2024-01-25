#ifndef DEIMOS_STDIO_H
#define DEIMOS_STDIO_H

#include <stddef.h>

#define EOF (-1)

int printf(const char* __restrict, ...);
int sprintf(char*, const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#endif //DEIMOS_STDIO_H