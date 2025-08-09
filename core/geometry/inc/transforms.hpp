#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "transforms_type.hpp"
#include <Eigen/Dense>

/// @brief Converts Euler angles (roll, pitch, yaw) to a quaternion.
/// @return Eigen::Quaterniond The resulting quaternion
Eigen::Quaterniond eulerToQuaternion(double roll, double pitch, double yaw);

/// @brief Converts a quaternion to Euler angles (roll, pitch, yaw).
/// @param q Input unit quaternion.
/// @return Eigen::Vector3d Euler angles in radians as (roll=X, pitch=Y, yaw=Z).
/// @note Order is ZYX when extracting (roll=X, pitch=Y, yaw=Z).
Eigen::Vector3d quaternionToEuler(const Eigen::Quaterniond& q);

/// @brief Converts Cartesian coordinates to spherical coordinates.
/// @param cartesian Input Cartesian coordinates as Eigen::Vector3d.
/// @return Eigen::Vector3d Spherical coordinates as (radius, theta, phi).
Eigen::Vector3d cartesianToSpherical(const Eigen::Vector3d& cartesian);

/// @brief Converts spherical coordinates to Cartesian coordinates.
/// @param spherical Input spherical coordinates as Eigen::Vector3d (radius, theta, phi
/// @return Eigen::Vector3d Cartesian coordinates as (x, y, z).
Eigen::Vector3d sphericalToCartesian(const Eigen::Vector3d& spherical);


/// @brief Wraps an angle to the range [0, 2pi] Rad.
double wrapAngleTo2Pi(double angle);

/// @brief Wraps an angle to the range [-pi, pi] Rad.
double wrapAngleToPi(double angle);


/**
 * @brief Generates a rotation matrix for a rotation around the X-axis.
 *
 * @param phi The rotation angle in radians.
 * @return A 3x3 rotation matrix for a rotation around the X-axis.
 */
Eigen::Matrix3d rotX(double phi);

/**
 * @brief Generates a rotation matrix for a rotation around the Y-axis.
 *
 * @param beta The rotation angle in radians.
 * @return A 3x3 rotation matrix for a rotation around the Y-axis.
 */
Eigen::Matrix3d rotY(double beta);

/**
 * @brief Generates a rotation matrix for a rotation around the Z-axis.
 *
 * @param theta The rotation angle in radians.
 * @return A 3x3 rotation matrix for a rotation around the Z-axis.
 */
Eigen::Matrix3d rotZ(double theta);

/**
 * @brief Calculates the combined rotation matrix from individual Euler angles.
 *
 * @param roll The rotation angle around the X-axis (in radians).
 * @param pitch The rotation angle around the Y-axis (in radians).
 * @param yaw The rotation angle around the Z-axis (in radians).
 * @return A 3x3 rotation matrix representing the combined rotation.
 */
Eigen::Matrix3d eulerToRotationMatrix(double roll, double pitch, double yaw);

/**
 * @brief Build a 4x4 homogeneous translation matrix.
 * @param t_xyz Translation vector (x, y, z).
 * @return 4x4 matrix with t at the last column.
 */
Eigen::Matrix4d translationMatrix(const Eigen::Vector3d& t_xyz);

/**
 * @brief Build a 4x4 homogeneous scaling matrix.
 * @param s_xyz Scale factors along x, y, z.
 * @return 4x4 diagonal scale matrix.
 */
Eigen::Matrix4d scaleMatrix(const Eigen::Vector3d& s_xyz);

/**
 * @brief Embed a 3x3 rotation into a 4x4 homogeneous matrix.
 * @param R 3x3 rotation matrix.
 * @return 4x4 matrix with R in the top-left block.
 */
Eigen::Matrix4d extendRotationMatrix(const Eigen::Matrix3d &rotation_matrix);

/**
 * @brief Compose 4x4 transform from RPY and translation.
 * @param rpy  (roll, pitch, yaw) in radians.
 * @param t_xyz Translation (x, y, z).
 * @return 4x4 transform T * R (rotate, then translate point).
 */
Eigen::Matrix4d composeRt(const Eigen::Vector3d& rpy, const Eigen::Vector3d& t_xyz);

/**
 * @brief Apply a 4x4 transform to a 3D point.
 * @param TR 4x4 transform (homogeneous).
 * @param p  3D point (x, y, z).
 * @return Transformed 3D point.
 * @note Treats p as a point (w = 1). For direction vectors, use w = 0.
 */
Eigen::Vector3d applyTransform(const Eigen::Matrix4d& TR, const Eigen::Vector3d& p);

#endif // TRANSFORMS_HPP