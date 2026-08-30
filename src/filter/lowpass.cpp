#include "omnikit/filter/lowpass.hpp"

#include <cmath>

namespace {
constexpr double PI = 3.14159265358979323846;
}

namespace omnikit::filter {

LowPassFilter2p::LowPassFilter2p(double sample_freq, double cutoff_freq) {
    setCutoffFrequency(sample_freq, cutoff_freq);
}

void LowPassFilter2p::setCutoffFrequency(double sample_freq, double cutoff_freq) {
    sample_freq_ = sample_freq;
    cutoff_freq_ = cutoff_freq;

    if (cutoff_freq_ <= 0.0 || sample_freq_ <= 0.0) {
        b0_ = 1.0;
        b1_ = 0.0;
        b2_ = 0.0;
        a1_ = 0.0;
        a2_ = 0.0;
        return;
    }

    const double fr = sample_freq_ / cutoff_freq_;
    const double ohm = std::tan(PI / fr);
    const double c = 1.0 + 2.0 * std::cos(PI / 4.0) * ohm + ohm * ohm;

    b0_ = ohm * ohm / c;
    b1_ = 2.0 * b0_;
    b2_ = b0_;
    a1_ = 2.0 * (ohm * ohm - 1.0) / c;
    a2_ = (1.0 - 2.0 * std::cos(PI / 4.0) * ohm + ohm * ohm) / c;
}

double LowPassFilter2p::apply(double sample) {
    const double output = b0_ * sample + b1_ * x1_ + b2_ * x2_ - a1_ * y1_ - a2_ * y2_;

    x2_ = x1_;
    x1_ = sample;
    y2_ = y1_;
    y1_ = output;

    return output;
}

void LowPassFilter2p::applyArray(double samples[], int num_samples) {
    for (int i = 0; i < num_samples; ++i) {
        samples[i] = apply(samples[i]);
    }
}

double LowPassFilter2p::reset(double sample) {
    x1_ = sample;
    x2_ = sample;
    y1_ = sample;
    y2_ = sample;
    return sample;
}

void LowPassFilter2p::disable() {
    b0_ = 1.0;
    b1_ = 0.0;
    b2_ = 0.0;
    a1_ = 0.0;
    a2_ = 0.0;
}

double LowPassFilter2p::getMagnitudeResponse(double frequency) const {
    // Evaluate the transfer function magnitude at the given frequency.
    // z = e^(j*w), w = 2*pi*frequency/sample_freq. |H(z)| is computed
    // from the real/imaginary parts of numerator and denominator.
    const double w = 2.0 * PI * frequency / sample_freq_;
    const double cos_w = std::cos(w);
    const double cos_2w = std::cos(2.0 * w);
    const double sin_w = std::sin(w);
    const double sin_2w = std::sin(2.0 * w);

    const double num_real = b0_ + b1_ * cos_w + b2_ * cos_2w;
    const double num_imag = -(b1_ * sin_w + b2_ * sin_2w);
    const double den_real = 1.0 + a1_ * cos_w + a2_ * cos_2w;
    const double den_imag = -(a1_ * sin_w + a2_ * sin_2w);

    const double num_mag = std::sqrt(num_real * num_real + num_imag * num_imag);
    const double den_mag = std::sqrt(den_real * den_real + den_imag * den_imag);

    return (den_mag != 0.0) ? (num_mag / den_mag) : 0.0;
}

} // namespace omnikit::filter
