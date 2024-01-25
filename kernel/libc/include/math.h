#ifndef DEIMOS_MATH_H
#define DEIMOS_MATH_H

#include <stdint.h>
#include <stddef.h>

#include <stdint.h>

typedef struct {
    int x;
    int y;
} point_t;

double pow(double value, int8_t power);
double abs(double value);
double clamp(double value, double min, double max);
#endif //DEMIOS_MATH_H