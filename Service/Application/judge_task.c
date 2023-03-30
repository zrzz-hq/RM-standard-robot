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
#include "DJI_Remote_Control.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

void Judge_Task(void *pvParameters)
{
	vTaskDelay(500);
	
	while(1)
	{
			
			vTaskDelay(Judge_Set_Update_Time_Set);
	}
}