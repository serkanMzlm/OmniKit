#pragma once 

#include <cstdint>

namespace omnikit::time {

/// @brief A simple stopwatch for measuring elapsed time and rates.
///        Uses a steady clock, so results are never affected by system
///        clock changes (NTP, daylight saving, manual adjustments).
class TicToc {
public:
    /// @brief Constructs the timer and starts it.
    TicToc();

    /// @brief Starts or restarts the timer from now.
    void tic();

    /// @brief Elapsed time since the last tic(), in milliseconds, and
    ///        resets the reference to now (so the next toc() measures
    ///        the next interval).
    double toc();

    /// @brief Frequency in hertz, based on the interval since the last
    ///        tic(): how many times per second this interval would repeat.
    ///        Also resets the reference to now.
    double frequency();

private:
    uint64_t last_;  // reference timestamp in nanoseconds
};

/// @brief Current steady-clock time in nanoseconds.
uint64_t currentTimeNs();

/// @brief Current steady-clock time in microseconds (1000 ns).
uint64_t currentTimeUs();

/// @brief Current steady-clock time in milliseconds (1000 us).
uint64_t currentTimeMs();

/// @brief Current steady-clock time in seconds.
uint64_t currentTimeSec();

}  // namespace omnikit::time