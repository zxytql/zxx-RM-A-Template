/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       OLED_Task.c/h
  * @brief      DJI OLED控制与显示任务
  * @note       1. bsp_oled.c/h中oled_printf不能使用，会导致程序堵塞，原因未知

  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2022-01-08       zxy            First version
  *  V1.0.1     2022-01-10       zxy             添加按键消抖
  *  V1.1.0     2022-03-31       zxy             优化按键逻辑
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */
#include "Oled_Task.h"
#include "cmsis_os.h"
#include "bsp_servo.h"
#include "bsp_usart.h"
#include "DJI_Motor_Ctrl.h"
#include "RC_Task.h"
#include "bsp_buzzer.h"

uint16_t OLED_value = 0;
uint8_t OLED_key_state = 0;
uint16_t OLED_show_value = 0;
uint8_t key_up = 1;
uint16_t zxy1;

/* USER CODE BEGIN Header_OLED_Task_Entry */
/**
* @brief Oled_Task入口函数
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLED_Task_Entry */
_Noreturn void OLED_Task_Entry(void const * argument)
{
    /* USER CODE BEGIN OLED_Task_Entry */
    oled_init();
    Servo_Init();
    /* Infinite loop */
    for(;;)
    {
        Led_On(LED_3_PORT,LED_3_PIN);

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
        oled_showstring(1,1,"Left_Vir:");
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
        oled_shownum(2,11,rc.rc_KeyValue.left_vir_roll,0x00,5);
        oled_shownum(2,11,rc.rc_KeyValue.key_d,0x00,5);
        oled_shownum(3,11,OLED_value,0x00,5);

        OLED_Key_Scan();  //按键扫描
        OLED_Value_trans(OLED_value); //键值转换
        OLED_Action(OLED_key_state); //按键回调函数
        oled_refresh_gram();
        osDelay(10);
    }
    /* USER CODE END Oled_Task_Entry */
}

void OLED_Key_Scan(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    OLED_value = HAL_ADC_GetValue(&hadc1); //获Oled五维按键键值
}

void OLED_Value_trans(uint16_t value)
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
            OLED_key_state = press;
        }
    }
    else if(value > 800 && value <= 900)
    {
        osDelay(100);
        if(value > 800 && value <= 900)
        {
            OLED_key_state = left;
        }
    }
    else if(value > 1700 && value <= 1800)
    {
        osDelay(100);
        if(value > 1700 && value <= 1800)
        {
            OLED_key_state = right;
        }
    }
    else if(value > 2400 && value <= 2500)
    {
        osDelay(100);
        if(value > 2400 && value <= 2500)
        {
            OLED_key_state = up;
        }
    }
    else if(value > 3200 && value <= 3300)
    {
        osDelay(100);
        if(value > 3200 && value <= 3300)
        {
            OLED_key_state = down;
        }
    }
    else if(value > 4000 && value <= 4100)
    {
        if(value > 4000 && value <= 4100)
        {
            OLED_key_state = none;
        }
    }
    else
    {
        return;
    }
}

void OLED_Action(uint8_t state)
{
    if(key_up == 1 && OLED_key_state != none)
    {
        key_up = 0;
        if(state == press)
        {
            OLED_Key_Press_Callback();
        }
        else if(state == up)
        {
            OLED_Key_Up_Callback();
        }
        else if(state == down)
        {
            OLED_Key_Down_Callback();
        }
        else if(state == left)
        {
            OLED_Key_Left_Callback();
        }
        else if(state == right)
        {
            OLED_Key_Right_Callback();
        }
        else
        {
            return;
        }
    }
    if(key_up == 0 && OLED_key_state == none)
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

void OLED_Key_Press_Callback(void)
{

}

void OLED_Key_Up_Callback(void)
{
    //SetPos(5,8192);
}

void OLED_Key_Down_Callback(void)
{
    //SetPos(5,-8192);
}

void OLED_Key_Left_Callback(void)
{

}

void OLED_Key_Right_Callback(void)
{

}
