#ifndef _JUDGE_TASK_H_
#define _JUDGE_TASK_H_

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "gimbal_task.h"
#include "chassis_task.h"
#include "Judge_Graphic.h"

#define JUDGE_TASK_PRIO 31
#define JUDGE_STK_SIZE 512

void Create_Judge_Task();

#endif

