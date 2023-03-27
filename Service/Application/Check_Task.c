/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    Check_Task.c
	* @brief   检查任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 在系统运行过程中不断对数据进行检查
	* @Question 	 需要对电机数据，遥控器数据，裁判系统数据等进行检测【4.9】
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     4-9-2021     	 JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe 检查任务
**/
#include "Check_Task.h"
#include "Judge_Data.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "DJI_Remote_Control.h"

#include "usb_usart.h"

#include "led.h"

void Check_Task(void *pvParameters)
{
	
	while(1)
	{		
		
		vTaskDelay(100);
		
	}
}
