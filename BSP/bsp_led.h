#ifndef BSP_LED_H
#define BSP_LED_H

#include "gpio.h"
#include "tim.h"

#ifdef STM32F427xx
	#define LED_NUMS 8
	#define LED_1_PORT GPIOG
	#define LED_2_PORT GPIOG
	#define LED_3_PORT GPIOG
	#define LED_4_PORT GPIOG
	#define LED_5_PORT GPIOG
	#define LED_6_PORT GPIOG
	#define LED_7_PORT GPIOG
	#define LED_8_PORT GPIOG

	#define LED_1_PIN GPIO_PIN_1
	#define LED_2_PIN GPIO_PIN_2
	#define LED_3_PIN GPIO_PIN_3
	#define LED_4_PIN GPIO_PIN_4
	#define LED_5_PIN GPIO_PIN_5
	#define LED_6_PIN GPIO_PIN_6
	#define LED_7_PIN GPIO_PIN_7
	#define LED_8_PIN GPIO_PIN_8
	
#endif

//typedef struct
//{
//	uint16_t led_cnt;
//} _led_t;
void Led_On(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin);
void Led_Off(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin);
void Led_All_Off(void);
void Led_All_On(void);
void Led_Flash(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin);

#endif
