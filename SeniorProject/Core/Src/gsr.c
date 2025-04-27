/*
 * gsr.c
 *
 *  Created on: Apr 26, 2025
 *      Author: School
 */
#include "gsr.h"
#include <stdint.h>

extern ADC_HandleTypeDef hadc1;
uint32_t read_adc_value(void)
{
    uint32_t adcValue = 0;

    // Start ADC conversion
    HAL_ADC_Start(&hadc1);

    // Wait for conversion completion (timeout in milliseconds)
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        // Retrieve ADC conversion result
        adcValue = HAL_ADC_GetValue(&hadc1);
    }
    else
    {
        // Optional: handle error
        printf("ADC conversion error!\r\n");
    }

    // Stop ADC (optional for single conversion mode)
    HAL_ADC_Stop(&hadc1);

    return adcValue;
}

float read_adc_voltage(void)
{
    uint32_t adc_raw = read_adc_value();
    // Convert raw ADC value to voltage:
    // voltage = (adc_raw / ADC_RESOLUTION) * ADC_VREF
    float voltage = ((float)adc_raw / ADC_RESOLUTION) * ADC_VREF;
    return voltage;
}

uint32_t read_adc_value_avg(uint8_t numSamples)
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < numSamples; i++)
    {
        sum += read_adc_value();
        HAL_Delay(5);
    }
    return (sum / numSamples);
}

float calculate_human_resistance(uint32_t adcReading, uint32_t calibrationReading)
{
    if (calibrationReading <= adcReading) {
        // Avoid division by zero or negative values
        return 0.0f;
    }
    float numerator = (4096.0f + 2.0f * (float)adcReading) * 10000.0f;
    float resistance = numerator / ((float)calibrationReading - (float)adcReading);
    return resistance;
}
