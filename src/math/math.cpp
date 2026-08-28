#include "omnikit/math/math.hpp"

#include <cstdint>
#include <cstring>

namespace omnikit::math {

float fastInverseSqrt(float number) {
    const float x2 = number * 0.5F;

    uint32_t i;
    std::memcpy(&i, &number, sizeof(float));

    i = 0x5f3759df - (i >> 1);

    float y;
    std::memcpy(&y, &i, sizeof(float));

    y = y * (1.5F - x2 * y * y);
    return y;
}

} // namespace omnikit::math