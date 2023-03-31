/**
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
  * @file    user_task.c
	* @brief   用户任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 测试用
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     11-18-2020      JackyJuu            Done
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
	* @describe 用户任务
*/
#include "User_Task.h"
#include "led.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Judge_Graphic.h"
static TaskHandle_t UserTask_Handler;

void Test()
{
		Judge_Graphic_Circle_Create(100,100,10,2);
		Judge_Graphic_Circle_Create(200,200,10,2);
		Judge_Graphic_Circle_Create(300,300,10,2);
		Judge_Graphic_Circle_Create(400,400,10,2);
		Judge_Graphic_Circle_Create(500,500,10,2);
		Judge_Graphic_Circle_Create(600,600,10,2);
}

void UserTask(void *pvParameters)
{
	vTaskDelay(100);
	Test();
	while(1)
	{
		
		LED_GREEN_ON();
		vTaskDelay(500);
		LED_GREEN_OFF();
		vTaskDelay(500);
		
	}
}

 void Create_User_task()
{
	xTaskCreate((TaskFunction_t)UserTask,
		(const char *)"UserTast",
		(uint16_t)USER_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)USER_TASK_PRIO,
		(TaskHandle_t *)&UserTask_Handler);
}