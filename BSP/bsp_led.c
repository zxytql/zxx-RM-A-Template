/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       bsp_led.c/h
  * @version    V1.0.0
  * @brief      开发板LED支持包
  * @note       基于RM-A板开发，更换开发板可能需要更换对应引脚
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2022-01-08       zxy            First version
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */

#include "bsp_led.h"
#include "main.h"

int led_cnt = 0;

//_led_t led_t;
/**
	* @brief 打开LED灯
	* @param GPIO: LED所在端口
	* @param GPIO_Pin: LED所在引脚
	*/
void Led_On(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIO,GPIO_Pin,GPIO_PIN_RESET);
}

/**
	* @brief 关闭LED灯
	* @param GPIO: LED所在端口
	* @param GPIO_Pin: LED所在引脚
	*/
void Led_Off(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIO,GPIO_Pin,GPIO_PIN_SET);
}
/**
	* @brief 关闭所有LED灯
	*/
void Led_All_Off(void)
{
	Led_Off(LED_1_PORT,LED_1_PIN | LED_2_PIN | LED_3_PIN | LED_4_PIN | LED_5_PIN | LED_6_PIN | LED_7_PIN | LED_8_PIN); 
}
/**
	* @brief 打开所有LED灯
	*/
void Led_All_On(void)
{
	Led_On(LED_1_PORT,LED_1_PIN | LED_2_PIN | LED_3_PIN | LED_4_PIN | LED_5_PIN | LED_6_PIN | LED_7_PIN | LED_8_PIN); 
}

/**
	* @brief LED灯闪烁
	* @param GPIO: LED所在端口
	* @param GPIO_Pin: LED所在引脚
	* @note 只能在一个线程中调用一次，调用多次会出现bug
	*/
void Led_Flash(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin)
{
	if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC2) != RESET) //判断TIM2中断更新标志位
	{
		led_cnt++;
	}
	if(led_cnt >= 300) //300ms
	{
		HAL_GPIO_TogglePin(GPIO,GPIO_Pin);
		led_cnt = 0;
	}
}
