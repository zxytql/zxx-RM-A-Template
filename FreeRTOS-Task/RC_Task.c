/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       RC_Task.c/h
  * @brief      遥控器控制任务
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2022-04-05       zxy            First version
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */
#include "RC_Task.h"
#include "cmsis_os.h"

/******* Global Variables ********/
rc_t rc;

/* USER CODE BEGIN Header_RC_Task_Entry */
/**
* @brief RC_Task入口函数
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RC_Task_Entry */
_Noreturn void RC_Task_Entry(void const * argument)
{
    /* USER CODE BEGIN RC_Task_Entry */
    rc.test = 0;
    /* Infinite loop */
    for(;;)
    {
        Rc_Update(&rc.rc_KeyValue, usart1_buff);
        Rc_Key_Handler(&rc.rc_KeyValue,&rc.rc_key_event);
        Rc_Key_Callback_Handler(&rc.rc_key_event);

        osDelay(1);
    }
    /* USER CODE END RC_Task_Entry */
}

/**
 * @brief 遥控器数据更新函数，将通过USART1接收到的SBUS数据转换为键值
 * @param value:    遥控器键值结构体
 * @param rx:       经过转换后的十个通道数值
 * @param buff:     USART1串口数据 长度为25个字节
*/
void Rc_Update(rc_keyValue_t *value, uint8_t buff[25])
{
    rc.online = (buff[23] == 0x0C) ? Offline : Online;

    /** 获取通道值 **/
    rc_rx.ch[0] = ((buff[1] | buff[2] << 8) & 0x07FF);
    rc_rx.ch[1] = ((buff[2] >> 3 | buff[3] << 5) & 0x07FF);
    rc_rx.ch[2] = ((buff[3] >> 6 | buff[4] << 2 | buff[5] << 10) & 0x07FF);
    rc_rx.ch[3] = ((buff[5] >> 1 | buff[6] << 7) & 0x07FF);
    rc_rx.ch[4] = ((buff[6] >> 4 | buff[7] << 4) & 0x07FF);
    rc_rx.ch[5] = ((buff[7] >> 7 | buff[8] << 1 | buff[9] << 9) & 0x07FF);
    rc_rx.ch[6] = ((buff[9] >> 2 | buff[10] << 6) & 0x07FF);
    rc_rx.ch[7] = ((buff[10] >> 5 | buff[11] << 3) & 0x07FF);
    rc_rx.ch[8] = ((buff[12] | buff[13] << 8) & 0x07FF);
    rc_rx.ch[9] = ((buff[13] >> 3 | buff[14] << 5) & 0x07FF);

    /** 键值转换 **/
    rc.rc_KeyValue.left_horz_roll   = RC_DEAD_LIMIT(rc_rx.ch[3] - RC_VALUE_OFFSET);
    rc.rc_KeyValue.left_vir_roll    = RC_DEAD_LIMIT(1693 - rc_rx.ch[2]);
    rc.rc_KeyValue.right_horz_roll  = RC_DEAD_LIMIT(rc_rx.ch[0] - RC_VALUE_OFFSET);
    rc.rc_KeyValue.right_vir_roll   = RC_DEAD_LIMIT(rc_rx.ch[1] - RC_VALUE_OFFSET);

    rc.rc_KeyValue.key_a            = RC_KEY_VALUE_SWITCH(rc_rx.ch[4], 1000);
    rc.rc_KeyValue.key_b            = RC_KEY_VALUE_SWITCH(rc_rx.ch[5], 1250);
    rc.rc_KeyValue.key_c            = RC_KEY_VALUE_SWITCH3(rc_rx.ch[8], 500, 1500);
    rc.rc_KeyValue.key_d            = RC_KEY_VALUE_SWITCH(rc_rx.ch[9], 1000);

    rc.rc_KeyValue.left_knob        = RC_KNOB_TRANS(rc_rx.ch[6], 306, 1694);
    rc.rc_KeyValue.right_knob       = RC_KNOB_TRANS(rc_rx.ch[7], 306, 1694);
}

/**
 * @brief 遥控器按钮状态处理函数
 * @param value:    遥控器键值结构体
 * @param event:    遥控器按钮事件结构体
 */
void Rc_Key_Handler(rc_keyValue_t *value, rc_key_event_t *event)
{
    if(value->key_a == 1)  /** 按钮A被拨下 **/
    {
        if(event->key_a_now == 0)
        {
            event->key_a_now = 1;
            event->key_a_flag = 1;
        }
    }
    else
    {
        event->key_a_now = 0;
    }

    if(value->key_b == 1)  /** 按钮B被拨下 **/
    {
        if(event->key_b_now == 0)
        {
            event->key_b_now = 1;
            event->key_b_flag = 1;
        }
    }
    else
    {
        event->key_b_now = 0;
    }

    if(value->key_d == 1) /** 按钮D被拨下 **/
    {
        if(event->key_d_now == 0)
        {
            event->key_d_now = 1;
            event->key_d_flag = 1;
        }
    }
    else
    {
        event->key_d_now = 0;
    }
}

/**
 * @brief 遥控器按钮回调处理函数
 * @param event:    遥控器按键事件结构体
 * @note  轮询判断按键事件结构体中回调标志是否被触发 是则进入该函数
 */
void Rc_Key_Callback_Handler(rc_key_event_t *event)
{
    if(event->key_a_flag == 1)
    {
        Rc_Key_a_Callback();
        event->key_a_flag = 0;
    }
    if(event->key_b_flag == 1)
    {
        Rc_Key_b_Callback();
        event->key_b_flag = 0;
    }
    if(event->key_d_flag == 1)
    {
        Rc_Key_d_Callback();
        event->key_d_flag = 0;
    }
}

/**
 * @brief 按键回调函数
 * @note  经过回调处理函数处理过后会进入相应按钮回调函数
 */
void Rc_Key_a_Callback()
{

}

void Rc_Key_b_Callback()
{

}

void Rc_Key_d_Callback()
{
    rc.test++;
}