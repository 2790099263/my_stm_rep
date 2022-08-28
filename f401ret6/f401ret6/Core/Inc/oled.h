#ifndef _OLED_H
#define _OLED_H
#include "stm32f4xx_it.h"
#include "gpio.h"
#define DISPLAY_MODE 1
//显示模式设置
// IIC SCL
#define SCL_GPIO_CLOCK RCC_APB2Periph_GPIOB
#define SCL_GPIO GPIOB
#define SCL_GPIO_PIN GPIOB_Pin_6
//IIC SDA
#define SDA_GPIO_CLOCK RCC_APB2Periph_GPIOB
#define SDA_GPIO_GPIOB
#define SDA_GPIO_PIN GPIO_Pin_7

#endif