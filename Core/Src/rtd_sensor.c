#include "main.h"
#include <math.h>
#include "rtd_sensor.h"

extern ADC_HandleTypeDef hadc;


float rtd_read_temperature_f(void)
{
    // --- Constants ---
    const float VCC  = 3.3f;         // Excitation voltage
    const float RREF = 100.0f;       // Reference resistor (ohms)
    const float R0   = 100.0f;       // Pt100 resistance at 0 °C

    // IEC 60751 coefficients
    const float A =  3.9083e-3f;
    const float B = -5.775e-7f;
    const float C = -4.183e-12f;     // only for T < 0 °C

	uint32_t sum = 0;

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
	  Error_Handler();
	}
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

    // Take 10 samples
    for (int i = 0; i < 10; i++)
    {
        uint16_t raw = HAL_ADC_GetValue(&hadc);
        sum += raw;
        HAL_Delay(2); // small delay to avoid reading the exact same sample
    }

    // Calculate average raw ADC value
    float avg_raw = sum / 10.0f;
    float temperatureInF = 0.219*avg_raw - 344;
    return temperatureInF;
}
