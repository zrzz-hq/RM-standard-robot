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

extern DJI_Judge_t DJI_Judge; 

int Super_C_Set;

uint8_t PC_Send_Data[10];

typedef struct 
{
	float curr_yaw;
	float curr_pitch;
	uint8_t state;
	uint8_t mark;
	uint8_t anti_top;
	uint8_t enemy_color;
	int delta_x;
	int delta_y;	
}PC_Data_t;

PC_Data_t PC_Data;
uint8_t PC_Send_Data_Set[60];
uint8_t PC_Vision_Mode = 'A';//自瞄A，大幅B，小幅S
uint8_t PC_Vision_Anti_Top = 0;
uint8_t Vision_Mark = 0;
uint8_t Vision_Enemy_Color = 1;

void PC_Data_Send(uint8_t state_set,uint8_t mark_set,uint8_t anti_top_set,uint8_t enemy_color_set)
{


	
	PC_Data.state = state_set;
	PC_Data.state = mark_set;
	PC_Data.state = anti_top_set;
	PC_Data.state = enemy_color_set;
	
	
	
	memcpy(PC_Send_Data_Set,&PC_Data,sizeof(PC_Data));
	
	for(int i = 0;i < sizeof(PC_Send_Data_Set);i++)
	{
		USART3_SendData(PC_Send_Data_Set[i]);
	}
//	
//	PC_Send_Data[0] = 0;
//	PC_Send_Data[1] = 0;
//	PC_Send_Data[2] = 0;
//	PC_Send_Data[3] = 0;
//	PC_Send_Data[4] = state;
//	PC_Send_Data[5] = mark;
//	PC_Send_Data[6] = anti_top;
//	PC_Send_Data[7] = enemy_color;
//	PC_Send_Data[8] = 0;
//	PC_Send_Data[9] = 0;
	
}

void Check_Task(void *pvParameters)
{
	
	while(1)
	{		
//			Super_C_Set = DJI_Judge.DJI_Judge_Mes.Judge_game_robot_status.max_chassis_power * 100;
//			PC_Data_Send(PC_Vision_Mode,0,0,Vision_Enemy_Color);
//			CAN1_Motor_Control(0x210,(int16_t)Super_C_Set,0,0,0);
		
			
		
		
//		LED_RED_ON();
//		vTaskDelay(500);
//		LED_RED_OFF();
		vTaskDelay(100);
		
	}
}
