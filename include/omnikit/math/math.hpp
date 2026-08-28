#pragma once

#include <algorithm>


namespace omnikit::math {

/// @brief Fast approximation of the inverse square root (1/sqrt(x)).
/// @param number  the input value
/// @return an approximation of 1/sqrt(number)
/// @note  A classic bit-trick approximation; prefer 1.0f/std::sqrt(x)
///        for accuracy on modern hardware.
float fastInverseSqrt(float number);

/// @brief Constrains a value to the range [min_value, max_value].
/// @tparam T  a comparable numeric type
/// @param data       the value to constrain
/// @param min_value  the lower bound (inclusive)
/// @param max_value  the upper bound (inclusive)
/// @return the value clamped into the range; if the range is reversed
///         (min > max), returns min_value
template <typename T> constexpr T constrain(T data, T min_value, T max_value) noexcept {
    if (min_value > max_value) {
        return min_value;
    }
    return std::min(max_value, std::max(min_value, data));
}

/// @brief Remaps a value from one range to another (linear scaling).
/// @tparam T  a numeric type
/// @param value    the value to remap
/// @param in_min   input range minimum
/// @param in_max   input range maximum
/// @param out_min  output range minimum
/// @param out_max  output range maximum
/// @return the remapped value; if in_min == in_max, returns out_min
template <typename T>
constexpr T remap(T value, T in_min, T in_max, T out_min, T out_max) noexcept {
    if (in_min == in_max) {
        return out_min;
    }
    value = constrain(value, in_min, in_max);
    return out_min + (value - in_min) * (out_max - out_min) / (in_max - in_min);
}
} // namespace omnikit::math