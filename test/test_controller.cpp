#include "framework.hpp"
#include "omnikit/controller/pid.hpp"

#include <cmath>
#include <memory>
#include <string>

using namespace omnikit::controller;

namespace {
/// Approximate equality for doubles (floating-point safe).
bool close(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) < tol;
}
} // namespace

int main() {
    PID pid(0.5, 0.1, 0.0);

    const double setpoint = 10.0;
    double position = 0.0;
    const double dt = 0.1;

    for (int i = 0; i < 500; ++i) {
        const double control = pid.update(Setpoint{setpoint}, Measurement{position}, DeltaTime{dt});
        position += control * dt; // very simple integrator "plant"
    }

    CHECK(close(position, setpoint, 0.5));

    using namespace omnikit::test;
    std::printf("\n%d checks, %d failed\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}