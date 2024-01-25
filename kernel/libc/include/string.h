#ifndef DEIMOS_STRING_H
#define DEIMOS_STRING_H

#include <stddef.h>

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memrcpy(void *dest, const void *src, size_t len);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
void* memzero(void *dest, size_t len);
size_t strlen(const char*);
size_t strcpy(void *dstptr, const void *srcptr);

#endif //DEIMOS_STRING_H
