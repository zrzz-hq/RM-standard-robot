/**
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
  * @file    start_task.c
	* @brief   Freertos�����ʼ��           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     11-18-2020      JackyJuu            Done
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
	* @describe Freertos�����ʼ��
*/
#include "Start_Task.h"
#include "User_Task.h"
#include "gui_task.h"
#include "judge_task.h"
#include "IMU_task.h"
#include "shoot_task.h"
#include "Check_Task.h"
#include "usb_task.h"
#include "supercap_task.h"
#include "data_task.h"

#include "gimbal_task.h"
#include "chassis_task.h"


#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#define START_TASK_PRIO 3
#define START_STK_SIZE 256
static TaskHandle_t StartTask_Handler;

void Start_Task(void *pvParameters)
{
		//�ڶ��׶γ�ʼ����������������
	
		create_gui_control_task();
	
		Create_Data_Task();
	
		Create_Judge_Task();
	
		//�����׶γ�ʼ������������
		taskENTER_CRITICAL();
		
	  Create_User_task();
		
		Create_IMU_Task();	
	
		Create_Shoot_Task();			

		Create_Gimbal_Task();	

		Create_Chassis_task();	

		create_supercap_task();
	
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

void Create_Start_Task()
{
		xTaskCreate((TaskFunction_t)Start_Task,
		(const char *)"StartTast",
		(uint16_t)START_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)START_TASK_PRIO,
		(TaskHandle_t *)&StartTask_Handler);
}
