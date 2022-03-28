/**
  ****************************(C) COPYRIGHT 2020 HCRT****************************
  * @file       hdc_driver.c/h
  * @brief      与大疆电机伺服控制器的通信程序
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2020.2.1    	 	 YLT                V1.0 
  *  V1.0.1     2020.8.22    	 	 YLT                V1.0.1 
  *  V1.0.2     2020.8.27    	 	 YLT                V1.0.2 

  @verbatim
  ==============================================================================
  * wiki|使用方式
  * 1.将 hdc_driver.c/h 包含在工程中，在需要使用的地方 #include "hdc_driver.h"
	* 直接调用函数，如发送速度使用 VelCrl(&hcan1,i+1,set_spd[i]*100);
	* 2.实现基本功能 更新wiki 使用说明
	
	* update 2020.8.27 :
  * 1.增加 while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	* 	修复第四个电机报文发送不出去的问题

	* update 2020.8.22 :
  * 1.修改文件名为hdc_driver.c/h 其命名含义为 HCRT DjiMotor Controller Driver
	* 	即湖工大竞技机器人队 大疆电机控制器 驱动程序
	* 2.实现基本功能 更新wiki 使用说明

	* update 2020.2.1 :
  * 1.更改为最新的HAL库函数，移植了之前缺失的一些函数
  * 2.实现基本功能
  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 HCRT****************************
  */


#include "hdc_driver.h"
#include "main.h"
#ifdef USING_HDC_DRIVER
/**
* @brief  电机使能
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @note   驱动器默认初始状态为电机失能，使用电机时需要对其进行使能
*         部分驱动器参数需要在电机失能状态下才可以配置
*/
void MotorOn(CAN_HandleTypeDef *CANx, uint8_t ElmoNum)
{
	//第一个数发送MO命令，第二个数发送1给电机使能（通电）
	uint32_t data[1][2] = {
		0x00004F4D, 0x00000001, // MO  1
	};

	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.ExtId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;					  
	TxHeader.RTR = CAN_RTR_DATA;				   
	TxHeader.DLC = 8;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;

	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}

/**
* @brief  电机失能（断电）
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
*/
void MotorOff(CAN_HandleTypeDef *CANx, uint8_t ElmoNum)
{
	//第一个数据发送MO命令，第二个数据发送0给电机失能（断电）
	uint32_t data[1][2] = {
		0x00004F4D, 0x00000000, // MO  0
	};

	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.ExtId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;					   
	TxHeader.RTR = CAN_RTR_DATA;				   
	TxHeader.DLC = 8;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;

	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}

/**
* @brief  驱动器速度环初始化
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  acc：加速度，单位：脉冲每二次方秒
* @param  dec：减速度，单位：脉冲每二次方秒
* @note   在速度环初始化后才可以使能电机！！
*/
void VelLoopCfg(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, uint32_t acc, uint32_t dec)
{
	SetUnitMode(CANx, ElmoNum, SPEED_CONTROL_MODE);

	SetAccAndDec(CANx, ElmoNum, acc, dec);
}

/**
* @brief  配置加速度与减速度
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  acc：加速度，单位：脉冲每二次方秒
* @param  dec：减速度，单位：脉冲每二次方秒
*/
void SetAccAndDec(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, uint32_t acc, uint32_t dec)
{
	//第一个数据发送AC\DC命令，第二个数据发送命令值
	uint32_t data[2][2] = {
		0x00004341, 0x00000000, //AC
		0x00004344, 0x00000000	//DC
	};

	data[0][1] = acc;
	data[1][1] = dec;

	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.ExtId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 8;
	for (uint8_t i = 0; i < 2; i++)
	{
		TxMessage[0] = *(unsigned long *)&data[i][0] & 0xff;
		TxMessage[1] = (*(unsigned long *)&data[i][0] >> 8) & 0xff;
		TxMessage[2] = (*(unsigned long *)&data[i][0] >> 16) & 0xff;
		TxMessage[3] = (*(unsigned long *)&data[i][0] >> 24) & 0xff;
		TxMessage[4] = *(unsigned long *)&data[i][1] & 0xff;
		TxMessage[5] = (*(unsigned long *)&data[i][1] >> 8) & 0xff;
		TxMessage[6] = (*(unsigned long *)&data[i][1] >> 16) & 0xff;
		TxMessage[7] = (*(unsigned long *)&data[i][1] >> 24) & 0xff;
		
		while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
		HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
	}
}

/**
* @brief  驱动器位置环初始化
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  acc：加速度，单位：脉冲每二次方秒
* @param  dec：减速度，单位：脉冲每二次方秒
* @param  vel: 速度，单位：脉冲每秒，范围：最小速度限制到最大速度限制
* @note   在位置环初始化后才可以使能电机！！
*/
void PosLoopCfg(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, uint32_t acc, uint32_t dec, uint32_t vel)
{
	SetUnitMode(CANx, ElmoNum, POSITION_CONTROL_MODE);

	SetAccAndDec(CANx, ElmoNum, acc, dec);

	SetPosLoopVel(CANx, ElmoNum, vel);
}

/**
* @brief  配置位置环运行最大速度
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  vel: 速度，单位：脉冲每秒，范围：最小速度限制到最大速度限制
* @note： 速度正负号代表旋转的方向，大于零为正方向，小于零为负方向
*/
void SetPosLoopVel(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, int32_t vel)
{
	//第一个数据发送SP命令，第二个数据发送命令值
	uint32_t data[1][2] = {
		0x00005053, 0x00000000, // SP 
	};

	data[0][1] = vel;

	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.ExtId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;					  
	TxHeader.RTR = CAN_RTR_DATA;				  
	TxHeader.DLC = 8;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;

	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}

/**
* @brief  电机速度控制
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  vel: 速度，单位：脉冲每秒，范围：最小速度限制到最大速度限制
*/
void VelCrl(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, int32_t vel)
{
	SetJoggingVel(CANx, ElmoNum, vel);
}

/**
* @brief  电机位置控制
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  posMode: 位置环运行模式，范围：
				ABSOLUTE_MODE: 绝对位置模式
				RELATIVE_MODE: 相对位置模式
* @param  pos:位置命令，单位：脉冲，范围：最大位置限制到最小位置限制
*/
void PosCrl(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, uint8_t posMode, int32_t pos)
{
	SendPosCmd(CANx, ElmoNum, posMode, pos);
}

/**
* @brief  配置驱动器工作模式
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  unitMode：驱动器工作模式，范围：
			TORQUE_CONTROL_MODE：力矩控制模式，在该模式下可以执行TC电流命令
			SPEED_CONTROL_MODE：速度控制模式，在该模式下通过设置JV值控制速度
			MICRO_STEPPER_MODE：直流电机不能使用该模式
			DUAL_POSITION_MODE：双位置闭环模式
			SINGLE_POSITION_MODE：单位置闭环模式，在该模式下可以配置PA、PR、JV、PT或PVT运动
* @note   只有在电机失能时可以配置该参数
*/
void SetUnitMode(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, uint8_t unitMode)
{
	//第一个数据发送UM命令，第二个数据发送模式
	uint32_t data[1][2] = {
		0x00004D55, 0x00000000, // UM
	};

	data[0][1] = unitMode;

	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 0x08;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;

	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}

/**
* @brief  配置运行速度
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  vel: 速度，单位：脉冲每秒，范围：最小速度限制到最大速度限制
* @note   速度正负号代表旋转的方向，大于零为正方向，小于零为负方向
*/
void SetJoggingVel(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, int32_t vel)
{
	//第一个数据发送JV命令，第二个数据发送命令值
	uint32_t data[1][2] = {
		0x0000564A, 0x00000000, //JV
	};

	data[0][1] = vel;

	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 0x08;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;

	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}

/**
* @brief  配置位置环命令
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @param  posMode: 位置环运行模式，范围：
				ABSOLUTE_MODE: 绝对位置模式
				RELATIVE_MODE: 相对位置模式
* @param  pos:位置命令，单位：脉冲，范围：最大位置限制到最小位置限制
* @note   位置正负号代表旋转的方向，大于零为正方向，小于零为负方向
*/
void SendPosCmd(CAN_HandleTypeDef *CANx, uint8_t ElmoNum, uint8_t posMode, int32_t pos)
{

	uint32_t data[1][2] = {
		0x00000000, 0x00000000, //PA
	};

	if (posMode == ABSOLUTE_MODE)
	{
		data[0][0] = 0x00004150; //绝对
	}

	data[0][1] = pos;

	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 0x08;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;
	
	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}

/**********************************读取驱动器数据命令*************************************/

/**
* @brief  读取电机位置
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @note	  接收标识符为：0x00005850
*/
void ReadActualPos(CAN_HandleTypeDef *CANx, uint8_t ElmoNum)
{
	uint32_t data[1][2] = {
		0x40005850, 0x00000000, //PX
	};
	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum; 
	TxHeader.ExtId = ELMO_DEVICE_BASEID + ElmoNum; 
	TxHeader.IDE = CAN_ID_STD;					   
	TxHeader.RTR = CAN_RTR_DATA;				   
	TxHeader.DLC = 8;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;

	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}

/**
* @brief  读取电机速度
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @note   接收标识符为：0x00005856
*/
void ReadActualVel(CAN_HandleTypeDef *CANx, uint8_t ElmoNum)
{
	uint32_t data[1][2] = {
		0x40005856, 0x00000000, //VX
	};
	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.ExtId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;					  
	TxHeader.RTR = CAN_RTR_DATA;				  
	TxHeader.DLC = 8;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;

	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}

/**
* @brief  读取电机电流
* @param  CANx：所使用的CAN通道编号
* @param  ElmoNum：驱动器ID号，范围：0~128，0为广播用ID号
* @note   接收标识符为：0x00005149
*/
void ReadActualCurrent(CAN_HandleTypeDef *CANx, uint8_t ElmoNum)
{
	uint32_t data[1][2] = {
		0x00005149, 0x00000000, //IQ
	};
	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxMessage[8];

	TxHeader.StdId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.ExtId = ELMO_DEVICE_BASEID + ElmoNum;
	TxHeader.IDE = CAN_ID_STD;					  
	TxHeader.RTR = CAN_RTR_DATA;				  
	TxHeader.DLC = 8;
	TxMessage[0] = *(unsigned long *)&data[0][0] & 0xff;
	TxMessage[1] = (*(unsigned long *)&data[0][0] >> 8) & 0xff;
	TxMessage[2] = (*(unsigned long *)&data[0][0] >> 16) & 0xff;
	TxMessage[3] = (*(unsigned long *)&data[0][0] >> 24) & 0xff;
	TxMessage[4] = *(unsigned long *)&data[0][1] & 0xff;
	TxMessage[5] = (*(unsigned long *)&data[0][1] >> 8) & 0xff;
	TxMessage[6] = (*(unsigned long *)&data[0][1] >> 16) & 0xff;
	TxMessage[7] = (*(unsigned long *)&data[0][1] >> 24) & 0xff;

	while( HAL_CAN_GetTxMailboxesFreeLevel( CANx ) == 0 );
	HAL_CAN_AddTxMessage(CANx, &TxHeader, TxMessage, &TxMailbox);
}
#endif
