#include "omnikit/filter/complementary.hpp"

#include "omnikit/math/math.hpp"

namespace omnikit::filter {

ComplementaryFilter::ComplementaryFilter(double alpha) : alpha_(math::constrain(alpha, 0.0, 1.0)) {}

double ComplementaryFilter::update(double fast_input, double slow_input, double dt) {
    // Integrate the fast signal onto the previous estimate, then blend
    // with the slow signal to correct long-term drift.
    const double predicted = estimate_ + fast_input * dt;
    estimate_ = alpha_ * predicted + (1.0 - alpha_) * slow_input;
    return estimate_;
}

void ComplementaryFilter::reset(double value) {
    estimate_ = value;
}

} // namespace omnikit::filter