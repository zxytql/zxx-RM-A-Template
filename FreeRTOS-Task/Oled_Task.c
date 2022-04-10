/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       Oled_Task.c/h
  * @brief      DJI Oled控制与显示任务
  * @note       1. bsp_oled.c/h中oled_printf不能使用，会导致程序堵塞，原因未知

  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2022-01-08       zxy            First version
  *  V1.0.1     2022-01-10       zxy            Added keys debounce
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */
#include "Oled_Task.h"
#include "cmsis_os.h"
#include "bsp_servo.h"
#include "bsp_usart.h"
#include "DJI_Motor_Ctrl.h"
#include "RC_Task.h"
#include "bsp_buzzer.h"

uint16_t oled_value = 0;
uint8_t oled_key_state = 0;
uint16_t oled_show_value = 0;
uint8_t key_up = 1;
uint16_t zxy1;

/* USER CODE BEGIN Header_Oled_Task_Entry */
/**
* @brief Oled_Task入口函数
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Oled_Task_Entry */
_Noreturn void Oled_Task_Entry(void const * argument)
{
    /* USER CODE BEGIN Oled_Task_Entry */
    oled_init();
    Servo_Init();
    /* Infinite loop */
    for(;;)
    {
        Led_On(LED_3_PORT,LED_3_PIN);

#ifdef ZXX_DEBUG
        oled_drawline(0,0,127,0,Pen_Write);
        oled_drawline(0,1,127,1,Pen_Write);

        oled_drawline(2,0,2,63,Pen_Write);
        oled_drawline(3,0,3,63,Pen_Write);

        oled_drawline(127,0,127,63,Pen_Write);
        oled_drawline(126,0,126,63,Pen_Write);

        oled_drawline(0,63,127,63,Pen_Write);
        oled_drawline(0,62,127,62,Pen_Write);  //Draw frame
        //oled_LOGO();
        //oled_printf(1,1,"Now_value: ");
        oled_showstring(1,1,"TEST:");
        oled_showstring(2,1,"key_state:");
        oled_showstring(3,1,"key_value:");
        //********************//
//        if (rc.rc_KeyValue.right_vir_roll < 0)
//        {
//            oled_show_value = -rc.rc_KeyValue.right_vir_roll;
//        }
//        else
//        {
//            oled_show_value = rc.rc_KeyValue.right_vir_roll;
//        }
        //********************//
        oled_shownum(1,11,rc.zxy1,0x00,5);
        oled_shownum(2,11,rc.rc_KeyValue.key_d,0x00,5);
        oled_shownum(3,11,oled_value,0x00,5);
#endif
        Oled_Key_Scan();  //按键扫描
        Oled_Value_trans(oled_value); //键值转换
        Oled_Action(oled_key_state); //按键回调函数
        oled_refresh_gram();
        osDelay(10);
    }
    /* USER CODE END Oled_Task_Entry */
}

void Oled_Key_Scan(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    oled_value = HAL_ADC_GetValue(&hadc1); //获Oled五维按键键值
}

void Oled_Value_trans(uint16_t value)
{
    if(value == 0)
    {
        //osDelay(300);
    }
    else if(value > 0 && value < 100)
    {
        osDelay(100);
        if(value > 0 && value < 100)
        {
            oled_key_state = press;
        }
    }
    else if(value > 800 && value <= 900)
    {
        osDelay(100);
        if(value > 800 && value <= 900)
        {
            oled_key_state = left;
        }
    }
    else if(value > 1700 && value <= 1800)
    {
        osDelay(100);
        if(value > 1700 && value <= 1800)
        {
            oled_key_state = right;
        }
    }
    else if(value > 2400 && value <= 2500)
    {
        osDelay(100);
        if(value > 2400 && value <= 2500)
        {
            oled_key_state = up;
        }
    }
    else if(value > 3200 && value <= 3300)
    {
        osDelay(100);
        if(value > 3200 && value <= 3300)
        {
            oled_key_state = down;
        }
    }
    else if(value > 4000 && value <= 4100)
    {
        if(value > 4000 && value <= 4100)
        {
            oled_key_state = none;
        }
    }
    else
    {
        return;
    }
}

void Oled_Action(uint8_t state)
{
    if(key_up == 1 && oled_key_state != none)
    {
        key_up = 0;
        if(state == press)
        {
            Oled_Key_Press_Callback();
        }
        else if(state == up)
        {
            Oled_Key_Up_Callback();
        }
        else if(state == down)
        {
            Oled_Key_Down_Callback();
        }
        else if(state == left)
        {
            Oled_Key_Left_Callback();
        }
        else if(state == right)
        {
            Oled_Key_Right_Callback();
        }
        else
        {
            return;
        }
    }
    if(key_up == 0 && oled_key_state == none)
    {
        key_up = 1;
    }
}


/*******************************************************************/
/****************************OLED 屏幕******************************/
/*******************************************************************/
/***************************回 调 函 数*****************************/
/*******************************************************************/
/*******************************************************************/

void Oled_Key_Press_Callback(void)
{
//		uint8_t zxy_buf[]= "CTIN";
//		zxy1 = sizeof(zxy_buf);
//		HAL_UART_Transmit(&huart8,zxy_buf,sizeof(zxy_buf),0xffff);
    //HAL_UART_Transmit_IT(&huart6,zxy_buf,sizeof(zxy_buf));
    //Servo_Ctrl('C',90);
    Warn_Buzzer();
}

void Oled_Key_Up_Callback(void)
{
    zxy1++;
    SetPos(0,4096 * zxy1);
}

void Oled_Key_Down_Callback(void)
{
    if(zxy1 > 0) {zxy1--;};
    SetPos(0,4096 * zxy1);
}

void Oled_Key_Left_Callback(void)
{

}

void Oled_Key_Right_Callback(void)
{

}
