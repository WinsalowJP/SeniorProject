/*
 * gsr.h
 *
 *  Created on: Apr 26, 2025
 *      Author: James Pan
 */

#ifndef GSR_H_
#define GSR_H_
#include <stdint.h>
#include <stdio.h>
#include "main.h"

//GSR VALUES
#define ADC_RESOLUTION 4095.0f   // 12-bit ADC: 2^12 - 1 = 4095
#define ADC_VREF       5.0f      // Reference voltage in volts
#define V_IN             5.0f        // Supply voltage (5V)
#define R_FIXED          8600.0f    // Fixed resistor in ohms (10 kΩ)
#define SERIAL_CALIBRATION   3140

uint32_t read_adc_value(void);
uint32_t read_adc1_channel(uint32_t channel);
float read_adc_voltage(uint32_t channel);
uint32_t read_adc_value_avg(uint32_t channel,uint8_t numSamples);
float calculate_human_resistance(uint32_t adcReading, uint32_t calibrationReading);

#endif /* GSR_H_ */
