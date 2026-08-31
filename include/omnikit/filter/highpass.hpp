#pragma once

#include "omnikit/filter/lowpass.hpp"

namespace omnikit::filter {

/// @brief A second-order high-pass filter, built as input minus its
///        low-pass component. Passes fast changes, removes slow drift.
class HighPassFilter2p {
public:
    HighPassFilter2p() = default;

    /// @param sample_freq  sampling frequency in Hz
    /// @param cutoff_freq  cutoff frequency in Hz
    HighPassFilter2p(double sample_freq, double cutoff_freq);

    /// @brief Sets the cutoff, recomputing the internal low-pass.
    void setCutoffFrequency(double sample_freq, double cutoff_freq);

    /// @brief Filters one sample: sample minus its low-pass component.
    double apply(double sample);

    /// @brief Resets the filter state to settle at `sample`.
    double reset(double sample);

    double getCutoffFreq() const { return lpf_.getCutoffFreq(); }
    [[nodiscard]] double getSampleFreq() const { return lpf_.getSampleFreq(); }

private:
    LowPassFilter2p lpf_;
};

} // namespace omnikit::filter