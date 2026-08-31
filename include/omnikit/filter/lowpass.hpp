#pragma once

namespace omnikit::filter {

struct SampleFreq {
    double value;
};
struct CutoffFreq {
    double value;
};

/// @brief A second-order (biquad) Butterworth low-pass filter.
///        Attenuates frequencies above the cutoff, passes lower ones.
class LowPassFilter2p {
public:
    LowPassFilter2p() = default;

    /// @brief Constructs and configures the filter.
    /// @param sample_freq  sampling frequency in Hz
    /// @param cutoff_freq  cutoff frequency in Hz
    LowPassFilter2p(SampleFreq sample_freq, CutoffFreq cutoff_freq);

    /// @brief Sets (or changes) the cutoff, recomputing coefficients.
    /// @param sample_freq  sampling frequency in Hz
    /// @param cutoff_freq  cutoff frequency in Hz (0 disables filtering)
    void setCutoffFrequency(SampleFreq sample_freq, CutoffFreq cutoff_freq);

    /// @brief Filters one sample and returns the filtered value.
    double apply(double sample);

    /// @brief Filters an array of samples in place.
    /// @param samples      the samples to filter (modified in place)
    /// @param num_samples  the number of samples
    void applyArray(double samples[], int num_samples);

    /// @brief Resets the filter state so its output settles at `sample`.
    /// @return the sample value
    double reset(double sample);

    /// @brief Disables filtering: apply() becomes a pass-through.
    void disable();

    [[nodiscard]] double getCutoffFreq() const { return cutoff_freq_.value; }
    [[nodiscard]] double getSampleFreq() const { return sample_freq_.value; }

    /// @brief Magnitude of the filter's response at a given frequency.
    /// @param frequency  the frequency in Hz
    /// @return the gain (0..1 typically) at that frequency
    double getMagnitudeResponse(double frequency) const;

private:
    SampleFreq sample_freq_ = {0.0};
    CutoffFreq cutoff_freq_ = {0.0};

    // Filter coefficients.
    double b0_ = 1.0;
    double b1_ = 0.0;
    double b2_ = 0.0;
    double a1_ = 0.0;
    double a2_ = 0.0;

    // State: previous two inputs and outputs.
    double x1_ = 0.0;
    double x2_ = 0.0;
    double y1_ = 0.0;
    double y2_ = 0.0;
};

} // namespace omnikit::filter