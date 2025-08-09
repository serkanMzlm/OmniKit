#include "scalar.hpp"

#define THREEHALFS (0x5f3759df)

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