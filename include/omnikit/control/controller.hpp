#pragma once

namespace omnikit::control {

class Controller {
public:
    Controller() = default;

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;

    virtual ~Controller() = default;
    virtual double update(double setpoint, double measurement, double dt) = 0;
    virtual void reset() = 0;
};

} // namespace omnikit::control