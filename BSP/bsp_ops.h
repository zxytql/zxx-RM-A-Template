#ifndef BSP_OPS_H
#define BSP_OPS_H

#include "main.h"
#define OPS_HEAD_H 0X0D
#define OPS_HEAD_L 0X0A
#define OPS_END_H 0X0A
#define OPS_END_L 0X0D

typedef union
{
    uint8_t data[24];
    float val[6];
}ops_data_t;

/** 数据包内容 **/
typedef enum
{
    Yaw = 0,
    Pitch,
    Roll,
    Position_x,
    Position_y,
    Yaw_speed
}ops_value_e;

/******** Extern ********/
extern ops_data_t ops_data;

/******** Function ********/
void Ops_Frame_Parse(ops_data_t *ops, uint8_t *buf);
void Ops_Calibration(void);
void Ops_Set_All(float yaw, float pos_x, float pos_y);
void Ops_Set_Pos(float pos_x,float pos_y);
void Ops_Set_X(float pos_x);
void Ops_Set_Y(float pos_y);
void Ops_Set_Yaw(float yaw);
void Ops_Waiting(void);
#endif
