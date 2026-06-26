
#ifndef __BUTTON_INPUT_H
#define __BUTTON_INPUT_H

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_SELECT,
	NONE
} Button;

Button GetButton(uint32_t adcBuffer[]);

#endif
