/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       bsp_buzzer.c/h
  * @version    V1.0.0
  * @brief      无源蜂鸣器支持包
  * @note
  * 1. 无源蜂鸣器需要1.5~2.5kHz（与蜂鸣器种类有关）的音频信号，这个音频信号即一个高低变化频率为1.5-2.5kHz的方波
  * 2. 需要开启对应PWM以产生方波，并且只能在任务中调用
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2022-03-11       zxy            First version
  * @verbatim
  *
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */

#include "bsp_buzzer.h"


void buzzer_on(uint16_t psc, uint16_t pwm)
{
	TIM12->PSC = psc;
	TIM12->CCR1 = pwm;
}
void buzzer_off()
{
	TIM12->CCR1=0;
}

void ActionDoneBuzzer(void)
{
	
	    int sound[]=
    {
        M7,50,Z0,10,M7,50,Z0,10,M7,100,Z0,10,
       
    };

    int length = sizeof(sound)/sizeof(sound[0]);//计算数组长度
    for(int i=0; i<(length/2); i++)//取数组数据
    {
        buzzer_on(sound[2*i], 200);
        HAL_Delay(4*sound[2*i+1]);//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
    }
		buzzer_off();

}


/**
* NAME: void BeginWarnBuzzer(void)
* FUNCTION : 开机提醒音 do mi xi
*/
void BeginWarnBuzzer(void)
{
    int sound[]=
    {
        M3,50,M1,50,M5,100,Z0,150,
        // H3,25,Z0,25, H3,25,Z0,25, H3,25,Z0,25, H3,25,Z0,100, H3,150,Z0,25,
    };

    int length = sizeof(sound)/sizeof(sound[0]);//计算数组长度
    for(int i=0; i<(length/2); i++)//取数组数据
    {
        buzzer_on(sound[2*i], 200);
        HAL_Delay(4*sound[2*i+1]);//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
    }
		buzzer_off();

}

/**
* NAME: void IMUWarnBuzzer(void)
* FUNCTION : IMU校准提示音
*/
void IMUWarnBuzzer(void)
{
    int sound[]=
    {
        M7,50,Z0,150,
        H3,25,Z0,25, H3,25,Z0,25, H3,25,Z0,25, H3,25,Z0,100, H3,150,Z0,25,
    };

    int length = sizeof(sound)/sizeof(sound[0]);//计算数组长度
    for(int i=0; i<(length/2); i++)//取数组数据
    {
        buzzer_on(sound[2*i], 200);
        HAL_Delay(4*sound[2*i+1]);//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
    }

}

/**
* NAME: void happy_time(void)
* FUNCTION : happy
*/
void happy_time(void)
{
    int happy_birthday[]=
    {
        M5,50,M5,25,M5,25,
        M6,100,M5,100,H1,100,
        M7,100,M7,100,M5,50,M5,25,M5,25,
        M6,100,M5,100,H2,100,
        H1,100,H1,100,M5,50,M5,25,M5,25,
        H5,100,H3,100,H1,100,
        M7,100,M6,100,H4,50,H4,25,H4,25,
        H3,100,H1,100,H2,100,H1,100,H1,100
    };

    int length = sizeof(happy_birthday)/sizeof(happy_birthday[0]);//计算数组长度
    for(int i=0; i<(length/2); i++)//取数组数据
    {
        buzzer_on(happy_birthday[2*i], 200);
        HAL_Delay(5*happy_birthday[2*i+1]);//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
    }
}

void promising_young(void)
{
    int promising_young[]=
    {
        M5,50,M6,50,H1,50,Z0,10,H2,50,   Z0,10,H2,50,Z0,1,H2,50, Z0,1, H1,50, Z0,1,H2,50,   Z0,15, H1,50, Z0,15,H3,50,  Z0,15,Z0,50,
        M7,40,Z0,10,M7,40,Z0,10,M7,40,Z0,10,M7,75,Z0,15,M7,50, Z0,25, M6,25, Z0,25, H1,25,Z0,25

    };

    int length = sizeof(promising_young)/sizeof(promising_young[0]);//计算数组长度
    for(int i=0; i<(length/2); i++)//取数组数据
    {
        buzzer_on(promising_young[2*i], 200);
        HAL_Delay(5*promising_young[2*i+1]);//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
    }
}

/**
 * @brief 小星星 部分
 */
void LittleStar_Buzzer(void)
{
	int sound[] = 
	{
		//H1,30,M3,30,H1,30,M3,30 //,M3,50,H1,50,M3,50,H1,50,M3,50,Z0,100
        H1, 50, Z0, 50, H1, 50 , Z0, 50,
        H5, 50, Z0, 50,H5, 50, Z0, 50,
        H6, 50,Z0, 50,H6,50, Z0, 50,
        H5, 100,Z0, 100,
        H4, 50,Z0, 50, H4,50 ,Z0, 50,
        H3, 50, Z0, 50, H3, 50, Z0, 50,
        H2, 50, Z0, 50, H2, 50, Z0, 50,
        H1, 100,Z0, 100,
        H5,50,Z0, 50, H5, 50, Z0, 50,
        H4, 50,Z0, 50, H4, 50, Z0, 50,
        H3, 50, Z0, 50, H3, 50, Z0, 50,
        H2, 100 ,Z0, 100,
	};
	int length = sizeof(sound)/sizeof(sound[0]);//计算数组长度
  for(int i=0; i<(length/2); i++)//取数组数据
  {
    buzzer_on(sound[2*i], 200);
    HAL_Delay(4*sound[2*i+1]);//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
  }
	buzzer_off();
}

/**
 * @brief 警报声
 */
void Warn_Buzzer(void)
{
    int sound[] =
            {
                    H5,30,M7,30,H5,30,M7,30
            };
    int length = sizeof(sound)/sizeof(sound[0]);//计算数组长度
    for(int i=0; i<(length/2); i++)//取数组数据
    {
        buzzer_on(sound[2*i], 200);
        HAL_Delay(4*sound[2*i+1]);//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
    }
    buzzer_off();
}