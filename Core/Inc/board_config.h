#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#define ZXX_DEBUG 
#define ZXX_WIRELESS_DEBUG
/***********DJI MOTOR CONTROLL SWITCH***********/

#ifndef USING_HDC_DRIVER       //使用闭环板
    #define USING_BOARD_DRIVER //使用板载控制
#endif
/*************BOARD LED INIT STATE**************/

//#define BOARD_LED_INIT_ON

#endif
