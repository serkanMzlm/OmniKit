#pragma once

#include "omnikit/control/controller.hpp"

namespace omnikit::control {

class PID : public Controller {
public:
    PID(double kp, double ki, double kd);

    double update(double setpoint, double measurement, double dt) override;
    void reset() override;

    /// @brief Sets symmetric limits on the accumulated integral term,
    ///        to prevent integral windup. Use a large value to disable.
    void setIntegralLimit(double limit);

    /// @brief Sets symmetric limits on the final output.
    void setOutputLimit(double limit);

private:
    double kp_;
    double ki_;
    double kd_;

    double integral_;
    double prev_error_;

    double integral_limit_;
    double output_limit_;
};

} // namespace omnikit::control