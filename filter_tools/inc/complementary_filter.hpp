#ifndef __COMPLEMENTRAY_FILTER__
#define __COMPLEMENTRAY_FILTER__

#include <cmath>

#include "filters_type.hpp"
#include "low_high_pass_filter.hpp"

void complementaryFilter(IMUData_s imu, float dt);
void accelToEuler(IMUData_s imu);
void gyroToEuler(IMUData_s imu, float dt);

#endif