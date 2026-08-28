#include "omnikit/geometry/geometry.hpp"

#include <cmath>

namespace {
constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = 2.0 * PI;
} // namespace

namespace omnikit::geometry {

Eigen::Quaterniond eulerToQuaternion(double roll, double pitch, double yaw) {
    return Eigen::Quaterniond(Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
                              Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
                              Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX()));
}

Eigen::Vector3d quaternionToEuler(const Eigen::Quaterniond& q) {
    return q.toRotationMatrix().eulerAngles(2, 1, 0);
}

Eigen::Vector3d cartesianToSpherical(const Eigen::Vector3d& cartesian) {
    const double r = cartesian.norm();

    if (r == 0.0) {
        return Eigen::Vector3d(0.0, 0.0, 0.0);
    }

    const double theta = std::acos(cartesian.z() / r);
    const double phi = std::atan2(cartesian.y(), cartesian.x());

    return Eigen::Vector3d(r, theta, phi);
}

Eigen::Vector3d sphericalToCartesian(const Eigen::Vector3d& spherical) {
    const double r = spherical[0];
    const double theta = spherical[1];
    const double phi = spherical[2];

    const double x = r * std::sin(theta) * std::cos(phi);
    const double y = r * std::sin(theta) * std::sin(phi);
    const double z = r * std::cos(theta);

    return Eigen::Vector3d(x, y, z);
}

double wrapAngleTo2Pi(double angle) {
    angle = std::fmod(angle, TWO_PI);
    if (angle < 0.0) {
        angle += TWO_PI;
    }
    return angle;
}

double wrapAngleToPi(double angle) {
    angle = std::fmod(angle + PI, TWO_PI);
    if (angle < 0.0) {
        angle += TWO_PI;
    }
    return angle - PI;
}

} // namespace omnikit::geometry