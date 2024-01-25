#include <math.h>

double pow(double value, int8_t power){
    double result = value;
    for(size_t i = 1; i < power; i++)
    {
        result *= value;
    }
    return result;
}

double abs(double value) {
    return value < 0 ? value * -1.0 : value;
}

double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}