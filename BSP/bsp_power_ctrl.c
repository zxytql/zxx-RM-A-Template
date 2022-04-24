/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       bsp_power_ctrl.c/h
  * @version    V1.0.0
  * @brief      DJI-RM-A开发板板载24V开关支持包
  * @note       引脚置高为使能24V
  * @history
  *  Version        Date            Author          Modification
  *  V1.0.0     2022-04-24          zxy            First version
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */
#include "bsp_power_ctrl.h"

/**
 * @brief   开启板载24V输出
 * @retval  NULL
 */
void BoardPowerOn(void)
{
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_3,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET);
}

/**
 * @brief   关闭板载24V输出
 * @retval  NULL
 */
void BoardPowerOff(void)
{
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_2,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_3,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET);
}