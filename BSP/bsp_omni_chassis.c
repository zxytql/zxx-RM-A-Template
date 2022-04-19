/**
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  * @file       bsp_omni_chassis.c/h
  * @version    V1.0.0
  * @brief      全向轮底盘支持包
  * @note       1. RM_3508输出轴减速比约为 19.2 : 1
  *             2. RM_2006输出轴减速比约为   36 : 1
  * @history
  *  Version        Date            Author          Modification
  *  V1.0.0     2022-04-12          zxy            First version
  ****************************(C) COPYRIGHT 2022 HCRT****************************
  */

#include "bsp_omni_chassis.h"
#include "DJI_Motor_Ctrl.h"
/******** Global Variables ********/
chassis_t chassis;

/**
 * @brief 三轮运动学逆解 由给定形心速度计算输出轴转速
 * @param ptr: 底盘运动结构体
 */
void kinematics_inverse_omni3(chassis_t *ptr)
{
    if (ptr == NULL) { return; }
    ptr->wheel_speed[0] = -ptr->chassis_ct.ct_vx + WHEEL3_TO_CENT * ptr->chassis_ct.ct_wz;
    ptr->wheel_speed[1] = ptr->chassis_ct.ct_vx * SIN_30 - ptr->chassis_ct.ct_vy * COS_30 +
                          WHEEL3_TO_CENT * ptr->chassis_ct.ct_wz;
    ptr->wheel_speed[2] = ptr->chassis_ct.ct_vx * SIN_30 + ptr->chassis_ct.ct_vy * COS_30 +
                          WHEEL3_TO_CENT * ptr->chassis_ct.ct_wz;

    for (int i = 0; i < 3; ++i) {
        ptr->wheel_speed[i] *= SHAFT_RPM_TO_WHEEL;
    }
}

/**
 * @brief 三轮运动学正解 由实际输出轴转速反算形心速度
 * @param ptr: 底盘运动结构体
 * ans =
 *
[Vx]    [-0.6667     0.3333     0.3333]  [V1]
[Vy] =  [ 0.0000    -0.5774     0.5774]  [V2]
[Wz]    [ 0.3333L    0.3333L   0.3333L]  [V3]
 *
 */
void kinematics_positive_omni3(chassis_t *ptr)
{
    if (ptr == NULL) { return; }
    /** Motor->vel返回的是转子转速 需要经过减速比换算成轴转速 **/
    for (int i = 0; i < 3; ++i) {
        ptr->wheel_fdb_speed[i] = Motor[i].vel / 19.2f;
    }

    ptr->chassis_ct_fdb.ct_fdb_vx = -0.6667f * ptr->wheel_fdb_speed[0] + 0.3333f * ptr->wheel_fdb_speed[1] +
            0.3333f * ptr->wheel_fdb_speed[2];
    ptr->chassis_ct_fdb.ct_fdv_vy = -0.5774f * ptr->wheel_fdb_speed[1] + 0.5774f * ptr->wheel_fdb_speed[2];
    ptr->chassis_ct_fdb.ct_fdb_wz = 0.3333f * WHEEL3_TO_CENT * (ptr->wheel_fdb_speed[0] + ptr->wheel_fdb_speed[1] +
            ptr->wheel_fdb_speed[2]);
}
/**
 * @brief 四轮运动学逆解 由给定形心速度计算输出轴转速
 * @param ptr: 底盘运动结构体
 */
void kinematics_inverse_omni4(chassis_t *ptr)
{
    if (ptr == NULL) { return; }
    ptr->wheel_speed[0] = -ptr->chassis_ct.ct_vx * COS_45 - ptr->chassis_ct.ct_vy * COS_45 +
                          WHEEL4_TO_CENT * ptr->chassis_ct.ct_wz;
    ptr->wheel_speed[1] = ptr->chassis_ct.ct_vx * COS_45 - ptr->chassis_ct.ct_vy * COS_45 +
                          WHEEL4_TO_CENT * ptr->chassis_ct.ct_wz;
    ptr->wheel_speed[2] = ptr->chassis_ct.ct_vx * COS_45 + ptr->chassis_ct.ct_vy * COS_45 +
                          WHEEL4_TO_CENT * ptr->chassis_ct.ct_wz;
    ptr->wheel_speed[3] = -ptr->chassis_ct.ct_vx * COS_45 + ptr->chassis_ct.ct_vy * COS_45 +
                          WHEEL4_TO_CENT * ptr->chassis_ct.ct_wz;

    for (int i = 0; i < 4; ++i) {
        ptr->wheel_speed[i] *= SHAFT_RPM_TO_WHEEL;
    }
}

/**
 * @brief 四轮运动学正解 由实际输出轴转速反算形心速度
 * @param ptr: 底盘运动结构体
 * ans =
 *
[Vx]   [-0.3536      0.3536    0.3536     -0.3536]
[Vy] = [-0.3536     -0.3536    0.3536      0.3536]  [V1 V2 V3 V4]T
[Wz]   [ 0.2500L    0.2500L    0.2500L    0.2500L]
 *
 */
void kinematics_positive_omni4(chassis_t *ptr)
{
    if (ptr == NULL) { return; }
    /** Motor->vel返回的是转子转速 需要经过减速比换算成轴转速 **/
    for (int i = 0; i < 4; ++i) {
        ptr->wheel_fdb_speed[i] = Motor[i].vel / 19.2f;
    }

    ptr->chassis_ct_fdb.ct_fdb_vx = -0.3536f * (ptr->wheel_fdb_speed[0] + ptr->wheel_fdb_speed[3]) +
            0.3536f * (ptr->wheel_fdb_speed[1] + ptr->wheel_fdb_speed[2]);
    ptr->chassis_ct_fdb.ct_fdv_vy = -0.3536f * (ptr->wheel_fdb_speed[0] + ptr->wheel_fdb_speed[1]) +
            0.3536f * (ptr->wheel_fdb_speed[2] + ptr->wheel_fdb_speed[3]);
    ptr->chassis_ct_fdb.ct_fdb_wz = 0.25f * WHEEL4_TO_CENT *
            (ptr->wheel_fdb_speed[0] + ptr->wheel_fdb_speed[1] + ptr->wheel_fdb_speed[2] + ptr->wheel_fdb_speed[3]);
}