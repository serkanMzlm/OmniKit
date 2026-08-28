#include "omnikit/geometry/transformations.hpp"
#include "test_framework.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cmath>

using omnikit::geometry::Axis;
using omnikit::geometry::reflection;
using omnikit::geometry::rotationX;
using omnikit::geometry::rotationY;
using omnikit::geometry::rotationZ;
using omnikit::geometry::scaleMatrix;
using omnikit::geometry::shear;
using omnikit::geometry::translationMatrix;

namespace {
/// Applies a 4x4 homogeneous transform to a 3D point.
Eigen::Vector3d apply(const Eigen::Matrix4d& m, const Eigen::Vector3d& p) {
    Eigen::Vector4d ph;
    ph << p, 1.0;
    return (m * ph).head<3>();
}

/// Approximate equality for two 3D points (floating-point safe).
bool close(const Eigen::Vector3d& a, const Eigen::Vector3d& b) {
    return (a - b).norm() < 1e-9;
}
} // namespace

void test_translation() {
    const Eigen::Matrix4d t = translationMatrix(Eigen::Vector3d(5, -2, 3));
    const Eigen::Vector3d p(1, 1, 1);

    CHECK(close(apply(t, p), Eigen::Vector3d(6, -1, 4)));
}

void test_reflection() {
    const Eigen::Vector3d p(2, 3, 4);

    CHECK(close(apply(reflection(Axis::Z), p), Eigen::Vector3d(2, 3, -4)));
    CHECK(close(apply(reflection(Axis::X), p), Eigen::Vector3d(-2, 3, 4)));
    CHECK(close(apply(reflection(Axis::Y), p), Eigen::Vector3d(2, -3, 4)));
}

void test_scale() {
    const Eigen::Vector3d p(2, 3, 4);

    CHECK(close(apply(scaleMatrix(Eigen::Vector3d(2, 3, 0.5)), p), Eigen::Vector3d(4, 9, 2)));
    CHECK(close(apply(scaleMatrix(2.0), p), Eigen::Vector3d(4, 6, 8)));
}

void test_rotation() {
    const double half_pi = M_PI / 2.0;

    // rotationZ(90): X axis -> Y axis.
    CHECK(close(apply(rotationZ(half_pi), Eigen::Vector3d(1, 0, 0)), Eigen::Vector3d(0, 1, 0)));
    // rotationX(90): Y axis -> Z axis.
    CHECK(close(apply(rotationX(half_pi), Eigen::Vector3d(0, 1, 0)), Eigen::Vector3d(0, 0, 1)));
    // rotationY(90): Z axis -> X axis.
    CHECK(close(apply(rotationY(half_pi), Eigen::Vector3d(0, 0, 1)), Eigen::Vector3d(1, 0, 0)));
}

void test_shear() {
    const Eigen::Vector3d p(3, 5, 0);

    CHECK(close(apply(shear(Axis::X, Axis::Y, 2.0), p), Eigen::Vector3d(13, 5, 0)));

    CHECK(close(apply(shear(Axis::X, Axis::X, 2.0), p), p));
}

void test_composition() {
    const Eigen::Matrix4d m = translationMatrix(Eigen::Vector3d(1, 0, 0)) * scaleMatrix(2.0);

    CHECK(close(apply(m, Eigen::Vector3d(3, 0, 0)), Eigen::Vector3d(7, 0, 0)));
}

int main() {
    test_translation();
    test_reflection();
    test_scale();
    test_rotation();
    test_shear();
    test_composition();

    using namespace omnikit::test;
    std::printf("\n%d checks, %d failed\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}