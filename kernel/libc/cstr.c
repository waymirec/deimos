#include "cstr.h"

#include <math.h>

size_t __int_to_hexstring(uint64_t value, uint8_t size, char *const buf);

const char* __hexChars = "0123456789ABCDEF";

size_t uint_to_string(uint64_t value, char* const buf)
{
    uint8_t size = 0;
    uint64_t test = value;
    while(test / 10 > 0)
    {
      size++;
      test /= 10;  
    }

    uint8_t index = 0;
    while (value / 10 > 0)
    {
        uint8_t rem = value % 10;
        value /= 10;
        buf[size - index++] = rem + '0';
    }

    uint8_t rem = value % 10;
    buf[size - index] = rem + '0';
    buf[index+1] = 0;
    return index+1;
}

size_t int_to_string(int64_t value, char* const buf)
{
    uint8_t offset = 0;
    if (value < 0)
    {
        buf[0] = '-';
        offset = 1;
        value *= -1;
    }
    return uint_to_string((uint64_t)value, buf+offset) + offset;
}

size_t float_to_string(float value, uint8_t precision, char* const buf)
{
    size_t intLength = int_to_string((int64_t)value, buf);
    buf[intLength] = '.';

    float fractional = abs(value) - (int64_t)abs(value);
    int64_t t = (int64_t)(fractional * pow(10, precision));
    size_t fracLength = int_to_string(t, buf + (intLength + 1));

    return 1;
}

size_t double_to_string(double value, uint8_t precision, char* const buf)
{
    size_t intLength = int_to_string((int64_t)value, buf);
    buf[intLength] = '.';
 
    
    double fractional = abs(value) - (int64_t)abs(value);
    int64_t t = (int64_t)(fractional * pow(10, precision));
    size_t fracLength = int_to_string(t, buf + (intLength + 1));

    return 1;
}

size_t uint8_to_hexstring(uint8_t value, char *const buf)
{
    return __int_to_hexstring(value, 1, buf);
}

size_t uint16_to_hexstring(uint16_t value, char *const buf)
{
    return __int_to_hexstring(value, 2, buf);
}

size_t uint32_to_hexstring(uint32_t value, char *const buf)
{
    return __int_to_hexstring(value, 4, buf);
}

size_t uint64_to_hexstring(uint64_t value, char *const buf)
{
    return __int_to_hexstring(value, 8, buf);
}

size_t uint_to_hexstring(uint64_t value, char *const buf)
{
    return __int_to_hexstring(value, 0, buf);
}


size_t __int_to_hexstring(uint64_t value, uint8_t numBytes, char *const buf)
{
    uint8_t nibbles = numBytes * 2;
    size_t index = 0;
    size_t size = 0;
    char tmp[16];
    
    while(value > 0)
    {
        tmp[index++] = __hexChars[value % 16];
        value /= 16;
        size++;
    }

    uint8_t padding = nibbles > 0 ? nibbles - index : 0;
    for(int i = 0; i < padding; i++)
    {
        buf[i] = '0';
        size++;
    }

    for (int i = index-1, j = padding; i >= 0; i--,j++)
    {
        buf[j] = tmp[i];
    }

    buf[size] = 0;
    return size;   
}