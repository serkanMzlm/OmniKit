#include "framework.hpp"
#include "omnikit/filter/complementary.hpp"
#include "omnikit/filter/highpass.hpp"
#include "omnikit/filter/lowpass.hpp"

#include <cmath>
#include <string>

using namespace omnikit::filter;

namespace {
bool close(double a, double b, double tol) {
    return std::fabs(a - b) < tol;
}
} // namespace

void test_lpf_settles() {
    LowPassFilter2p lpf(SampleFreq{100.0}, CutoffFreq{10.0}); // 100 Hz sample, 10 Hz cutoff

    double out = 0.0;
    for (int i = 0; i < 200; ++i) {
        out = lpf.apply(5.0);
    }
    CHECK(close(out, 5.0, 0.01));
}

void test_lpf_smooths() {
    LowPassFilter2p lpf(SampleFreq{100.0}, CutoffFreq{5.0});
    lpf.reset(0.0);

    double max_out = 0.0;
    for (int i = 0; i < 100; ++i) {
        const double in = (i % 2 == 0) ? 1.0 : -1.0;
        const double out = lpf.apply(in);
        max_out = std::max(max_out, std::fabs(out));
    }
    CHECK(max_out < 0.5);
}

void test_hpf_removes_constant() {
    HighPassFilter2p hpf(100.0, 5.0);
    hpf.reset(3.0);

    double out = 0.0;
    for (int i = 0; i < 200; ++i) {
        out = hpf.apply(3.0);
    }
    CHECK(close(out, 0.0, 0.05));
}

void test_complementary_fusion() {
    ComplementaryFilter filter(0.98);
    filter.reset(0.0);

    double est = 0.0;
    for (int i = 0; i < 10; ++i) {
        est = filter.update(FastInput{1.0}, SlowInput{0.1}, 0.1); // dt = 0.1
    }

    CHECK(est > 0.0);
    CHECK(est < 1.5);
}

void test_complementary_reset() {
    ComplementaryFilter filter(0.9);
    filter.reset(2.5);
    CHECK(close(filter.getEstimate(), 2.5, 1e-9));
}

int main() {
    test_lpf_settles();
    test_lpf_smooths();
    test_hpf_removes_constant();
    test_complementary_fusion();
    test_complementary_reset();

    using namespace omnikit::test;
    std::printf("\n%d checks, %d failed\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}