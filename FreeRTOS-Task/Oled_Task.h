#ifndef _OLED_TASK_H_
#define _OLED_TASK_H_

#include "../board_config.h"
#include "adc.h"
#include "bsp_oled.h"
#include "bsp_led.h"
#include "bsp_can.h"

#ifdef USING_HDC_DRIVER
#include "hdc_driver.h"
#endif

enum
{
/* 0      1    2     3     4     5  */	
	none, press, up, left, down, right 
}Oled_Key_State_t;

void Oled_Key_Scan(void);
void Oled_Value_trans(uint16_t value);
void Oled_Action(uint8_t);
void Oled_Key_Up_Callback(void);
void Oled_Key_Down_Callback(void);
void Oled_Key_Left_Callback(void);
void Oled_Key_Right_Callback(void);
void Oled_Key_None_Callback(void);
void Oled_Key_Press_Callback(void);
#endif
