#include "main.h"         // includes hadc1
#include "button_input.h"   // your header


extern ADC_HandleTypeDef hadc;

Button GetButton(uint32_t adcBuffer[]) {

    // determine button push
    if (adcBuffer[0] < 50) return BUTTON_RIGHT;
    else if (adcBuffer[0] < 1100) return BUTTON_UP;
    else if (adcBuffer[0] < 1900) return BUTTON_DOWN;
    else if (adcBuffer[0] < 2700) return BUTTON_LEFT;
    else if (adcBuffer[0] < 3400) return BUTTON_SELECT;
    else return NONE;
}
