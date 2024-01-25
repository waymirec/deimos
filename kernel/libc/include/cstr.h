#ifndef DEIMOS_CSTR_H
#define DEIMOS_CSTR_H

#include <stdint.h>
#include <stddef.h>

size_t uint_to_string(uint64_t value, char* const buf);
size_t int_to_string(int64_t value, char* const buf);
size_t float_to_string(float value, uint8_t precision, char* const buf);
size_t double_to_string(double value, uint8_t precision, char* const buf);
size_t uint8_to_hexstring(uint8_t value, char *const buf);
size_t uint16_to_hexstring(uint16_t value, char *const buf);
size_t uint32_to_hexstring(uint32_t value, char *const buf);
size_t uint64_to_hexstring(uint64_t value, char *const buf);
size_t uint_to_hexstring(uint64_t value, char *const buf);

#endif //DEIMOS_CSTR_H