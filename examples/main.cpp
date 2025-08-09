#include <thread>
#include <iostream>

#include "omnikit/utils/timing.hpp"
#include "omnikit/math/scalar.hpp"
#include "omnikit/geometry/transforms.hpp"

int main()
{
    double roll = M_PI / 6;   // 30°
    double pitch = M_PI / 4;  // 45°
    double yaw = M_PI / 3;    // 60°
    Eigen::Quaterniond q = eulerToQuaternion(roll, pitch, yaw);

    std::cout << "Quaternion (w, x, y, z): "
              << q.w() << ", " << q.x() << ", "
              << q.y() << ", " << q.z() << "\n";
    Eigen::Vector3d euler = quaternionToEuler(q);
    std::cout << "Recovered Euler angles (rad): roll=" << euler.x()
              << " pitch=" << euler.y()
              << " yaw="   << euler.z() << "\n";

    // 3. Bir noktaya dönüşüm uygulama
    Eigen::Vector3d rpy(M_PI/6, 0.0, M_PI/4);      // roll=30°, yaw=45°
    Eigen::Vector3d t_xyz(1.0, 2.0, 3.0);          // translation
    Eigen::Matrix4d T = composeRt(rpy, t_xyz);

    Eigen::Vector3d point(1.0, 0.0, 0.0);          // başlangıç noktası
    Eigen::Vector3d transformed_point = applyTransform(T, point);

    std::cout << "Original point: " << point.transpose() << "\n";
    std::cout << "Transformed point: " << transformed_point.transpose() << "\n";

    // 4. Cartesian ↔ Spherical dönüşüm
    Eigen::Vector3d cartesian(3.0, 4.0, 5.0);
    Eigen::Vector3d spherical = cartesianToSpherical(cartesian);
    Eigen::Vector3d cartesian_back = sphericalToCartesian(spherical);

    std::cout << "Cartesian: " << cartesian.transpose() << "\n";
    std::cout << "Spherical (r, theta, phi): " << spherical.transpose() << "\n";
    std::cout << "Back to Cartesian: " << cartesian_back.transpose() << "\n";

    // 5. Angle wrapping
    double angle1 = 7.5;  // yaklaşık 430°
    double angle2 = -4.0; // -229°

    std::cout << "wrapAngleTo2Pi(7.5)  = " << wrapAngleTo2Pi(angle1) << " rad\n";
    std::cout << "wrapAngleToPi(-4.0) = " << wrapAngleToPi(angle2) << " rad\n";
    return 0;
}