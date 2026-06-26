#include "main.h"
#include <math.h>
#include <temp_meas.h>

extern ADC_HandleTypeDef hadc;


float rtd_read_temperature_f(uint32_t adcBuffer[])
{
	// value from adc
	float reading = (float) adcBuffer[0];

	return reading;
}
