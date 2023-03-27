/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    usb_task.c
	* @brief   USB通讯任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 负责USB虚拟串口的信息交互
	* @Question 	 需要通过USB虚拟串口获取上位机发送的信息进行解析
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     4-19-2021     	 JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe USB通讯任务
**/
#include "usb_task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "App_Set.h"
#include "usb_usart.h"

extern float vision_x_add;//2.84201
extern float vision_y_add;//-5.08175
extern float vision_x_fliter[5];
extern float vision_y_fliter[5];

extern int rexflag;
extern uint8_t angle_get_buf[16];
extern float yaw_angle,pitch_angle;
extern float yaw_angle_change,pitch_angle_change;
extern float yaw_angle_get,pitch_angle_get;



int Check_Motionless_Flag;
int Check_Motionless_Add;
int Check_Motionless_Time;
float PID_Sum_of_Error_Clean[5];
uint8_t USB_Data_Rx[128];



void USB_Task(void *pvParameters)
{
//	Check_Motionless_Time = Motionless_Time_Set / 1000;
	
	while(1)
	{
		
		Usb_Read_Data(USB_Data_Rx,12);

		if((USB_Data_Rx[0] == 0xAA) && (USB_Data_Rx[8] == 0xBB))
		{
			yaw_angle = (float)((USB_Data_Rx[3]) | (USB_Data_Rx[4]<<8));
			pitch_angle =(float)((USB_Data_Rx[1] ) | (USB_Data_Rx[2]<<8));
//			Armor_Long =(float)((USB_Data_Rx[5] ) | (USB_Data_Rx[6]<<8));
//			Vision_Get_Flag = (int)USB_Data_Rx[7];
		}
		
		yaw_angle_change = -(yaw_angle * 90 / 32767);
		pitch_angle_change = -(pitch_angle * 90 / 32767);
//		Armor_Long_Get = -(Armor_Long * 90 / 32767);
		
		if(yaw_angle_change < -90)
		{
			yaw_angle_get = yaw_angle_change + 180 + vision_x_add;

		}
		else if(yaw_angle_change > -90 && yaw_angle_change<0)
		{
			yaw_angle_get = yaw_angle_change + vision_x_add;
			
		}
		else if(yaw_angle_change == 0)
		{
			yaw_angle_get = 0;
		}						

		if(pitch_angle_change < -90)
		{
			pitch_angle_get = pitch_angle_change + 180 + vision_y_add;							
		}
		else if(pitch_angle_change > -90 && pitch_angle_change<0)
		{
		 pitch_angle_get = pitch_angle_change + vision_y_add;
		}
		else if(pitch_angle_change == 0)
		{
		pitch_angle_get = 0;
		}
		
		vTaskDelay(50);
	}
}

