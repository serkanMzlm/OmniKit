#include "framework.hpp"
#include "omnikit/time/timer.hpp"

#include <chrono>
#include <string>
#include <thread>

using omnikit::time::currentTimeMs;
using omnikit::time::currentTimeNs;
using omnikit::time::currentTimeSec;
using omnikit::time::currentTimeUs;
using omnikit::time::TicToc;

namespace {
void sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
} // namespace

void test_toc_measures_elapsed() {
    TicToc timer;
    timer.tic();
    sleepMs(50);
    const double ms = timer.toc();

    CHECK(ms >= 40.0);
    CHECK(ms < 500.0);
}

void test_frequency() {
    TicToc timer;
    timer.tic();
    sleepMs(100);
    const double hz = timer.frequency();

    CHECK(hz > 2.0);
    CHECK(hz < 20.0);
}

void test_clock_is_monotonic() {
    const uint64_t a = currentTimeNs();
    sleepMs(10);
    const uint64_t b = currentTimeNs();

    CHECK(b >= a);
    CHECK((b - a) >= 5000000ULL);
}

int main() {
    test_toc_measures_elapsed();
    test_frequency();
    test_clock_is_monotonic();

    using namespace omnikit::test;
    std::printf("\n%d checks, %d failed\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}