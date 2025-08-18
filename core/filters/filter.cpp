#include <stdexcept>
#include "filter.hpp"

double DigitalFilter::validateAlpha(double alpha)
{
    if (alpha <= 0.0 || alpha > 1.0)
    {
        throw std::invalid_argument("Alpha must be between 0 and 1");
    }
    return alpha;
}

double DigitalFilter::calculateTimeConstant(double cutoff_freq)
{
    if (cutoff_freq <= 0.0)
    {
        throw std::invalid_argument("Cutoff frequency must be positive");
    }
    return 1.0 / (2 * M_PI * cutoff_freq);
}

double DigitalFilter::calculateSamplePeriod(double sample_freq)
{
    if (sample_freq <= 0.0)
    {
        throw std::invalid_argument("Sample frequency must be positive");
    }
    return 1.0 / sample_freq;
}

void DigitalFilter::reset()
{
    data_.delay_element_1 = 0.0;
    data_.delay_element_2 = 0.0;
    initialized_ = false;
}


/////////////////////////////////////////////////
//                    LOW PASS                 //
/////////////////////////////////////////////////
LowPassFilter::LowPassFilter(double alpha)
{
    alpha_ = validateAlpha(alpha);
}

LowPassFilter::LowPassFilter(double cutoff_freq, double sample_freq)
{
    alpha_ = calculateSamplePeriod(sample_freq) / (calculateSamplePeriod(sample_freq) + calculateTimeConstant(cutoff_freq));
}

void LowPassFilter::initBiquad()
{
    const double fr = 1.0 / alpha_;
    const double ohm = tan(M_PI / fr);
    const double c = 1.0 + 2.0 * cos(M_PI / 4.0) * ohm + ohm * ohm;

    data_.b0 = ohm * ohm / c;
    data_.b1 = 2.0 * data_.b0;
    data_.b2 = data_.b0;
    data_.a1 = 2.0 * (ohm * ohm - 1.0) / c;
    data_.a2 = (1.0 - 2.0 * cos(M_PI / 4.0) * ohm + ohm * ohm) / c;
}

double LowPassFilter::applySimple(double current_input, double previous_output)
{
    assert(std::isfinite(current_input) && "Invalid current_input");
    assert(std::isfinite(previous_output) && "Invalid previous_output");
    return alpha_ * current_input + (1.0 - alpha_) * previous_output;
}

double LowPassFilter::applyAdvanced(double sample)
{
    if (!initialized_)
    {
        initBiquad();
        initialized_ = true;
    }

    double delay_element_0 = sample - data_.delay_element_1 * data_.a1 - data_.delay_element_2 * data_.a2;

    if (!std::isfinite(delay_element_0))
    {
        delay_element_0 = sample;
    }

    double output = delay_element_0 * data_.b0 + data_.delay_element_1 * data_.b1 + data_.delay_element_2 * data_.b2;

    data_.delay_element_2 = data_.delay_element_1;
    data_.delay_element_1 = delay_element_0;

    return output;
}


/////////////////////////////////////////////////
//                    HIGH PASS                //
/////////////////////////////////////////////////
HighPassFilter::HighPassFilter(double alpha)
{
    alpha_ = validateAlpha(alpha);
}

HighPassFilter::HighPassFilter(double cutoff_freq, double sample_freq)
{
    alpha_ = calculateTimeConstant(cutoff_freq) / (calculateSamplePeriod(sample_freq) + calculateTimeConstant(cutoff_freq));
}

void HighPassFilter::initBiquad()
{
    const double fr = 1.0 / alpha_;
    const double ohm = tan(M_PI / fr);
    const double c = 1.0 + 2.0 * cos(M_PI / 4.0) * ohm + ohm * ohm;

    data_.b0 = 1.0 / c;
    data_.b1 = -2.0 / c;
    data_.b2 = data_.b0;
    data_.a1 = 2.0 * (ohm * ohm - 1.0) / c;
    data_.a2 = (1.0 - 2.0 * cos(M_PI / 4.0) * ohm + ohm * ohm) / c;
}

double HighPassFilter::applySimple(double current_input, double previous_input, double previous_output)
{
    assert(std::isfinite(current_input) && "Invalid current_input");
    assert(std::isfinite(previous_input) && "Invalid previous_input");
    assert(std::isfinite(previous_output) && "Invalid previous_output");
    return alpha_ * (previous_output + current_input - previous_input);
}

double HighPassFilter::applyAdvanced(double sample)
{
    if (!initialized_)
    {
        initBiquad();
        initialized_ = true;
    }

    double delay_element_0 = sample - data_.delay_element_1 * data_.a1 - data_.delay_element_2 * data_.a2;

    if (!std::isfinite(delay_element_0))
    {
        delay_element_0 = sample;
    }

    double output = delay_element_0 * data_.b0 + data_.delay_element_1 * data_.b1 + data_.delay_element_2 * data_.b2;

    data_.delay_element_2 = data_.delay_element_1;
    data_.delay_element_1 = delay_element_0;

    return output;
}


/////////////////////////////////////////////////
//                 COMPLEMENTARY               //
/////////////////////////////////////////////////
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