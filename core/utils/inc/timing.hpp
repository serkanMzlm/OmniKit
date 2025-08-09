#ifndef TIMING_HPP
#define TIMING_HPP

#include <chrono>

class TicToc
{
public:
    /// @brief Constructor that initializes the timer.
    TicToc();

    /// @brief Starts or restarts the timer.
    void tic();

    /// @brief Stops the timer and returns the elapsed time in milliseconds.
    double toc();

private:
    uint64_t _last;
};

/// @brief Returns the current time in nanoseconds using the system clock.
uint64_t currentTimeNs();

/// @brief Returns the current time in microseconds using the system clock. (1000 nanoseconds)
uint64_t currentTimeUs();

/// @brief Returns the current time in milliseconds using the system clock. (1000 microseconds)
uint64_t currentTimeMs();

/// @brief Returns the current time in seconds using the system clock.
uint64_t currentTimeSec();

#endif
