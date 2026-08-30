#pragma once

namespace omnikit::controller {

/// @brief Strongly-typed wrappers to prevent argument-swap mistakes at call sites.
///        Each is a trivial, single-double aggregate: zero runtime cost, passed in
///        registers exactly like a bare double.
struct Setpoint {
    double value;
};
struct Measurement {
    double value;
};
struct DeltaTime {
    double value;
};

class PID {
public:
    PID(double kp, double ki, double kd);

    double update(Setpoint setpoint, Measurement measurement, DeltaTime dt);
    void reset();

    /// @brief Sets symmetric limits on the accumulated integral term,
    ///        to prevent integral windup. Use a large value to disable.
    void setIntegralLimit(double limit);

    /// @brief Sets symmetric limits on the final output.
    void setOutputLimit(double limit);

private:
    double kp_;
    double ki_;
    double kd_;

    double integral_{0.0};
    double prev_error_{0.0};

    double integral_limit_;
    double output_limit_;
};

} // namespace omnikit::controller