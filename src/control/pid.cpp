#include "omnikit/control/pid.hpp"

#include "omnikit/math/math.hpp"

#include <limits>

namespace omnikit::control {

PID::PID(double kp, double ki, double kd)
    : kp_(kp), ki_(ki), kd_(kd), integral_(0.0), prev_error_(0.0),
      integral_limit_(std::numeric_limits<double>::max()),
      output_limit_(std::numeric_limits<double>::max()) {}

double PID::update(double setpoint, double measurement, double dt) {
    const double error = setpoint - measurement;

    if (dt <= 0.0) {
        return math::constrain(kp_ * error, -output_limit_, output_limit_);
    }

    const double p_term = kp_ * error;

    integral_ += error * dt;
    integral_ = math::constrain(integral_, -integral_limit_, integral_limit_);
    const double i_term = ki_ * integral_;

    const double derivative = (error - prev_error_) / dt;
    const double d_term = kd_ * derivative;
    prev_error_ = error;

    const double output = p_term + i_term + d_term;
    return math::constrain(output, -output_limit_, output_limit_);
}

void PID::reset() {
    integral_ = 0.0;
    prev_error_ = 0.0;
}

void PID::setIntegralLimit(double limit) {
    integral_limit_ = limit;
}

void PID::setOutputLimit(double limit) {
    output_limit_ = limit;
}

} // namespace omnikit::control