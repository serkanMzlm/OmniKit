#include "complementary.hpp"
#include <stdexcept>

ComplementaryFilter::ComplementaryFilter(double alpha)
    : alpha_(alpha), previous_value_(0.0)
{
    if (alpha <= 0.0 || alpha >= 1.0) {
        throw std::invalid_argument("Alpha must be in the range (0, 1)");
    }
}

double ComplementaryFilter::update(double primary, double secondary)
{
    double filtered_value = alpha_ * primary + (1 - alpha_) * secondary;
    return filtered_value;
}

double ComplementaryFilter::updatePrimaryOnly(double measurement) 
{
    double filtered_value = alpha_ * measurement + (1 - alpha_) * previous_value_;
    previous_value_ = filtered_value; // Update previous value for next call
    return filtered_value;
}

void ComplementaryFilter::setAlpha(double alpha)
{
    if (alpha <= 0.0 || alpha >= 1.0) {
        throw std::invalid_argument("Alpha must be in the range (0, 1)");
    }
    alpha_ = alpha;
}