#ifndef __TIME_UTILS_HPP__
#define __TIME_UTILS_HPP__

#include <chrono>

/**
 * @class TicToc
 * @brief A simple timer class for measuring time intervals.
 */
class TicToc
{
public:
    /// @brief Constructor that initializes the timer.
    TicToc();

    /// @brief Starts or restarts the timer.
    void tic();

    /**
     * @brief Stops the timer and returns the elapsed time in milliseconds.
     *
     * @return Elapsed time in milliseconds.
     */
    double toc();

private:
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
};

/**
 * @brief Returns the current time in milliseconds using the system clock.
 *
 * @return The current time in milliseconds.
 */
uint64_t timeStepMs();
uint64_t timeStepSec();

#endif
