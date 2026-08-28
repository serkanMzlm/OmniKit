#include "omnikit/time/timer.hpp"

#include <chrono>

namespace omnikit::time {

TicToc::TicToc() : last_(currentTimeNs()) {}

void TicToc::tic() {
    last_ = currentTimeNs();
}

double TicToc::toc() {
    const uint64_t now = currentTimeNs();
    const double elapsed_ms = static_cast<double>(now - last_) / 1.0e6;
    last_ = now;
    return elapsed_ms;
}

double TicToc::frequency() {
    const double ms = toc();
    if (ms <= 0.0) {
        return 0.0;
    }

    return 1000.0 / ms;
}

uint64_t currentTimeNs() {
    const auto now =
        std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
    return static_cast<std::uint64_t>(now.time_since_epoch().count());
}

uint64_t currentTimeUs() {
    const auto now =
        std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    return static_cast<std::uint64_t>(now.time_since_epoch().count());
}

uint64_t currentTimeMs() {
    const auto now =
        std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    return static_cast<std::uint64_t>(now.time_since_epoch().count());
}

uint64_t currentTimeSec() {
    const auto now =
        std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
    return static_cast<std::uint64_t>(now.time_since_epoch().count());
}

} // namespace omnikit::time