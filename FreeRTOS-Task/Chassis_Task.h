#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H

#include "bsp_omni_chassis.h"
#include "cmsis_os.h"
#include "RC_Task.h"

/******** Function ********/
void Chassis_Param_Init(chassis_t *);
void Chassis_action(chassis_t *, rc_keyValue_t *);
#endif
