#include "omnikit/geometry/transformations.hpp"

namespace omnikit::geometry {

Eigen::Matrix4d translationMatrix(const Eigen::Vector3d& translation) {
    Eigen::Matrix4d t = Eigen::Matrix4d::Identity();
    t.block<3, 1>(0, 3) = translation;
    return t;
}

Eigen::Matrix4d reflection(Axis axis) {
    Eigen::Matrix4d m = Eigen::Matrix4d::Identity();
    const int index = static_cast<int>(axis);
    m(index, index) = -1.0;
    return m;
}

Eigen::Matrix4d scaleMatrix(const Eigen::Vector3d& factors) {
    Eigen::Matrix4d s = Eigen::Matrix4d::Identity();
    s(0, 0) = factors.x();
    s(1, 1) = factors.y();
    s(2, 2) = factors.z();
    return s;
}

Eigen::Matrix4d scaleMatrix(double factor) {
    return scaleMatrix(Eigen::Vector3d(factor, factor, factor));
}

Eigen::Matrix4d rotationX(double angle) {
    const double c = std::cos(angle);
    const double s = std::sin(angle);

    Eigen::Matrix4d r = Eigen::Matrix4d::Identity();
    r(1, 1) = c;
    r(1, 2) = -s;
    r(2, 1) = s;
    r(2, 2) = c;
    return r;
}

Eigen::Matrix4d rotationY(double angle) {
    const double c = std::cos(angle);
    const double s = std::sin(angle);

    Eigen::Matrix4d r = Eigen::Matrix4d::Identity();
    r(0, 0) = c;
    r(0, 2) = s;
    r(2, 0) = -s;
    r(2, 2) = c;
    return r;
}

Eigen::Matrix4d rotationZ(double angle) {
    const double c = std::cos(angle);
    const double s = std::sin(angle);

    Eigen::Matrix4d r = Eigen::Matrix4d::Identity();
    r(0, 0) = c;
    r(0, 1) = -s;
    r(1, 0) = s;
    r(1, 1) = c;
    return r;
}

Eigen::Matrix4d shear(Axis affected, Axis by, double factor) {
    Eigen::Matrix4d s = Eigen::Matrix4d::Identity();

    // Shearing an axis by itself is meaningless; return identity.
    if (affected == by) {
        return s;
    }

    // The off-diagonal cell (row = affected, col = by) holds the factor,
    // so the affected coordinate gains factor * (the 'by' coordinate).
    const int row = static_cast<int>(affected);
    const int col = static_cast<int>(by);
    s(row, col) = factor;

    return s;
}

} // namespace omnikit::geometry