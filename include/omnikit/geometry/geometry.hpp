#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>

constexpr double f2p(double x) {
    return 1000.0 / x;
}

constexpr double deg2rad(double x) {
    return x * (M_PI / 180.0);
}

constexpr double rad2deg(double x) {
    return x * (180.0 / M_PI);
}

namespace omnikit::geometry {

/// @brief Converts Euler angles (roll, pitch, yaw) to a quaternion.
///        Uses the ZYX (Tait-Bryan) convention (yaw, then pitch, then roll).
/// @param roll   rotation about the X axis, in radians
/// @param pitch  rotation about the Y axis, in radians
/// @param yaw    rotation about the Z axis, in radians
/// @return the resulting orientation as a quaternion
Eigen::Quaterniond eulerToQuaternion(double roll, double pitch, double yaw);

/// @brief Converts a quaternion to Euler angles.
///        ZYX convention, matching eulerToQuaternion.
/// @param q  the orientation quaternion
/// @return a vector [yaw, pitch, roll] in radians
Eigen::Vector3d quaternionToEuler(const Eigen::Quaterniond& q);

/// @brief Converts Cartesian coordinates (x, y, z) to spherical (r, theta, phi).
/// @param cartesian  the point in Cartesian coordinates
/// @return a vector [r, theta, phi]: radius, polar angle, azimuth (radians)
Eigen::Vector3d cartesianToSpherical(const Eigen::Vector3d& cartesian);

/// @brief Converts spherical coordinates (r, theta, phi) to Cartesian (x, y, z).
///        Inverse of cartesianToSpherical.
/// @param spherical  the point as (r, theta, phi) in radians
/// @return the point as (x, y, z)
Eigen::Vector3d sphericalToCartesian(const Eigen::Vector3d& spherical);

/// @brief Wraps an angle to the range [0, 2*pi).
/// @param angle  the angle in radians
/// @return the equivalent angle in [0, 2*pi)
double wrapAngleTo2Pi(double angle);

/// @brief Wraps an angle to the range [-pi, pi).
/// @param angle  the angle in radians
/// @return the equivalent angle in [-pi, pi)
double wrapAngleToPi(double angle);

} // namespace omnikit::geometry