#ifndef RC_TASK_H
#define RC_TASK_H

#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "bsp_omni_chassis.h"

#define RC_DEADZONE 10
#define RC_VALUE_OFFSET 1024

#define RC_DEAD_LIMIT(X)                            (X > -RC_DEADZONE && X < RC_DEADZONE) ? 0 : X
#define RC_KEY_VALUE_SWITCH(X, MID_VALUE)           (X > MID_VALUE) ? 1 : 0
#define RC_KEY_VALUE_SWITCH3(X, VALUE1, VALUE2)     (X > VALUE1) ? ((X > VALUE1 && X < VALUE2) ? 1 : 2) : 0
#define RC_KNOB_TRANS(X, MINVALUE, MAXVALUE)        (X < MINVALUE || X > MAXVALUE) \
                                                    ? 0 : (X - MINVALUE / (MAXVALUE - MINVALUE))
/******** Struct ********/
typedef enum
{
    Offline = 0,
    Online
}rc_state_e;

typedef enum
{
    Control_state = 0,
    Navigation_state
}rc_ctrl_state_e;

typedef struct
{
    /** KEY VALUE **/
    uint8_t key_a;
    uint8_t key_b;
    uint8_t key_c;
    uint8_t key_d;
    uint8_t key_f;

    /** STICK VALUE **/
    int16_t left_vir_roll;
    int16_t left_horz_roll;
    int16_t right_vir_roll;
    int16_t right_horz_roll;

    /** KNOB VALUE **/
    int16_t left_knob;
    int16_t right_knob;
}rc_keyValue_t;

typedef struct
{
    _Bool key_a_flag;
    _Bool key_b_flag;
    _Bool key_d_flag;
    _Bool key_f_flag;

    _Bool key_a_now;
    _Bool key_b_now;
    _Bool key_d_now;
    _Bool key_f_now;
}rc_key_event_t;

typedef struct
{
    rc_state_e rc_state;
    rc_ctrl_state_e rc_ctrl_state;
    rc_keyValue_t rc_KeyValue;
    rc_key_event_t rc_key_event;

    _Bool online;        /** 存储遥控器是否在线 **/
    uint8_t power_state; /** 储存按钮D控制的板载电源状态 **/
    uint8_t state;       /** 存储遥控器控制状态 **/
    int16_t test;        /** 测试用 **/
}rc_t;
/******** Extern ********/
extern rc_t rc;

/******** Function ********/
void Rc_Update(rc_keyValue_t *value, uint8_t buff[25]);
void Rc_Key_Handler(rc_keyValue_t *value, rc_key_event_t *event);
void Rc_Key_Callback_Handler(rc_key_event_t *event);
void Rc_Key_a_Callback();
void Rc_Key_b_Callback();
void Rc_Key_d_Callback();

#endif //RC_TASK_H
