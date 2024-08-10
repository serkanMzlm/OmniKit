#ifndef __MATH_OPERATIONS_HPP__
#define __MATH_OPERATIONS_HPP__

/**
 * @brief Maps a value from one range to another.
 *
 * @param data The input value to be mapped.
 * @param in_min The minimum value of the input range.
 * @param in_max The maximum value of the input range.
 * @param out_min The minimum value of the output range.
 * @param out_max The maximum value of the output range.
 * @return The mapped value in the output range.
 */
double map(double data, double in_min, double in_max, double out_min, double out_max);

/**
 * @brief Constrain a value within a specified range.
 *
 * @param data The value to be constrained.
 * @param min_value The minimum allowed value (inclusive).
 * @param max_value The maximum allowed value (inclusive).
 * @return The constrained value within the specified range.
 */
double constrain(double data, double min_value, double max_value);

/**
 * @brief Calculate the fast inverse square root of a number.
 *
 * @param number The input number for which the inverse square root is to be calculated.
 * @return The approximate inverse square root of the input number.
 */
float fastInverseSqrt(float number);

#endif