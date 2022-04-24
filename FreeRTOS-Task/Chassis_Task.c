/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       Chassis_Task.c/h
  * @brief      底盘运动控制任务
  * @note
  * @history
  *  Version        Date            Author          Modification
  *  V1.0.0     2022-04-23          zxy            First version
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */
#include "Chassis_Task.h"
#include "DJI_Motor_Ctrl.h"

/******** Global Variables ********/

/* USER CODE BEGIN Header_Chassis_Task_Entry */
/**
* @brief Function implementing the Chassis_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Chassis_Task_Entry */
_Noreturn void Chassis_Task_Entry(void const * argument)
{
    /* USER CODE BEGIN Chassis_Task_Entry */
    Chassis_Param_Init(&chassis);
    /* Infinite loop */
    for(;;)
    {
        Chassis_action(&chassis, &rc.rc_KeyValue);
        osDelay(10);
    }
    /* USER CODE END Chassis_Task_Entry */
}

void Chassis_Param_Init(chassis_t *chs)
{
    chs->chassis_rc_co.vx_co = 0.003f;
    chs->chassis_rc_co.vy_co = 0.003f;
    chs->chassis_rc_co.wz_co = 0.003f;
}
/**
 * @brief 底盘运动控制函数
 * @param chs: 底盘运动结构体
 * @param rcKeyValue: 遥控器键值结构体
 */
void Chassis_action(chassis_t *chs, rc_keyValue_t *rcKeyValue)
{
    chs->chassis_ct.ct_vy = rcKeyValue->left_vir_roll * chs->chassis_rc_co.vx_co;
    chs->chassis_ct.ct_vx = rcKeyValue->right_horz_roll * chs->chassis_rc_co.vy_co;
    chs->chassis_ct.ct_wz = rcKeyValue->left_horz_roll * chs->chassis_rc_co.wz_co;
    kinematics_inverse_omni3(chs);
    for (int i = 0; i < 3; ++i)
    {
        SetSpeed(i,chs->wheel_speed[i]);
    }
}