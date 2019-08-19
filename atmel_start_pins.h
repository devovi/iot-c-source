/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAML11 has 9 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8

//doubt
#define LIGHT_SENSOR GPIO(GPIO_PORTA, 2)
#define SERCOM0_SPI_MISO GPIO(GPIO_PORTA, 4)
//#define SERCOM0_SPI_MISO GPIO(GPIO_PORTA, 5)
//#define USER_LED0 GPIO(GPIO_PORTA, 7)
#define PA08 GPIO(GPIO_PORTA, 8)
//#define PA08 GPIO(GPIO_PORTA, 11)
#define PA09 GPIO(GPIO_PORTA, 9)
//#define PA09 GPIO(GPIO_PORTA, 12)
#define SERCOM0_SPI_MOSI GPIO(GPIO_PORTA, 14)
//#define SERCOM0_SPI_MOSI GPIO(GPIO_PORTA, 15)
#define SERCOM0_SPI_SCK GPIO(GPIO_PORTA, 15)
//#define SERCOM0_SPI_SCK GPIO(GPIO_PORTA, 16)
#define PA16 GPIO(GPIO_PORTA, 16)
//#define PA16 GPIO(GPIO_PORTA, 17)
#define PA17 GPIO(GPIO_PORTA, 17)
//#define PA17 GPIO(GPIO_PORTA, 18)
#define USER_BUTTON GPIO(GPIO_PORTA, 27)

#endif // ATMEL_START_PINS_H_INCLUDED
