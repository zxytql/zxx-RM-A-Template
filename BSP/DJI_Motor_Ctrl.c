/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       DJI_Motor_Ctrl.c/h
  * @version    V1.0.0
  * @brief      DJI电机控制算法
  * @note       
  * @history
  *  Version        Date        Author          Modification
  *  V1.0.0     2022-01-08       zxy            First version
  *  V1.1.0     2022-04-10       zxy            修改了初始化方式
  * @verbatim
  * [V1.1.0]
  * 1. 添加了CanIDInit函数，将DriverInit中的初始化部分转移到该函数中，只需执行一次
  * 2. V1.0.0中的DriverInit函数，执行时会将8个电机全部初始化为同一类型同一控制方式，
  *    与实际情况不符合，现可以根据需要填入形参，初始化对应电机的类型和控制方式
  * ============================================================================
  *                                 使用说明
  * ============================================================================
  * 1. 新建一个任务，控制周期需根据实际情况调整osDelay(ms)的值
  * 2. 在任务初始化部分，添加CanIDInit函数和DriverInit函数
  * 3. 在任务循环部分执行MotorCtrl函数
  * 4. 调用SetPos和SetSpeed控制函数即可进行闭环控制，需与电机初始化控制方式相同
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */

#include "DJI_Motor_Ctrl.h"
#include "bsp_can.h"

#ifdef USING_BOARD_DRIVER
/****TEST VIR*****/
int test1 = 0;
int test2 = 0;

/******** Global Variables *********/
DriverType Driver[8];
MotorType Motor[8];

/**
 * @brief 初始化Can NodeID，在DriveInit控制任务初始化前需运行一次
 *
 */
void CanId_Init()
{
    Driver[0].command.canId = 1;
    Driver[1].command.canId = 2;
    Driver[2].command.canId = 3;
    Driver[3].command.canId = 4;
    Driver[4].command.canId = 5;
    Driver[5].command.canId = 6;
    Driver[6].command.canId = 7;
    Driver[7].command.canId = 8;
}

/**
 * @brief 初始化单个电机控制方式
 * @param n:            电机ID减1的值
 * @param motor_type:   电机型号: RM_3508, M_2006
 * @param unit_mode:    控制方式: SPEED_CONTROL_MODE, POSITION_CONTROL_MODE
 */
void DriverInit(int i, int motor_type, int unit_mode)
{
    Driver[i].status = ENABLE;
    Driver[i].encoder.period = 8192;

    Driver[i].posCtrl.actualPos = 0.0f;
    Motor[i].type = motor_type;
    MotorOn(i);

    if (Motor[i].type == RM_3508) {
        /*
         * HOMING_MODE
         * POSITION_CONTROL_MODE
         * */
        //Driver[i].unitMode = SPEED_CONTROL_MODE;
        Driver[i].unitMode = unit_mode;
        Driver[i].velCtrl.kp = VEL_KP_3508;
        Driver[i].velCtrl.ki = VEL_KI_3508;
        Driver[i].velCtrl.maxOutput = CURRENT_MAX_3508;
        Driver[i].velCtrl.desiredVel[MAX_V] = VEL_MAX_3508;
        Driver[i].posCtrl.kd = POS_KD_3508;
        Driver[i].posCtrl.kp = POS_KP_3508;
        Driver[i].homingMode.current = 0.8f;

        Driver[i].velCtrl.acc = 1000.0f;
        Driver[i].velCtrl.dec = 1000.0f;
        Driver[i].velCtrl.desiredVel[CMD] = 0.0f;
        Driver[i].posCtrl.acc = Driver[i].velCtrl.dec;
        Driver[i].posCtrl.posVel = 50.0f;
        Driver[i].homingMode.vel = -160.0f;
    }
    else if (Motor[i].type == M_2006) {
        Driver[i].unitMode = unit_mode;
        Driver[i].velCtrl.kp = VEL_KP_2006;
        Driver[i].velCtrl.ki = VEL_KI_2006;
        Driver[i].velCtrl.maxOutput = CURRENT_MAX_2006;
        Driver[i].velCtrl.desiredVel[MAX_V] = VEL_MAX_2006;
        Driver[i].posCtrl.kd = POS_KD_2006;
        Driver[i].posCtrl.kp = POS_KP_2006;
        Driver[i].homingMode.current = 0.8f;

        Driver[i].velCtrl.acc = 1000.0f;
        Driver[i].velCtrl.dec = 1000.0f;
        Driver[i].velCtrl.desiredVel[CMD] = 0.0f;
        Driver[i].posCtrl.acc = Driver[i].velCtrl.dec;
        Driver[i].posCtrl.posVel = 50.0f;
        Driver[i].homingMode.vel = -160.0f;
    } else { ;
    }
}

/**
 * @brief  电机控制
 * @note   控制频率需要根据实际情况调整
 */
void MotorCtrl(void)
{
    float PerCur[8] = { 0.0f };

    for (int i = 0; i < 8; ++i)
    {
        if (Motor[i].type == NONE)
            break;

        CalculateSpeed_Pos(&Driver[i], &Motor[i]);

        if (Driver[i].status != ENABLE)
        {
            Driver[i].output = 0.0f;
            continue;
        }

        switch (Driver[i].unitMode)
        {
            case POSITION_CONTROL_MODE:
                PosCtrl(&Driver[i].posCtrl);
                Driver[i].velCtrl.desiredVel[CMD] = Driver[i].posCtrl.output;
                VelSlope(&Driver[i].velCtrl);
                Driver[i].output = VelPidCtrl(&Driver[i].velCtrl);
                break;
            case SPEED_CONTROL_MODE:
                VelSlope(&Driver[i].velCtrl);
                Driver[i].output = VelPidCtrl(&Driver[i].velCtrl);
                break;
            case HOMING_MODE:
                HomingMode(&Driver[i]);
                Driver[i].output = Driver[i].homingMode.output;
                break;
            default:
                break;
        }
    }

    for (int i = 0; i < 8; ++i)
    {
        if (Motor[i].type == RM_3508)
            PerCur[i] = Driver[i].output * 16384.0f / 20.0f;
        else if (Motor[i].type == M_2006)
            PerCur[i] = Driver[i].output * 10000.0f / 10.0f;
        else
            PerCur[i] = 0.0f;
    }
    if (!PerCur[0] || !PerCur[1] || !PerCur[2] || !PerCur[3])
    {
        SetCur((int16_t) PerCur[0], (int16_t) PerCur[1], (int16_t) PerCur[2], (int16_t) PerCur[3]);
    }
    if (!PerCur[4] || !PerCur[5] || !PerCur[6] || !PerCur[7])
    {
        SetCur1((int16_t) PerCur[4], (int16_t) PerCur[5], (int16_t) PerCur[6], (int16_t) PerCur[7]);
    }
}

/**
  * @brief  设定电机的运行电流
  * @param
  * @param
  * @retval
  */
void SetCur(int16_t MotorCur0, int16_t MotorCur1, int16_t MotorCur2, int16_t MotorCur3)
{
    test1++;
    uint32_t TxMailbox;
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxMessage[8];


    TxHeader.StdId = 0x200;         // standard identifier=0
    TxHeader.ExtId = 0x200;         // extended identifier=StdId
    TxHeader.IDE = CAN_ID_STD; // type of identifier for the message is Standard
    TxHeader.RTR = CAN_RTR_DATA;    // the type of frame for the message that will be transmitted
    TxHeader.DLC = 8;

    TxMessage[0] = MotorCur0 >> 8;
    TxMessage[1] = MotorCur0;
    TxMessage[2] = MotorCur1 >> 8;
    TxMessage[3] = MotorCur1;
    TxMessage[4] = MotorCur2 >> 8;
    TxMessage[5] = MotorCur2;
    TxMessage[6] = MotorCur3 >> 8;
    TxMessage[7] = MotorCur3;

    //CAN_Transmit_Messages_Byte(&hcan1,TxHeader.StdId,TxMessage[0],TxMessage[1],TxMessage[2],TxMessage[3],TxMessage[4],TxMessage[5],TxMessage[6],TxMessage[7]);
    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxMessage, &TxMailbox);
}

/**
 * @brief
 * @param MotorCur4
 * @param MotorCur5
 * @param MotorCur6
 * @param MotorCur7
 */
void SetCur1(int16_t MotorCur4, int16_t MotorCur5, int16_t MotorCur6, int16_t MotorCur7)
{
    test1++;
    uint32_t TxMailbox;
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxMessage[8];


    TxHeader.StdId = 0x1ff;         // standard identifier=0
    TxHeader.ExtId = 0x1ff;         // extended identifier=StdId
    TxHeader.IDE = CAN_ID_STD; // type of identifier for the message is Standard
    TxHeader.RTR = CAN_RTR_DATA;    // the type of frame for the message that will be transmitted
    TxHeader.DLC = 8;

    TxMessage[0] = MotorCur4 >> 8;
    TxMessage[1] = MotorCur4;
    TxMessage[2] = MotorCur5 >> 8;
    TxMessage[3] = MotorCur5;
    TxMessage[4] = MotorCur6 >> 8;
    TxMessage[5] = MotorCur6;
    TxMessage[6] = MotorCur7 >> 8;
    TxMessage[7] = MotorCur7;

    //CAN_Transmit_Messages_Byte(&hcan1,TxHeader.StdId,TxMessage[0],TxMessage[1],TxMessage[2],TxMessage[3],TxMessage[4],TxMessage[5],TxMessage[6],TxMessage[7]);
    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxMessage, &TxMailbox);
}
/**
 * @brief  速度斜坡输入
 * @param  None
 * @retval 速度期望输出
 */
float VelSlope(VelCtrlType *velPid)
{
    /*************计算加减速度斜坡**************/
    if (velPid->desiredVel[SOFT] < (velPid->desiredVel[CMD] - velPid->acc))
    {
        velPid->desiredVel[SOFT] += velPid->acc;
    }
    else if (velPid->desiredVel[SOFT] > (velPid->desiredVel[CMD] + velPid->dec))
    {
        velPid->desiredVel[SOFT] -= velPid->dec;
    }
    else
    {
        velPid->desiredVel[SOFT] = velPid->desiredVel[CMD];
    }
    return velPid->desiredVel[SOFT];
}

/**
 * @brief  速度控制
 * @param  None
 * @retval 速度PID的输出
 */
float VelPidCtrl(VelCtrlType *velPid)
{
    /*****************速度环PID*****************/
    velPid->velErr = velPid->desiredVel[SOFT] - velPid->speed;
    //计算积分
    velPid->iOut += velPid->ki * velPid->velErr;
    //积分限幅
    velPid->iOut = MaxMinLimit(velPid->iOut, velPid->maxOutput);
    //计算输出
    velPid->output = velPid->kp * velPid->velErr + velPid->iOut;
    //输出限幅
    velPid->output = MaxMinLimit(velPid->output, velPid->maxOutput);

    return velPid->output;
}

/**
 * @brief  限制输出幅值
 * @param  val：输入值
 * @retval 输出值
 */
float OutPutLim(float value)
{
    float outputMax, outputMin, outputBasic;
    /********************计算动态最大最小输出****************************/
    outputBasic = Driver[0].velCtrl.speed * EMF_CONSTANT; //估算反电动势
    outputMax = outputBasic + VOL_AMP;                    //输出幅度
    outputMin = outputBasic - VOL_AMP;                    //需要根据速度与电压关系改变
    if (outputMax < VOL_AMP)
        outputMax = VOL_AMP;
    if (outputMin > -VOL_AMP)
        outputMin = -VOL_AMP;

    if (value < outputMin)
        value = outputMin;
    if (value > outputMax)
        value = outputMax;

    if (value > VOL_MAX)
        value = VOL_MAX;
    if (value < -VOL_MAX)
        value = -VOL_MAX;

    if (value < 0)
        value -= VOL_BLIND_AREA;                        //消除控制盲区0.3043f Vq发布给tim4
    else
        value += VOL_BLIND_AREA;

    return value;
}

/**
 * @brief  位置控制(新位置环程序)
 * @param  None
 * @retval 位置环PID的输出。
 */
float PosCtrl(PosCtrlType *posPid)
{
    float posPidOut = 0.0f;
    float desiredVel = 0.0f, signVel = 1.0f;

    /******************************计算位置环输出**************************************/
    posPid->posErr = posPid->desiredPos - posPid->actualPos;
    posPidOut = posPid->posErr * posPid->kp + posPid->kd * (posPid->posErr - posPid->posErrLast);
    posPid->posErrLast = posPid->posErr;

    if (posPid->posErr < 0.0f)
        signVel = -1.0f;

    //乘以0.7是因为减速需要有裕量，有待优化（斜坡问题）
//    desiredVel = signVel * __sqrtf(2.0f * 0.7f * posPid->acc * signVel * posPid->posErr);
    desiredVel = signVel * sqrt(2.0f * 0.7f * posPid->acc * signVel * posPid->posErr);

    if (fabsf(desiredVel) < fabsf(posPidOut))
        posPidOut = desiredVel;
    //给一定大小的死区
    //if(fabsf(posPid->posErr) <= 200.0f)     posPidOut = 0.0f;

    posPid->output = MaxMinLimit(posPidOut, posPid->posVel);

    return posPid->output;
}
/**
 * @brief  Homing mode
 * @param  None
 * @retval 输出的值
 */

void HomingMode(DriverType *driver)
{
    float output;

    driver->velCtrl.desiredVel[SOFT] = driver->homingMode.vel;
    output = VelPidCtrl(&driver->velCtrl);

    driver->homingMode.output = MaxMinLimit(output, driver->homingMode.current); //限制home模式时电流值

    if (fabsf(driver->velCtrl.speed) <= 2)
    {
        driver->homingMode.cnt++;
    }
    else
    {
        driver->homingMode.cnt = 0;
    }

    if (driver->homingMode.cnt >= 500)
    {
        driver->posCtrl.actualPos = 0.0f;
        driver->posCtrl.desiredPos = driver->posCtrl.actualPos + 8192.0f;
        //清除输出
        driver->homingMode.output = 0.0f;
        driver->velCtrl.desiredVel[CMD] = 0.0f;
        driver->velCtrl.desiredVel[SOFT] = 0.0f;
        driver->velCtrl.output = 0.0f;
        driver->output = 0.0f;
        driver->homingMode.output = 0.0f;
        driver->velCtrl.iOut = 0.0f;
        driver->unitMode = POSITION_CONTROL_MODE;
    }
}

/**
 * @brief  传递输出电压
 * @param  None
 * @retval 位置环输出的值
 */
float GetPosPidOut(void)
{
    return Driver[0].posCtrl.output;
}

/**
 * @brief  Calculate Speed
 * @param  None
 * @retval Subtraction number between every two times.
 **/
float CalculateSpeed_Pos(DriverType *driver, MotorType *motor)
{
    int deltaPos = 0;
    deltaPos = (motor->pos - motor->posLast);
    motor->posLast = motor->pos;
    if (deltaPos > (driver->encoder.period / 2))
        deltaPos -= driver->encoder.period;
    if (deltaPos < -(driver->encoder.period / 2))
        deltaPos += driver->encoder.period;

    driver->posCtrl.actualPos += deltaPos;

    //用反馈速度输入
    driver->velCtrl.speed = (float) (motor->vel) * 0.1365333f; //1/60*8192/1000=0.136533
    //用位置差分出的速度输入
    //    driver->velCtrl.speed = speed;

    return driver->velCtrl.speed;
}

#define INC_NUM 0.1365333f * 19.0f // 19.0f/0.1365333f

// 该函数接收输入的是轮子输出轴的转速RPM
void SetSpeed(int n, float rpm)
{
    // 由于3508闭环的时候做了转换因此这里要多*一个0.1365333f
    Driver[n].velCtrl.desiredVel[CMD] = rpm * 2.594132f;
}

void SetPos(int n, float pos)
{
    Driver[n].posCtrl.desiredPos = pos;
}

/**
 * @brief  Get Speed
 * @param  None
 * @retval Speed
 **/
float GetSpeed(void)
{
    return Driver[0].velCtrl.speed;
}

/**
 * @brief  传递输出电压
 * @param  None
 * @retval 得到的值
 */
float GetVelPidOut(void)
{
    return Driver[0].velCtrl.output;
}

/**
 * @brief  max min limit
 * @param  inDat:
 * @retval outDat
 */
float MaxMinLimit(float val, float limit)
{
    if (val > limit)
        val = limit;
    if (val < -limit)
        val = -limit;

    return val;
}

/**
 * @brief  电机使能
 * @param  n:哪个电机  (0-7)
 * @retval None
 */
void MotorOn(int n)
{
    if (Driver[n].unitMode == POSITION_CONTROL_MODE)
        Driver[n].posCtrl.desiredPos = Driver[n].posCtrl.actualPos;

    if (Driver[n].unitMode == SPEED_CONTROL_MODE)
    {
        Driver[n].velCtrl.desiredVel[0] = 0.0f;
        Driver[n].velCtrl.desiredVel[1] = 0.0f;
        Driver[n].velCtrl.desiredVel[2] = 0.0f;
    }
    Driver[n].velCtrl.velErr = 0.0f;
    Driver[n].velCtrl.iOut = 0.0f;
    Driver[n].velCtrl.output = 0.0f;

    Driver[n].output = 0.0f;

    Motor[n].vel = 0.0f;

    Driver[n].status = ENABLE;
}

/**
 * @brief  电机失能
 * @param  n:哪个电机  (0-7)
 * @retval None
 */
void MotorOff(int n)
{
    Driver[n].status = DISABLE;
}

void MotorOffMulti(int n0, int n1, int n2, int n3)
{
    MotorOff(n0);
    MotorOff(n1);
    MotorOff(n2);
    MotorOff(n3);
}

void MotorOnMulti(int n0, int n1, int n2, int n3)
{
    MotorOn(n0);
    MotorOn(n1);
    MotorOn(n2);
    MotorOn(n3);
}

/**
 * @brief 自定义单个电机控制方式
 * @note  放在DriverInit()之后，用于设定单个电机控制方式
 * @param n:            电机ID减1的值
 * @param motor_type:   电机型号: RM_3508, M_2006
 * @param unit_mode:    控制方式: SPEED_CONTROL_MODE, POSITION_CONTROL_MODE
 */
//void CustomMotorInit(int n, int motor_type, int unit_mode);
//{
//
//}

/**
  * @brief          hal库CAN回调函数,接收数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    static CAN_RxPacketTypeDef packet;
#ifdef USING_BOARD_DRIVER
    test2++;
    uint8_t u = 0;
    HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&packet.hdr,packet.data);
    if ((packet.hdr.StdId == 0x201) || (packet.hdr.StdId == 0x202) || (packet.hdr.StdId == 0x203) || (packet.hdr.StdId == 0x204) || //
        (packet.hdr.StdId == 0x205) || (packet.hdr.StdId == 0x206) || (packet.hdr.StdId == 0x207) || (packet.hdr.StdId == 0x208))
    {
        u = packet.hdr.StdId - 0x201;
        Motor[u].pos = (packet.data[0] << 8) + packet.data[1];
        Motor[u].vel = (int16_t) (packet.data[2] << 8) + packet.data[3];
        Motor[u].cur = (packet.data[4] << 8) + packet.data[5];
        Motor[u].temp = packet.data[6];
    }
#endif
}

#endif
