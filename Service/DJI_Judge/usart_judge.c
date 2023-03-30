/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    usart_judge.c
	* @brief   DJI裁判系统串口数据           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 利用DMA双缓冲完成，不需要重启DMA通道
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     12-2-2020      JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe DJI裁判系统串口数据
*/
#include "usart_judge.h"
#include "Judge_Data.h"


//裁判系统数据缓冲区;
