/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    judge_task.c
	* @brief   裁判系统任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 定期发送
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     12-4-2020      JackyJuu            Done
  *  V1.2.0      4-2-2020      JackyJuu            Done	
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe OLED显示屏任务
*/

#include "judge_task.h"

#include "chassis_task.h"

#include "Judge_Data.h"
#include "Judge_Graphic.h"
#include "DJI_Remote_Control.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

static TaskHandle_t JudgeTask_Handler;	


void Judge_Task(void *pvParameters)
{
	Judge_Data_Init();
	Judge_Graphic_Init();
	while(1)
	{
			Judge_Graphic_Handler();
			vTaskDelay(100);
	}
}

void Create_Judge_Task()
{
		xTaskCreate((TaskFunction_t)Judge_Task,
		(const char *)"JudgeTast",
		(uint16_t)JUDGE_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)JUDGE_TASK_PRIO,
		(TaskHandle_t *)&JudgeTask_Handler);
}