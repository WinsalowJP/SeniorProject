#ifndef __IMU_H
#define __IMU_H

#include "stm32wbxx_hal.h"
#include <stdint.h>
extern I2C_HandleTypeDef hi2c1;
// BNO055 I2C address (7-bit 0x28) shifted left for HAL
#define BNO055_ADDR           (0x28 << 1)

// BNO055 Register addresses
#define BNO055_OPR_MODE       0x3D
#define BNO055_PWR_MODE       0x3E
#define BNO055_SYS_STAT       0x39
#define BNO055_CALIB_STAT     0x35
#define BNO055_MAG_REG        0x0E
#define BNO055_ACCEL_REG      0x08
#define BNO055_GYRO_REG       0x14
#define BNO055_EULER_H        0x1A

// Operation modes
#define OPERATION_MODE_CONFIG 0x00
#define OPERATION_MODE_NDOF   0x0C

// Initialize the BNO055 (CONFIG mode)
// Returns 0 on success, -1 on error
int BNO055_Init(void);

// Sequence of mode writes: PWR, CONFIG, and target mode
// Returns 0 on success, -1 on error
int BNO055_SetModeSequence(uint8_t mode);

// Read calibration status (sys, gyro, accel, mag: 0-3)
// Returns 0 on success, -1 on error
int BNO055_GetCalibrationStatus(uint8_t *sys, uint8_t *gyro, uint8_t *accel, uint8_t *mag);

// Block until full calibration achieved (all == 3)
// Returns 0 on success, -1 on error
int BNO055_CalibrateIMU(void);

// Read sensor vectors: magnetometer, accelerometer, gyroscope, Euler angles
// Each returns 0 on success (fills x,y,z), -1 on error
int BNO055_GetMag(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z);
int BNO055_GetAccel(int16_t *accel_x, int16_t *accel_y, int16_t *accel_z);
int BNO055_GetGyr(int16_t *gyr_x, int16_t *gyr_y, int16_t *gyr_z);
int BNO055_GetEuler(int16_t *euler_x, int16_t *euler_y, int16_t *euler_z);

#endif // __IMU_H
