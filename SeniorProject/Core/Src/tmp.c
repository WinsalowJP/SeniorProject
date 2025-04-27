/*
 * tmp.c
 *
 *  Created on: Apr 26, 2025
 *      Author: School
 */

#include "tmp.h"
#include <stdint.h>
extern I2C_HandleTypeDef hi2c3;

float read_temperature(void)
{
    uint8_t tempData[2];

    // Read 2 bytes from the temperature register
    if (HAL_I2C_Mem_Read(&hi2c3, TMP117_ADDR, TMP117_TEMP_REG, I2C_MEMADD_SIZE_8BIT, tempData, 2, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("TMP117 read error!\n");
        return -999.0f;  // Return a clearly erroneous value to indicate a failure
    }

    // Combine the two bytes into a 16-bit value (big-endian format)
    int16_t rawTemp = (int16_t)((tempData[0] << 8) | tempData[1]);

    // Convert the raw value to temperature in Celsius.
    // Each LSB represents 0.0078125°C
    float temperature = rawTemp * 0.0078125f;

    return temperature;
}
