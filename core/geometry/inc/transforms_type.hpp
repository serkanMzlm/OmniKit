#ifndef TRANSFORMS_TYPE_HPP
#define TRANSFORMS_TYPE_HPP

#define F2P(x) (1000 / x)

#ifndef DEG2RAD
#define DEG2RAD(x) ((x) * (M_PI / 180.0f))
#endif
#ifndef RAD2DEG
#define RAD2DEG(x) ((x) * (180.0f / M_PI))
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (M_PI / 180.0f)
#endif
#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)
#endif

/// @brief Represents quaternion components.
enum class QuaternionComponent
{
    X,  ///< X component
    Y,  ///< Y component
    Z,  ///< Z component
    W,  ///< W component
    All ///< All components
};

/// @brief Represents Euler angle components.
enum class EulerAngle
{
    Roll,  ///< Roll angle
    Pitch, ///< Pitch angle
    Yaw,   ///< Yaw angle
    All    ///< All components
};

/// @brief Represents Cartesian (3D) coordinate components.
enum class CartesianCoordinate
{
    X,  ///< X axis
    Y,  ///< Y axis
    Z,  ///< Z axis
    All ///< All components
};

/// @brief Represents spherical coordinate components.
enum class SphericalCoordinate
{
    Radius, ///< r - Radius
    Theta,  ///< θ - Polar angle (0 to π)
    Phi,    ///< φ - Azimuthal angle (0 to 2π)
    All     ///< All components
};

/// @brief Represents cylindrical coordinate components.
enum class CylindricalCoordinate
{
    Radius, ///< r - Radius
    Theta,  ///< θ - Angular coordinate (0 to 2π)
    Z,      ///< z - Height axis
    All     ///< All components
};

#endif // TRANSFORMS_TYPE_HPP