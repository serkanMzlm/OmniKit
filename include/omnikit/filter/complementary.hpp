#pragma once

namespace omnikit::filter {

struct FastInput {
    double value;
};
struct SlowInput {
    double value;
};

/// @brief A complementary filter fusing a fast (integrated) signal with
///        a slow (absolute) signal. Classic use: gyro + accelerometer
///        for tilt estimation.
class ComplementaryFilter {
public:
    /// @brief Constructs the filter.
    /// @param alpha  weight for the fast signal, in [0, 1]. Higher trusts
    ///               the fast (integrated) input more; (1-alpha) corrects
    ///               drift using the slow input.
    explicit ComplementaryFilter(double alpha);

    /// @brief Fuses one step of the two signals.
    /// @param fast_input  rate to integrate (e.g. gyro angular velocity)
    /// @param slow_input  absolute reference (e.g. accelerometer angle)
    /// @param dt          time step in seconds
    /// @return the fused estimate
    double update(FastInput fast_input, SlowInput slow_input, double dt);

    /// @brief Resets the estimate to a starting value.
    void reset(double value);

    [[nodiscard]] double getEstimate() const { return estimate_; }

private:
    double alpha_;
    double estimate_ = 0.0;
};

} // namespace omnikit::filter