#ifndef __GEOMETRY_OPERATIONS_HPP__
#define __GEOMETRY_OPERATIONS_HPP__

#include <Eigen/Dense>
#include "geometry_type.hpp"

/**
 * @brief Converts Euler angles to a quaternion.
 *
 * @param euler A double array of size 3 containing the Euler angles (roll, pitch, yaw) in radians.
 * @param quaternion A double array of size 4 to store the resulting quaternion (w, x, y, z).
 */
void eulerToQuaternion(double* euler, double *quaternion);

/**
 * @brief Converts a quaternion to Euler angles.
 *
 * @param quaternion A double array of size 4 containing the quaternion (w, x, y, z).
 * @param euler A double array of size 3 to store the resulting Euler angles (roll, pitch, yaw) in radians.
 */
void quaternionToEuler(double *quaternion, double* euler);

/**
 * @brief Converts Cartesian coordinates to spherical coordinates.
 *
 * @param[in] cart_data A pointer to an array of three floats representing the
 *                      cartesian coordinates (x, y, z).
 * @param[out] spe_data A pointer to an array of three floats where the spherical
 *                      coordinates (radius, theta, phi) will be stored.
 */
void cartesianToSpherical(float *cart_data, float *spe_data);

/**
 * @brief Converts spherical coordinates to Cartesian coordinates.
 *
 * @param[in] spe_data A pointer to an array of three floats representing the
 *                      spherical coordinates (radius, theta, phi).
 * @param[out] cart_data A pointer to an array of three floats where the Cartesian
 *                      coordinates (x, y, z) will be stored.
 */
void sphericalToCartesian(float *spe_data, float *cart_data);

/**
 * @brief Constrains the given angle to a specified range.
 *
 * @param angle The angle to be normalized.
 * @param min_angle The minimum angle of the range. Valid values are 0 and -180.
 * @return The normalized angle constrained to the specified range.
 *
 * @note If min_angle is neither 0 nor -180, it will be set to 0.
 */
float constrainAngle(float angle = 0, float min_angle = 0);

/**
 * @brief Wraps an angle to the range [-180, 180] degrees.
 *
 * @param angle The angle in degrees.
 * @return The wrapped angle.
 */
float wrapAngleTo180(float angle);

/**
 * @brief Wraps an angle to the range [0, 360] degrees.
 *
 * @param angle The angle in degrees.
 * @return The wrapped angle.
 */
float wrapAngleTo360(float angle);

#endif