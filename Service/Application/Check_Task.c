/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    Check_Task.c
	* @brief   �������           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 ��ϵͳ���й����в��϶����ݽ��м��
	* @Question 	 ��Ҫ�Ե�����ݣ�ң�������ݣ�����ϵͳ���ݵȽ��м�⡾4.9��
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     4-9-2021     	 JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe �������
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
