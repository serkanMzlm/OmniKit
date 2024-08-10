#ifndef __GEOMETRY_OPERATIONS_TYPE_HPP__
#define __GEOMETRY_OPERATIONS_TYPE_HPP__

#define F2P(x) (1000 / x)

#ifndef DEG2RAD
#define DEG2RAD(x) ((x)*(M_PI / 180.0f))
#endif
#ifndef RAD2DEG
#define RAD2DEG(x) ((x)*(180.0f / M_PI))
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (M_PI / 180.0f)
#endif
#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)
#endif

typedef enum
{
    X,
    Y,
    Z,
    W,
    QUATERNION_ALL
} Quaternion_e;

typedef enum
{
    RADIUS,
    THETA,
    PHI,
    CYL_COORD_ALL
} CylindricalCoordinate_e;

typedef enum
{
    ROLL,
    PITCH,
    YAW,
    EULER_ALL
} Euler_e;

#endif