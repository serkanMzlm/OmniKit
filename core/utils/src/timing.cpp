
#include <ctime>
#include <cstdlib>

#include "timing.hpp"

TicToc::TicToc()
{
    tic();
}

void TicToc::tic()
{
    _last = currentTimeUs();
}

double TicToc::toc()
{
    return static_cast<double>((currentTimeUs() - _last) / 1e3);
}

uint64_t currentTimeNs()
{
    const auto now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
    return static_cast<std::uint64_t>(now.time_since_epoch().count());
}

uint64_t currentTimeUs()
{
    const auto now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    return static_cast<std::uint64_t>(now.time_since_epoch().count());
}

uint64_t currentTimeMs()
{
    const auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    return static_cast<std::uint64_t>(now.time_since_epoch().count());
}

uint64_t currentTimeSec()
{
    const auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
    return static_cast<std::uint64_t>(now.time_since_epoch().count());
}