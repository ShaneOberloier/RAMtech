#ifndef LCD_H
#define LCD_H

#include "stm32l1xx_hal.h"
#include <stdint.h>
#include <stdarg.h>  // for va_list, va_start, va_end
#include <stdio.h>   // for snprintf
#include <stdbool.h>

#define LCD_ADDR       (0x3E << 1) // 8-bit format for HAL
#define RGB_ADDR       (0x2D << 1) // RGB controller address (PCA9633)
#define REG_ADDR       0x00
#define DATA_ADDR      0x40

#define REG_RED        0x04
#define REG_GREEN      0x03
#define REG_BLUE       0x02

#define REG_MODE1      0x00
#define REG_MODE2      0x01
#define REG_OUTPUT     0x08

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t rgb_addr; // RGB controller I2C address (<<1 already)
    uint8_t cols;
    uint8_t rows;
} RGB_LCD_HandleTypeDef;

void RGB_LCD_Init(RGB_LCD_HandleTypeDef *lcd);
void RGB_LCD_ClearScreen(RGB_LCD_HandleTypeDef *lcd);
void RGB_LCD_SetMemoryPointerAtScreenPosition(RGB_LCD_HandleTypeDef *lcd, uint8_t col, uint8_t row);
void RGB_LCD_WriteStringToMemory(RGB_LCD_HandleTypeDef *lcd, const char *str);
void RGB_LCD_SetRGB(RGB_LCD_HandleTypeDef *lcd, uint8_t r, uint8_t g, uint8_t b);
void RGB_LCD_WriteFormattedStringToMemory(RGB_LCD_HandleTypeDef *lcd, const char *fmt, ...);


#endif // __RGB_LCD_H__
