#include "omnikit/filter/highpass.hpp"

namespace omnikit::filter {

HighPassFilter2p::HighPassFilter2p(double sample_freq, double cutoff_freq)
    : lpf_(sample_freq, cutoff_freq) {}

void HighPassFilter2p::setCutoffFrequency(double sample_freq, double cutoff_freq) {
    lpf_.setCutoffFrequency(sample_freq, cutoff_freq);
}

double HighPassFilter2p::apply(double sample) {
    // High-pass = original signal minus its low-pass component.
    return sample - lpf_.apply(sample);
}

double HighPassFilter2p::reset(double sample) {
    lpf_.reset(sample);
    return sample;
}

} // namespace omnikit::filter