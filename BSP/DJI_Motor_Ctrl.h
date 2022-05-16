#ifndef _DJI_MOTOR_CTRL_H
#define _DJI_MOTOR_CTRL_H

#include "main.h"
#include "math.h"
#include "board_config.h"
typedef enum
{
	RM_3508 = 1, 
	M_2006 = 2, 
	NONE = 3  //none表示没有接电机

} MotorType_TypeDef;

/**
 * @brief  Robomaster Motor 3508/2006 type structure definition
 * @note
 */
typedef struct
{
    int32_t pos, posLast;

    MotorType_TypeDef type;

    int32_t vel;        //电机返回的速度，单位： rpm

    int16_t cur;        //电机返回的电流值

    int8_t temp;        //电机的温度

} MotorType;

int32_t GetMotorPos(int i);

/**
 * @brief  Encoder PulseMode type structure definition
 * @note
 */
typedef struct
{
    int32_t Cnt;

    float Vel;

} PulseModeType;
/**
 * @brief  Encoder TimeMode type structure definition
 * @note
 */
typedef struct
{
    int32_t Tim;    //间隔包含的100us的个数

    float TimUite;

    int32_t TimNum; //间隔总时间 单位：us

    float Vel;

} TimeModeType;

/**
 * @brief  Encoder type structure definition
 * @note
 */
typedef struct
{
    TimeModeType TimeMode;

    PulseModeType PulseMode;

    int32_t period;

    int32_t Direct;

} EncoderType;

/**
 * @brief  Commutation type structure definition
 * @note
 */
typedef struct
{
    int32_t Mode;

} CommutationType;

/**
 * @brief  Commutation type structure definition
 * @note
 */
typedef struct
{
    int32_t can_status;

    int32_t canId;

} CommandType;

/**
 * @brief  VelCtrl type structure definition
 * @note
 */
typedef struct
{
    float speed;

    float desiredVel[3];

    float velErr;

    float acc;

    float dec;

    float kp;

    float ki;

    float iOut;

    float output;

    float maxOutput;

} VelCtrlType;

/**
 * @brief  PosCtrl type structure definition
 * @note
 */
typedef struct
{
    float actualPos;

    float desiredPos;

    float posErr, posErrLast;

    float posVel, acc;

    float basicPos;

    float kp;

    float kd;

    float output;

} PosCtrlType;

/**
 * @brief  HomingMode type structure definition
 * @note
 */
typedef struct
{
    float vel;

    float current;

    float initPos;

    int32_t cnt;

    float output;

} HomingModeType;

/**
 * @brief  Driver type structure definition
 * @note
 */
typedef struct
{
    uint32_t unitMode;

    int32_t time;

    FunctionalState status;

    float output;

    VelCtrlType velCtrl;

    PosCtrlType posCtrl;

    HomingModeType homingMode;

    EncoderType encoder;

    CommutationType commutation;

    CommandType command;

} DriverType;

/**
 * @brief  Control type structure definition
 * @note
 */
typedef struct
{
    float DesiredValue[8];

    float KP[4];

    float KI[4];

} CtrlType;

/********************FOC参数******************************/
#define  VOL_AMP                    1.10f           //Voltage amplitude 作用于电流的电压幅值约0.8--13.6A
#define  VOL_MAX                    18.00f          //电压最大值
#define  VOL_BLIND_AREA             0.80f           //输出盲区，电机不动的最大矢量电压值
#define  EMF_CONSTANT               0.020926f       //电动势常数，电机本身参数 = 电压矢量(V)/速度(pulse/ms)

#define  CURRENT_MAX_3508           20.0f
#define  CURRENT_MAX_2006           6.0f

#define  VEL_MAX_3508               1280.0f         //最大速度
#define  VEL_KP_3508                0.1f            //速度环Kp
#define  VEL_KI_3508                0.001f          //速度环Ki
#define  POS_KP_3508                0.11f
#define  POS_KD_3508                0.0f

#define  VEL_MAX_2006               2400.0f         //最大速度
#define  VEL_KP_2006                0.05f           //速度环Kp
#define  VEL_KI_2006                0.003f          //速度环Ki
#define  POS_KP_2006                0.027f
#define  POS_KD_2006                0.05f

//驱动器工作模式
#define  SPEED_CONTROL_MODE             2
#define  POSITION_CONTROL_MODE          5
#define  HOMING_MODE                    6
//区别使用斜坡前后的速度
#define  CMD   0
#define  SOFT  1
#define  MAX_V 2

//换相模式
#define  BLDC_MODE              1
#define  FOC_MODE               2

#define  CAN_ID_NUM     5
//自动5号初始  电流为2.5其余为1.5

/******** Extern ********/
extern DriverType Driver[8];
extern MotorType Motor[8];
/* Exported functions ------------------------------------------------------- */
float OutPutLim(float val);
float VelSlope(VelCtrlType *velPid);
float VelPidCtrl(VelCtrlType *velPid);
float PosCtrl(PosCtrlType *posPid);
//float     VelCtrl(float cmdVel);
void VelCtrlInit(void);
void PosCtrlInit(void);
//float     CalculSpeed(void);
float CalculateSpeed_Pos(DriverType *driver, MotorType *motor);
float GetVelPidOut(void);
void SetSpeed(int n, float rpm);
void SetPos(int n, float pos);
float GetSpeed(void);
float GetPosPidOut(void);
float MaxMinLimit(float val, float limit);
void DriverInit(int i, int motor_type, int unit_mode);
void MotorCtrl(void);
void HomingMode(DriverType *driver);
void HomingModeInit(void);
void MotorOn(int n);
void MotorOff(int n);
void VelCtrlTest(float vel, int tim);
void MotorOffMulti(int n0, int n1, int n2, int n3);
void MotorOnMulti(int n0, int n1, int n2, int n3);
void SetCur(int16_t MotorCur0, int16_t MotorCur1, int16_t MotorCur2, int16_t MotorCur3);
void SetCur1(int16_t MotorCur4, int16_t MotorCur5, int16_t MotorCur6, int16_t MotorCur7);
void CanId_Init();

//void CustomMotorInit(int n, int motor_type, int unit_mode);
#endif
