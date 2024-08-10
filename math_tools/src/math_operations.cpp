#include <cmath>
#include <cstdint>

#include "math_operations.hpp"

#define THREEHALFS (0x5f3759df)

double map(double data, double in_min, double in_max, double out_min, double out_max)
{
    if (in_min == in_max)
    {
        return out_min;
    }

    double proportion = (data - in_min) / (in_max - in_min);
    return (proportion * (out_max - out_min) + out_min);
}

double constrain(double data, double min_value, double max_value)
{
    if (min_value > max_value)
    {
        return -1;
    }

    return fminf(max_value, fmaxf(min_value, data));
}

float fastInverseSqrt(float number)
{
    union
    {
        float f;
        uint32_t i;
    } conv = {.f = number};

    // Step 1: Initial Value and Bit Manipulation
    conv.i = THREEHALFS - (conv.i >> 1);

    // Step 2: Newton-Raphson Iteration
    conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
    return conv.f;
}
