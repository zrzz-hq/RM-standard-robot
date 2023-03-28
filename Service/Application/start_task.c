/**
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
  * @file    start_task.c
	* @brief   Freertos任务初始化           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     11-18-2020      JackyJuu            Done
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
	* @describe Freertos任务初始化
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

#include "gimbal_task.h"
#include "chassis_task.h"


#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#define START_TASK_PRIO 3
#define START_STK_SIZE 256
static TaskHandle_t StartTask_Handler;

#define USER_TASK_PRIO 5
#define USER_STK_SIZE 128
static TaskHandle_t UserTask_Handler;

//#define OSKER_TASK_PRIO 6
//#define OSKER_STK_SIZE 512
//static TaskHandle_t OskerTask_Handler;

#define JUDGE_TASK_PRIO 10
#define JUDGE_STK_SIZE 512
static TaskHandle_t JudgeTask_Handler;

#define IMU_TASK_PRIO 20
#define IMU_STK_SIZE 512
static TaskHandle_t IMUTask_Handler;

#define SHOOT_TASK_PRIO 17
#define SHOOT_STK_SIZE 512
static TaskHandle_t ShootTask_Handler;

#define OLED_LIST_TASK_PRIO 18
#define OLED_LIST_STK_SIZE 256
static TaskHandle_t OledTask_Handler;

#define GIMBAL_TASK_PRIO 19
#define GIMBAL_STK_SIZE 512
static TaskHandle_t Gimbaldask_Handler;

#define CHASSIS_TASK_PRIO 15
#define CHASSIS_STK_SIZE 512
static TaskHandle_t ChassisTask_Handler;

#define USB_TASK_PRIO 14
#define USB_STK_SIZE 512
static TaskHandle_t UsbTask_Handler;

#define CHECK_TASK_PRIO 13
#define CHECK_STK_SIZE 512
static TaskHandle_t CheckTask_Handler;

void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();
		
		xTaskCreate((TaskFunction_t)UserTask,
		(const char *)"UserTast",
		(uint16_t)USER_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)USER_TASK_PRIO,
		(TaskHandle_t *)&UserTask_Handler);
		
		xTaskCreate((TaskFunction_t)IMU_Task,
	  (const char *)"IMU_Task",
		(uint16_t)IMU_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)IMU_TASK_PRIO,
		(TaskHandle_t *)&IMUTask_Handler);		
	
		xTaskCreate((TaskFunction_t)Shoot_Task,
	  (const char *)"Shoot_Task",
		(uint16_t)SHOOT_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)SHOOT_TASK_PRIO,
		(TaskHandle_t *)&ShootTask_Handler);			

		xTaskCreate((TaskFunction_t)Gimbal_Task,
		(const char *)"Gimbal_Task",
		(uint16_t)GIMBAL_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)GIMBAL_TASK_PRIO,
		(TaskHandle_t *)&Gimbaldask_Handler);		
	
		xTaskCreate((TaskFunction_t)Chassis_Task,
	  (const char *)"Chassis_Task",
	  (uint16_t)CHASSIS_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)CHASSIS_TASK_PRIO,
		(TaskHandle_t *)&Gimbaldask_Handler);	

		xTaskCreate((TaskFunction_t)supercap_task,
	  (const char *)"SuperCap_Task",
	  (uint16_t)SUPERCAP_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)SUPERCAP_TASK_PRIO,
		(TaskHandle_t *)&SuperCapTask_Handler);	
	
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
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
