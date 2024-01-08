#include "mpu6050.hpp"

extern "C" {
#include <errno.h>
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
}

MPU6050::MPU6050(int bus_number){
    filename[9] = *std::to_string(bus_number).c_str();
	std::cout << filename << "__" << std::endl;
    if(initI2c(filename, MPU6050_ADDR) < 0){
        exit(1);
    }

	int result = i2c_smbus_write_byte_data(fd, PWR_MGMT_1, 0);
    if(result < 0){
        exit(1);
    }

    readGyroscopeRange();
    readAccelerometerRange();
    readDlpfConfig();
}

MPU6050::~MPU6050(){
    close(fd);
}

int MPU6050::initI2c(char* filename, int mpu_addr){
	fd = open(filename, O_RDWR);
	if(fd < 0){
        perror("Could not open the I2C device");
		return -1;
	}
	if(ioctl(fd, I2C_SLAVE, mpu_addr) < 0){
        perror("Could not set I2C device address");
        close(fd);
		return -1;
	}
	return 1;
}

void MPU6050::printConfig() const{
    std::cout << "Accelerometer Range: +-" << ranges[ACC_R] << "g\n";
    std::cout << "Gyroscope Range: +-" << ranges[GYR_R] << " degree per sec\n";
    std::cout << "DLPF Range: " << ranges[DLPF_R] << " Hz\n";
}

void MPU6050::printOffsets() const{
    std::cout << "Accelerometer Offsets: x: " << acc_offset[X] << ", y: " << acc_offset[Y]
            << ", z: " << acc_offset[Z] << "\n";
    std::cout << "Gyroscope Offsets: x: " << gyro_offset[X] << ", y: " << gyro_offset[Y]
            << ", z: " << gyro_offset[Z] << "\n";
}

int MPU6050::readGyroscopeRange(){
    int result = i2c_smbus_read_byte_data(fd, GYRO_CONFIG_REG);
    if(result < 0){
        exit(1);
    }
    result = result >> 3;
    ranges[GYR_R] = gyro_ranges[result];
    return ranges[GYR_R];
}

int MPU6050::readAccelerometerRange(){
    int result = i2c_smbus_read_byte_data(fd, ACC_CONFIG_REG);
    if(result < 0){
        exit(1);
    }
    result = result >> 3;
    ranges[ACC_R] = accel_ranges[result];
    return ranges[ACC_R];
}

int MPU6050::readDlpfConfig(){
    int result = i2c_smbus_read_byte_data(fd, DLPF_CONFIG_REG);
    if(result < 0){
        exit(1);
    }
    result = result >> 3;
    ranges[DLPF_R] = dlpf_ranges[result];
    return ranges[DLPF_R];
}

void MPU6050::setGyroscopeRange(GyroRange range){
    int result = i2c_smbus_write_byte_data(fd, GYRO_CONFIG_REG, range << 3);
    if (result < 0) reportError(errno);
    ranges[GYR_R] = gyro_ranges[static_cast<size_t>(range)];
}

void MPU6050::setAccelerometerRange(AccelRange range){
    int result = i2c_smbus_write_byte_data(fd, ACC_CONFIG_REG, range << 3);
    if (result < 0) reportError(errno);
    ranges[ACC_R] = accel_ranges[static_cast<size_t>(range)];
}

void MPU6050::setDlpfBandwidth(DlpfBandwidth bandwidth){
    int result = i2c_smbus_write_byte_data(fd, DLPF_CONFIG_REG, bandwidth);
    if (result < 0) reportError(errno);
    ranges[DLPF_R] = dlpf_ranges[static_cast<size_t>(bandwidth)];
}

double MPU6050::getAccelerationX() const
{
  int16_t acc_x_h = i2c_smbus_read_byte_data(fd, ACC_X_H);
  int16_t acc_x_l = i2c_smbus_read_byte_data(fd, ACC_X_H + 1);
  int16_t acc_x = acc_x_l | acc_x_h << 8;
  double accel_x_converted = convertRawAccData(acc_x);
  if (calibrated) {
    return accel_x_converted - acc_offset[X];
  }
  return accel_x_converted;
}

double MPU6050::getAccelerationY() const
{
  int16_t accel_y_msb = i2c_smbus_read_byte_data(fd, ACC_Y_H);
  int16_t accel_y_lsb = i2c_smbus_read_byte_data(fd, ACC_Y_H + 1);
  int16_t accel_y = accel_y_lsb | accel_y_msb << 8;
  double accel_y_converted = convertRawAccData(accel_y);
  if (calibrated) {
    return accel_y_converted - acc_offset[Y];
  }
  return accel_y_converted;
}

double MPU6050::getAccelerationZ() const
{
  int16_t accel_z_msb = i2c_smbus_read_byte_data(fd, ACC_Z_H);
  int16_t accel_z_lsb = i2c_smbus_read_byte_data(fd, ACC_Z_H + 1);
  int16_t accel_z = accel_z_lsb | accel_z_msb << 8;
  double accel_z_converted = convertRawAccData(accel_z);
  if (calibrated) {
    return accel_z_converted - acc_offset[Z];
  }
  return accel_z_converted;
}

double MPU6050::getAngularVelocityX() const
{
  int16_t gyro_x_msb = i2c_smbus_read_byte_data(fd, GYRO_X_H);
  int16_t gyro_x_lsb = i2c_smbus_read_byte_data(fd, GYRO_X_H + 1);
  int16_t gyro_x = gyro_x_lsb | gyro_x_msb << 8;
  double gyro_x_converted = convertRawGyroData(gyro_x);
  if (calibrated) {
    return gyro_x_converted - gyro_offset[X];
  }
  return gyro_x_converted;
}

double MPU6050::getAngularVelocityY() const
{
  int16_t gyro_y_msb = i2c_smbus_read_byte_data(fd, GYRO_Y_H);
  int16_t gyro_y_lsb = i2c_smbus_read_byte_data(fd, GYRO_Y_H + 1);
  int16_t gyro_y = gyro_y_lsb | gyro_y_msb << 8;
  double gyro_y_converted = convertRawGyroData(gyro_y);
  if (calibrated) {
    return gyro_y_converted - gyro_offset[Y];
  }
  return gyro_y_converted;
}

double MPU6050::getAngularVelocityZ() const
{
  int16_t gyro_z_msb = i2c_smbus_read_byte_data(fd, GYRO_Z_H);
  int16_t gyro_z_lsb = i2c_smbus_read_byte_data(fd, GYRO_Z_H + 1);
  int16_t gyro_z = gyro_z_lsb | gyro_z_msb << 8;
  double gyro_z_converted = convertRawGyroData(gyro_z);
  if (calibrated) {
    return gyro_z_converted - gyro_offset[Z];
  }
  return gyro_z_converted;
}

double MPU6050::convertRawAccData(int16_t gyro_raw) const{
    const double ang_vel = static_cast<double>(gyro_raw) / gyro_map.at(ranges[GYR_R]);
    return ang_vel; //angular velocity (deg/s)
}

double MPU6050::convertRawGyroData(int16_t accel_raw) const{
    const double accel = static_cast<double>(accel_raw) / accel_map.at(ranges[ACC_R]) * GRAVITY;
    return accel; // (m/s^2)
}

void MPU6050::setGyroOffset(double* offset){
  gyro_offset[X] = offset[X];
  gyro_offset[Y] = offset[Y];
  gyro_offset[Z] = offset[Z];
}

void MPU6050::setAccOffset(double* offset){
  acc_offset[X] = offset[X];
  acc_offset[Y] = offset[Y];
  acc_offset[Z] = offset[Z];
}

void MPU6050::reportError(int error) { 
	std::cerr << "Error! Errno: " << strerror(error); 
}

int main(){

	MPU6050 mpu6050(1);
	mpu6050.printOffsets();
	return 0;
}