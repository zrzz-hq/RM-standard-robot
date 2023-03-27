/**
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
  * @file    main.c
	* @brief   stm32初始化以及开始任务freeRTOS。h文件定义相关全局宏定义            
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     11-18-2020      JackyJuu            Done
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
	* @describe 程序启动入口
*/
#include "main.h"
#include "delay.h"
#include "flash.h"
#include "power.h"

#include "usart_judge.h"
#include "data_task.h"
#include "gui_task.h"

#include "imu_bsp.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Start_Task.h"
#include "usb_usart.h"
#include "PC_usart.h"

#include "DJI_Remote_Control.h"
int main(void)
{ 
 	delay_init(configTICK_RATE_HZ);
	delay_ms(10);
	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
	
	//__disable_irq();
	//Usb_Init();	
	Led_Init();
	
	remote_control_init();
	PC_Usart_Init();
	DJI_Judge_Usart_Init();
	
	ADC1_Init();
	Power_24V_Init();

	USART3_Init();
	USART6_Init();
	UART7_Init();
	UART8_Init();
	
	TIM1_Init();
	TIM2_Init();
	TIM4_Init();
	TIM5_Init();
	TIM8_Init();
	
	CAN1_Init();
	CAN2_Init();
	
	Usr_Key_Init();
	Shoot_Key_Init();
	
	Laser_Configuration();
	
	buzzer_init(30000, 90);
	
	GPIO_SPI_Init();
	SPI1_Init();
	SD_LowLevel_Init();
	//__enable_irq();
	
	
	Create_Data_Task();
	vTaskStartScheduler();//开启任务调度
	while(1)             
	{


	}
}	

void sys_assert_fault_handler()
{
		gui_sys_assert_fault_handler();
}
