/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       DJI_Motor_Task.c/h
  * @brief      DJI电机控制任务
  * @note       1. 使用前需在board_config.h中选择控制方式，不同控制方式对应函数不同，
	                 闭环板控制参考文件hdc_driver.c/h，板载控制参考DJI_Motor_Ctrl.c/h
								2. 
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2022-01-08       zxy            First version
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */
#include "DJI_Motor_Task.h"
#include "cmsis_os.h"
#include "DJI_Motor_Ctrl.h"

/* USER CODE BEGIN Header_DJI_Motor_Entry */
/**
* @brief Function implementing the DJI_Motor_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DJI_Motor_Entry */
_Noreturn void DJI_Motor_Entry(void const * argument)
{
  /* USER CODE BEGIN DJI_Motor_Entry */
#ifdef USING_BORAD_DRIVER
  DriverInit();
#endif
  /* Infinite loop */
  for(;;)
  {
#ifdef USING_BORAD_DRIVER
      MotorCtrl();
#endif
      osDelay(1);
  }
  /* USER CODE END DJI_Motor_Entry */
}
