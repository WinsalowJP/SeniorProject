#include "imu.h"
#include <stdio.h>
#include "stm32wbxx_hal_i2c.h"
// bring in the I2C1 handle defined elsewhere
extern I2C_HandleTypeDef hi2c1;


int BNO055_Init(void)
{
	// Set the BNO055 into CONFIG mode (0x00) over I2C1
	{
	    uint8_t mode = 0x00;
	    HAL_Delay(100);
	    HAL_I2C_Mem_Write(&hi2c1, (0x28 << 1), 0x3D, I2C_MEMADD_SIZE_8BIT, &mode, 1, HAL_MAX_DELAY);
	}

}

int BNO055_SetMode(uint8_t mode)
{
    HAL_StatusTypeDef res;
    uint8_t value;

    // 1) Delay 100 ms
    HAL_Delay(100);

    // 2) Write 0x00 to PWR_MODE (power on / switch to normal)
    value = 0x00;
    res = HAL_I2C_Mem_Write(&hi2c1, BNO055_ADDR, BNO055_PWR_MODE, I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);
    if (res != HAL_OK) return -1;

    // 3) Delay 100 ms
    HAL_Delay(100);

    // 4) Write 0x00 to OPR_MODE (switch into CONFIG mode)
    value = 0x00;
    res = HAL_I2C_Mem_Write(&hi2c1,BNO055_ADDR,BNO055_OPR_MODE,I2C_MEMADD_SIZE_8BIT,&value,1,HAL_MAX_DELAY);
    if (res != HAL_OK) return -1;

    // 5) Delay 100 ms
    HAL_Delay(100);

    // 6) Write your desired mode to OPR_MODE
    value = mode;
    res = HAL_I2C_Mem_Write( &hi2c1,BNO055_ADDR,BNO055_OPR_MODE,I2C_MEMADD_SIZE_8BIT,&value,1,HAL_MAX_DELAY);
    if (res != HAL_OK) return -1;

    // 7) Wait 1 s for the mode switch to take effect
    HAL_Delay(1000);

    // 8) Read back 1 byte from SYS_STAT to confirm
    res = HAL_I2C_Mem_Read(&hi2c1, BNO055_ADDR,BNO055_SYS_STAT,I2C_MEMADD_SIZE_8BIT,&value,1,HAL_MAX_DELAY);
    if (res != HAL_OK) return -1;

    printf("Status is: 0x%02X\n", value);
    return 0;
}

int BNO055_GetCalibrationStatus(uint8_t *sys, uint8_t *gyro, uint8_t *accel, uint8_t *mag)
{
    uint8_t calib_byte;
    if (HAL_I2C_Mem_Read(&hi2c1, BNO055_ADDR, BNO055_CALIB_STAT,
                         I2C_MEMADD_SIZE_8BIT, &calib_byte, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("Error reading calibration status\n");
        return -1;
    }
    HAL_Delay(1000);
    *sys   = (calib_byte >> 6) & 0x03;  *gyro  = (calib_byte >> 4) & 0x03;
    *accel = (calib_byte >> 2) & 0x03;  *mag   = (calib_byte      ) & 0x03;
    return 0;
}


int BNO055_CalibrateIMU(void)
{
    uint8_t sys, gyro, accel, mag;

    printf("Starting IMU calibration procedure.\r\n");

    // Switch into NDOF (fusion) mode
    if (BNO055_SetMode(OPERATION_MODE_NDOF) != 0) {
        printf("Error: failed to set NDOF mode\r\n");
        return -1;
    }

    printf(
      "Please move the sensor in a figure-eight pattern to calibrate the magnetometer,\r\n"
      "and keep it steady for the accelerometer and gyroscope calibration.\r\n"
    );

    // Loop until everything is fully calibrated (value==3)
    while (1) {
        if (BNO055_GetCalibrationStatus(&sys, &gyro, &accel, &mag) == 0) {
            printf(
              "Calibration status: sys=%u, gyro=%u, accel=%u, mag=%u\r\n",
              sys, gyro, accel, mag
            );
            if (sys == 3 && gyro == 3 && accel == 3 && mag == 3) {
                printf("IMU calibration complete!\r\n");
                break;
            }
        } else {
            printf("Error reading calibration status\r\n");
        }
        HAL_Delay(1000);
    }

    return 0;
}


static int16_t read_vec3(uint8_t reg, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buf[6];
    if (HAL_I2C_Mem_Read(&hi2c1,
                         BNO055_ADDR,
                         reg,
                         I2C_MEMADD_SIZE_8BIT,
                         buf,
                         6,
                         HAL_MAX_DELAY) != HAL_OK) {
        return -1;
    }
    *x = (int16_t)(buf[0] | (buf[1] << 8));
    *y = (int16_t)(buf[2] | (buf[3] << 8));
    *z = (int16_t)(buf[4] | (buf[5] << 8));
    return 0;
}


int BNO055_GetMag(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) {
    return read_vec3(BNO055_MAG_REG, mag_x, mag_y, mag_z);
}

int BNO055_GetAccel(int16_t *accel_x, int16_t *accel_y, int16_t *accel_z) {
    return read_vec3(BNO055_ACCEL_REG, accel_x, accel_y, accel_z);
}

int BNO055_GetGyr(int16_t *gyr_x, int16_t *gyr_y, int16_t *gyr_z) {
    return read_vec3(BNO055_GYRO_REG, gyr_x, gyr_y, gyr_z);
}

int BNO055_GetEuler(int16_t *euler_x, int16_t *euler_y, int16_t *euler_z) {
    return read_vec3(BNO055_EULER_H, euler_x, euler_y, euler_z);
}
