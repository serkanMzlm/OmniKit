#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "transforms.hpp"

Eigen::Quaterniond eulerToQuaternion(double roll, double pitch, double yaw)
{
    Eigen::Quaterniond q;
    q = Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX())
      * Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY())
      * Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ());
    return q;
}

Eigen::Vector3d quaternionToEuler(const Eigen::Quaterniond& q)
{
    return q.toRotationMatrix().eulerAngles(0, 1, 2); // roll=x, pitch=y, yaw=z
}

Eigen::Vector3d cartesianToSpherical(const Eigen::Vector3d& cartesian)
{
    const double r = cartesian.norm();
    if (r == 0.0) {
        return Eigen::Vector3d(0.0, 0.0, 0.0);
    }

    const double theta = acosf(cartesian.z() / r);
    const double phi = atan2f(cartesian.y(), cartesian.x());
    return Eigen::Vector3d(r, theta, phi);
}

Eigen::Vector3d sphericalToCartesian(const Eigen::Vector3d& spherical)
{
    double r = spherical[0];
    double theta = spherical[1];
    double phi   = spherical[2];

    double x = r * std::sin(theta) * std::cos(phi);
    double y = r * std::sin(theta) * std::sin(phi);
    double z = r * std::cos(theta);

    return Eigen::Vector3d(x, y, z);
}

double wrapAngleTo2Pi(double angle) {
    angle = std::fmod(angle, 2 * M_PI); 
    if (angle < 0) {
        angle += 2 * M_PI; 
    }
    return angle;
}

double wrapAngleToPi(double angle) {
    angle = std::fmod(angle + M_PI, 2 * M_PI); 
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    return angle - M_PI; 
}

Eigen::Matrix3d rotX(double phi)
{
    Eigen::Matrix3d rx;
    rx << 1, 0, 0,
        0, std::cos(phi), -std::sin(phi),
        0, std::sin(phi), std::cos(phi);
    return rx;
}

Eigen::Matrix3d rotY(double beta)
{
    Eigen::Matrix3d ry;
    ry << std::cos(beta), 0, std::sin(beta),
        0, 1, 0,
        -std::sin(beta), 0, std::cos(beta);
    return ry;
}

Eigen::Matrix3d rotZ(double theta)
{
    Eigen::Matrix3d rz;
    rz << std::cos(theta), -std::sin(theta), 0,
        std::sin(theta), std::cos(theta), 0,
        0, 0, 1;
    return rz;
}

Eigen::Matrix3d eulerToRotationMatrix(double roll, double pitch, double yaw)
{
    return rotZ(yaw) * rotY(pitch) * rotX(roll);
}

Eigen::Matrix4d translationMatrix(const Eigen::Vector3d& t_xyz)
{
    Eigen::Matrix4d t = Eigen::Matrix4d::Identity();
    // t(0,3) = t_xyz.x();
    // t(1,3) = t_xyz.y();
    // t(2,3) = t_xyz.z();
    t.block<3,1>(0,3) = t_xyz;
    return t;
}

Eigen::Matrix4d scaleMatrix(const Eigen::Vector3d& s_xyz)
{
    Eigen::Matrix4d S = Eigen::Matrix4d::Identity();
    // S << s_xyz.x(), 0, 0, 0,
    //     0, s_xyz.y(), 0, 0,
    //     0, 0, s_xyz.z(), 0;
    //     0, 0, 0, 1;
    S(0,0) = s_xyz.x();
    S(1,1) = s_xyz.y();
    S(2,2) = s_xyz.z();
    return S;
}

Eigen::Matrix4d extendRotationMatrix(const Eigen::Matrix3d &rotationMatrix)
{
    Eigen::Matrix4d extended = Eigen::Matrix4d::Identity();
    extended.block<3, 3>(0, 0) = rotationMatrix;
    return extended;
}

Eigen::Matrix4d composeRt(const Eigen::Vector3d& rpy, const Eigen::Vector3d& t_xyz)
{
    Eigen::Matrix3d R = eulerToRotationMatrix(rpy.x(), rpy.y(), rpy.z());
    Eigen::Matrix4d T = translationMatrix(t_xyz);
    Eigen::Matrix4d R_ext = extendRotationMatrix(R);
    return T * R_ext; // transform = translation * rotation
}

Eigen::Vector3d applyTransform(const Eigen::Matrix4d& tr, const Eigen::Vector3d& p)
{
    Eigen::Vector4d ph;
    ph << p, 1.0; // homogeneous
    Eigen::Vector4d result = tr * ph;
    return result.head<3>();
}