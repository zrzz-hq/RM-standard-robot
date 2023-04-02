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
Judge_Graphic_Obj_t* Circle1;

void Test()
{
		Circle1=Judge_Graphic_Circle_Create(600,600,50,10);
}

uint16_t x = 0;
uint16_t y = 0;

void UserTask(void *pvParameters)
{
	vTaskDelay(100);
	Test();
	while(1)
	{
		Judge_Graphic_Obj_Set_Center_X(Circle1,x++);
		Judge_Graphic_Obj_Set_Center_Y(Circle1,y++);
		if(x==2000)
			x = 0;
		if(y==2000)
			y = 0;
		//Judge_Graphic_Obj_Set_Height(Circle1,300);
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