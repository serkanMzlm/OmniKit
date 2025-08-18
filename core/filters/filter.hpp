#ifndef FILTER_HPP
#define FILTER_HPP

#include <cmath>
#include <stdexcept>
#include <limits>
#include <cassert>

struct BiquadData
{
    double b0, b1, b2, a1, a2;
    double delay_element_1 = 0.0;
    double delay_element_2 = 0.0;
};


class DigitalFilter
{
protected:
    double alpha_;
    BiquadData data_;
    bool initialized_ = false;

    static double validateAlpha(double alpha);
    static double calculateTimeConstant(double cutoff_freq);
    static double calculateSamplePeriod(double sample_freq);
    virtual void initBiquad() = 0;

public:
    virtual ~DigitalFilter() = default;
    virtual void reset();
};


/////////////////////////////////////////////////
//                    LOW PASS                 //
/////////////////////////////////////////////////
class LowPassFilter : public DigitalFilter
{
public:
    explicit LowPassFilter(double alpha);
    LowPassFilter(double cutoff_freq, double sample_freq);

    double applySimple(double current_input, double previous_output);
    double applyAdvanced(double sample);

private:
    void initBiquad() override;
};


/////////////////////////////////////////////////
//                    HIGH PASS                //
/////////////////////////////////////////////////
class HighPassFilter : public DigitalFilter
{
public:
    explicit HighPassFilter(double alpha);
    HighPassFilter(double cutoff_freq, double sample_freq);
    double applySimple(double current_input, double previous_input, double previous_output);
    double applyAdvanced(double sample);

private:
    void initBiquad() override;
};


/////////////////////////////////////////////////
//                 COMPLEMENTARY               //
/////////////////////////////////////////////////
class ComplementaryFilter
{
public:
    ComplementaryFilter(double alpha = 0.98);

    /// @brief Update the filter with a new measurement.
    /// @param measurement The new measurement value.
    /// @return The filtered value.
    double update(double primary, double secondary);
    double updatePrimaryOnly(double measurement);
    void setAlpha(double alpha);

private:
    double alpha_;          ///< Filter coefficient (0 < alpha < 1)
    double previous_value_; ///< Previous filtered value
};



/////////////////////////////////////////////////
//                    MAHONY                   //
/////////////////////////////////////////////////






/////////////////////////////////////////////////
//                 LINEAR KALMAN               //
/////////////////////////////////////////////////

#endif // FILTER_HPP