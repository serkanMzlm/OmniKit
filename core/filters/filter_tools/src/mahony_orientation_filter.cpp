#include <cstdint>

#include "mahony_orientation_filter.hpp"

#define THREEHALFS (0x5f3759df)

float qw = 1.0f, qx = 0.0f, qy = 0.0f, qz = 0.0f;

const float twoKp = 2.0f * 0.5f;  // Proportional gain
const float twoKi = 2.0f * 0.0f;  // Integral gain

float integralFBx = 0.0f, integralFBy = 0.0f, integralFBz = 0.0f;

float fastInverseSqrt(float number)
{
    union
    {
        float f;
        uint32_t i;
    } conv = {.f = number};

    // Step 1: Initial Value and Bit Manipulation
    conv.i = THREEHALFS - (conv.i >> 1);

    // Step 2: Newton-Raphson Iteration
    conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
    return conv.f;
}

void mahonyOrientationFilter(IMUData_s imu, float dt)
{
    float recipNorm;
    float halfvx, halfvy, halfvz;
    float halfex, halfey, halfez;
    float qa, qb, qc;

    imu.gx = imu.gx * DEG_TO_RAD;
    imu.gy = imu.gy * DEG_TO_RAD;
    imu.gz = imu.gz * DEG_TO_RAD;

    if (!((imu.ax == 0.0f) && (imu.ay == 0.0f) && (imu.az == 0.0f))) 
    {
        recipNorm = fastInverseSqrt(imu.ax * imu.ax + imu.ay * imu.ay + imu.az * imu.az);
        imu.ax *= recipNorm;
        imu.ay *= recipNorm;
        imu.az *= recipNorm;

        halfvx = qx * qz - qw * qy;
        halfvy = qw * qx + qy * qz;
        halfvz = qw * qw - 0.5f + qz * qz;

        halfex = (imu.ay * halfvz - imu.az * halfvy);
        halfey = (imu.az * halfvx - imu.ax * halfvz);
        halfez = (imu.ax * halfvy - imu.ay * halfvx);

        if (twoKi > 0.0f) {
            integralFBx += twoKi * halfex * dt;
            integralFBy += twoKi * halfey * dt;
            integralFBz += twoKi * halfez * dt;
            imu.gx += integralFBx;
            imu.gy += integralFBy;
            imu.gz += integralFBz;
        }

        imu.gx += twoKp * halfex;
        imu.gy += twoKp * halfey;
        imu.gz += twoKp * halfez;
    }

    imu.gx *= (0.5f * dt);
    imu.gy *= (0.5f * dt);
    imu.gz *= (0.5f * dt);
    qa = qw;
    qb = qx;
    qc = qy;
    qw += (-qb * imu.gx - qc * imu.gy - qz * imu.gz);
    qx += (qa * imu.gx + qc * imu.gz - qz * imu.gy);
    qy += (qa * imu.gy - qb * imu.gz + qz * imu.gx);
    qz += (qa * imu.gz + qb * imu.gy - qc * imu.gx);

    recipNorm = fastInverseSqrt(qw * qw + qx * qx + qy * qy + qz * qz);
    qw *= recipNorm;
    qx *= recipNorm;
    qy *= recipNorm;
    qz *= recipNorm;
}