#include <cmath>
#include <string>

#include "omnikit/math/math.hpp"

#include "test_framework.hpp"

using omnikit::math::constrain;
using omnikit::math::fastInverseSqrt;
using omnikit::math::remap;


void test_fast_inverse_sqrt() {
    const float inputs[] = {1.0F, 4.0F, 16.0F, 100.0F, 0.25F};

    for (float x : inputs) {
        const float approx = fastInverseSqrt(x);
        const float exact = 1.0F / std::sqrt(x);
        const float relative_error = std::fabs(approx - exact) / exact;

        CHECK(relative_error < 0.02F);  // within 2%
    }
}

int main() {
    CHECK(constrain(5, 0, 10) == 5);
    CHECK(constrain(15, 0, 10) == 10);
    CHECK(constrain(0, 0, 10) == 0);
    CHECK(constrain(5, 10, 0) == 10);

    CHECK(remap(5.0, 0.0, 10.0, 0.0, 100.0) == 50.0);
    CHECK(remap(0.0, 0.0, 10.0, 0.0, 100.0) == 0.0);
    CHECK(remap(10.0, 0.0, 10.0, 0.0, 100.0) == 100.0);

    test_fast_inverse_sqrt();

    using namespace omnikit::test;
    std::printf("\n%d checks, %d failed\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}