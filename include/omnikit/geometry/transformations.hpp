#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace omnikit::geometry {

/// @brief Identifies a coordinate axis, used by axis-parameterized
enum class Axis { X, Y, Z };

/// @brief Builds a 4x4 homogeneous translation matrix.
/// @param translation  the offset (tx, ty, tz)
/// @return a 4x4 matrix that shifts a point by the given offset
Eigen::Matrix4d translationMatrix(const Eigen::Vector3d& translation);

/// @brief Reflection that flips the given axis (mirrors across the plane
///        spanned by the other two). E.g. reflection(Axis::Z) flips Z,
///        which mirrors across the XY plane.
/// @param axis  the axis whose sign is negated
/// @return the 4x4 reflection matrix
Eigen::Matrix4d reflection(Axis axis);

/// @brief Builds a 4x4 scaling matrix with per-axis factors.
/// @param factors  the scale factors (sx, sy, sz)
/// @return a 4x4 matrix scaling each axis independently
Eigen::Matrix4d scaleMatrix(const Eigen::Vector3d& factors);

/// @brief Builds a 4x4 uniform scaling matrix.
/// @param factor  the single scale factor applied to all axes
/// @return a 4x4 matrix scaling every axis by the same amount
Eigen::Matrix4d scaleMatrix(double factor);

/// @brief Rotation matrix about the X axis (roll), as a 4x4 homogeneous matrix.
/// @param angle  rotation angle in radians
/// @return the 4x4 rotation matrix
Eigen::Matrix4d rotationX(double angle);

/// @brief Rotation matrix about the Y axis (pitch), as a 4x4 homogeneous matrix.
/// @param angle  rotation angle in radians
/// @return the 4x4 rotation matrix
Eigen::Matrix4d rotationY(double angle);

/// @brief Rotation matrix about the Z axis (yaw), as a 4x4 homogeneous matrix.
/// @param angle  rotation angle in radians
/// @return the 4x4 rotation matrix
Eigen::Matrix4d rotationZ(double angle);

/// @brief Builds a 4x4 shear matrix: shifts the `affected` axis in
///        proportion to the `by` axis, by the given factor.
///        Example: shear(Axis::X, Axis::Y, k) maps (x,y,z) to (x+k*y, y, z).
/// @param affected  the axis whose value is shifted
/// @param by        the axis that drives the shift
/// @param factor    the shear factor
/// @return the 4x4 shear matrix (identity if affected == by)
Eigen::Matrix4d shear(Axis affected, Axis by, double factor);
} // namespace omnikit::geometry