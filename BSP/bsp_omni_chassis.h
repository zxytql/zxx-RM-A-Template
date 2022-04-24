#ifndef ZXX_RM_A_TEMPLATE_BSP_OMNI_CHASSIS_H
#define ZXX_RM_A_TEMPLATE_BSP_OMNI_CHASSIS_H

#define COS_30 0.866025f
#define SIN_30 0.5f
#define COS_45 0.707107f
#define SIN_45 0.707107f
#define WHEEL3_TO_CENT 0.36f /** meters **/
#define WHEEL4_TO_CENT 0.36f /** meters **/
#define WHEEL_RADIUS  0.08f /** meters **/
#define SHAFT_RPM_TO_WHEEL (60 / (2 * 3.14 * WHEEL_RADIUS)) /** TRANS COEFFICIENT**/

/******** Struct ********/
typedef struct
{
    /** 底盘形心速度 m/s **/
    float ct_vx;
    float ct_vy;
    float ct_wz;
}chassis_ct_t;

typedef struct
{
    /** 反馈速度 m/s **/
    float ct_fdb_vx;
    float ct_fdv_vy;
    float ct_fdb_wz;
}chassis_ct_fdb_t;

typedef struct
{
    /** 遥控器键值与底盘运动比例系数 **/
    float vx_co;
    float vy_co;
    float wz_co;
}chassis_rc_co_t;

typedef struct
{
    chassis_ct_t chassis_ct;
    chassis_ct_fdb_t chassis_ct_fdb;
    chassis_rc_co_t chassis_rc_co;

    float wheel_speed[4]; /** 输出轴速度 m/s **/
    float wheel_fdb_speed[4]; /** 反馈转速 m/s **/
}chassis_t;

/******** Extern ********/
extern chassis_t chassis;

/******** Function ********/
void kinematics_inverse_omni3(chassis_t *ptr);
void kinematics_positive_omni3(chassis_t *chassis);

void kinematics_inverse_omni4(chassis_t *ptr);
void kinematics_positive_omni4(chassis_t *chassis);
#endif //ZXX_RM_A_TEMPLATE_BSP_OMNI_CHASSIS_H
