#include "lcd.h"
#include <string.h>
#include <stdbool.h>

#define LCD_SETCGRAMADDR 0x40  // base command to set CGRAM address


// ─────────── Low-Level Helper Functions (Private) ───────────

/*
 *  Sends a command byte directly to the LCD controller over I2C.
 *  Commands configure the LCD’s behavior, such as clearing the screen
 *  or changing display modes.
 */
static void lcd_send_command(RGB_LCD_HandleTypeDef *lcd, uint8_t cmd) {
    uint8_t data[2] = {REG_ADDR, cmd};
    HAL_I2C_Master_Transmit(lcd->hi2c, LCD_ADDR, data, 2, HAL_MAX_DELAY);
}

/*
 * Sends a single data byte to the LCD controller over I2C. Data typically
 * corresponds to character codes that will be displayed on screen.
*/
static void lcd_send_data(RGB_LCD_HandleTypeDef *lcd, uint8_t data_byte) {
    uint8_t data[2] = {DATA_ADDR, data_byte};
    HAL_I2C_Master_Transmit(lcd->hi2c, LCD_ADDR, data, 2, HAL_MAX_DELAY);
}

//  ─────────── Public Functions (User API) ───────────

/*
 *  Initializes the LCD and its RGB backlight controller. This function sets
 *  up the display’s instruction set, clears the screen, configures contrast
 *  and oscillator settings, and prepares the backlight driver. It must be
 *  called before using other LCD functions.
*/
void RGB_LCD_Init(RGB_LCD_HandleTypeDef *lcd) {
    HAL_Delay(50);
    lcd_send_command(lcd, 0x38); // function set: 8-bit, 2 line, 5x8 dots
    lcd_send_command(lcd, 0x39); // function set with extended instruction
    lcd_send_command(lcd, 0x14); // internal OSC frequency
    lcd_send_command(lcd, 0x70); // contrast set
    lcd_send_command(lcd, 0x56); // power/icon/contrast control
    lcd_send_command(lcd, 0x6C); // follower control
    HAL_Delay(200);
    lcd_send_command(lcd, 0x38); // function set
    lcd_send_command(lcd, 0x0C); // display ON
    lcd_send_command(lcd, 0x01); // clear display
    HAL_Delay(2);

    uint8_t init_vals[] = {
        REG_MODE1,  0x00,
        REG_MODE2,  0x00,
		REG_OUTPUT, 0xAA
    };

    for (uint8_t i = 0; i < sizeof(init_vals); i += 2) {
        uint8_t tmp[2] = {init_vals[i], init_vals[i + 1]};
        HAL_I2C_Master_Transmit(lcd->hi2c, RGB_ADDR, tmp, 2, HAL_MAX_DELAY);
    }
    RGB_LCD_SetRGB(lcd, 255, 255, 255);
}

/*
 *  Clears all characters from the display and resets the cursor to the home
 *  position (top-left corner).
*/
void RGB_LCD_ClearScreen(RGB_LCD_HandleTypeDef *lcd) {
    lcd_send_command(lcd, 0x01);
    HAL_Delay(2);
}

/*
 *  Moves the memory pointer to a set position on the screen, based on col and row arguments.
 *  Use this before you write data to the screen in order to write at the correct position.
*/
void RGB_LCD_SetMemoryPointerAtScreenPosition(RGB_LCD_HandleTypeDef *lcd, uint8_t col, uint8_t row) {
    const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_send_command(lcd, 0x80 | (col + row_offsets[row]));
}

/*
 *  Sets the LCD’s backlight color by adjusting the brightness of the red, green,
 *  and blue LEDs independently.
*/
void RGB_LCD_SetRGB(RGB_LCD_HandleTypeDef *lcd, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t data[2];
    data[0] = REG_RED; data[1] = r;
    HAL_I2C_Master_Transmit(lcd->hi2c, RGB_ADDR, data, 2, HAL_MAX_DELAY);
    data[0] = REG_GREEN; data[1] = g;
    HAL_I2C_Master_Transmit(lcd->hi2c, RGB_ADDR, data, 2, HAL_MAX_DELAY);
    data[0] = REG_BLUE; data[1] = b;
    HAL_I2C_Master_Transmit(lcd->hi2c, RGB_ADDR, data, 2, HAL_MAX_DELAY);
}


/*
 *  Prints a string to the display at the current cursor position by sending
 *  each character sequentially.
 *
 *  WARNING: When trying to write custom characters to memory (numbers 0 - 7),
 *  remember that C treats a 0 as the null-terminator (i.e. end of string).
 *  It may be easier to write your own function to send only ONE byte to the LCD,
 *  to avoid this behavior when writing custom characters.
*/
void RGB_LCD_WriteStringToMemory(RGB_LCD_HandleTypeDef *lcd, const char *str) {
    while (*str) {
        lcd_send_data(lcd, *str++);
    }
}

/*
 *  Works like printf() in C, formatting text with variables before printing it
 *  to the LCD. Useful for displaying sensor readings, counters, or other
 *  dynamic data.
 *
 *
 *  WARNING: When trying to write custom characters to memory (numbers 0 - 7),
 *  remember that C treats a 0 as the null-terminator (i.e. end of string).
 *  It may be easier to write your own function to send only ONE byte to the LCD,
 *  to avoid this behavior when writing custom characters.
 *
*/
void RGB_LCD_WriteFormattedStringToMemory(RGB_LCD_HandleTypeDef *lcd, const char *fmt, ...) {
    char buffer[64]; // Adjust size for your LCD’s needs
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args); // format safely into buffer
    va_end(args);

    RGB_LCD_WriteStringToMemory(lcd, buffer);
}







