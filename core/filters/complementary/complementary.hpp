#ifndef COMPLEMENTARY_HPP
#define COMPLEMENTARY_HPP

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

#endif // COMPLEMENTARY_HPP