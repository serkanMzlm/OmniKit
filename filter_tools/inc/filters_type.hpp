#ifndef __FILTERS_TYPE_HPP__
#define __FILTERS_TYPE_HPP__

#include <cmath>

#define IIR_SHIFT 8
#define GRAVITY 9.8f

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (M_PI / 180.0f)
#endif
#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)
#endif

typedef enum
{
    AXIS_X,
    AXIS_Y,
    AXIS_Z,
    ALL_AXIS
} Axis_e;

typedef struct
{
    float ax, ay, az; // Acc data
    float gx, gy, gz; // Gyro data
} IMUData_s;

typedef struct {
    float a1;
    float a2;
    float b0;
    float b1;
    float b2;
    float delay_element_1;
    float delay_element_2;
} lpf2pData;

#endif