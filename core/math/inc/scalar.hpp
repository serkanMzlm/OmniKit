#ifndef SCALAR_HPP
#define SCALAR_HPP

#include <cmath>
#include <cstdint>
#include <iostream>

/// @brief  Constrains a value within a specified range.
/// @tparam T The type of the value to be constrained (e.g., int, float, double).
/// @param data The value to be constrained.
/// @param min_value The minimum allowed value (inclusive).
/// @param max_value The maximum allowed value (inclusive).
/// @return The constrained value within the specified range.
/// @note   If `min_value` is greater than `max_value`, it returns a
template <typename T>
constexpr T constrain(T data, T min_value, T max_value) noexcept
{
    if (min_value > max_value)
    {
        std::cerr << "Error: min_value is greater than max_value." << std::endl;
        return T{}; 
    }
    return std::min(max_value, std::max(min_value, data));
}

/// @brief  Remaps a value from one range to another.
/// @tparam T The type of the value to be remapped (e.g., int, float, double).
/// @param value The value to be remapped.
/// @param in_min The minimum value of the input range.
/// @param in_max The maximum value of the input range.
/// @param out_min The minimum value of the output range.
/// @param out_max The maximum value of the output range.
/// @return The remapped value in the output range.
/// @note   If `in_min` equals `in_max`, it returns `out_min
template <typename T>
constexpr T remap(T value, T in_min, T in_max, T out_min, T out_max) noexcept
{
    if (in_min == in_max)
    {
        std::cerr << "Error: in_min and in_max cannot be equal." << std::endl;
        return out_min; // Avoid division by zero
    }
    value = constrain(value, in_min, in_max); // Ensure value is within the input range
    return out_min + (value - in_min) * (out_max - out_min) / (in_max - in_min);
}

/// @brief  Computes the fast inverse square root of a number.
/// @param number The input number for which the inverse square root is to be calculated.
/// @return The approximate inverse square root of the input number.
/// @note   This function uses a fast approximation method, which may not be as accurate as
float fastInverseSqrt(float number);

#endif // SCALAR_HPP