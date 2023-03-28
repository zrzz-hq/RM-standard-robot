#ifndef __CAN_H__
#define __CAN_H__

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

typedef enum
{
	CAN1_RX = 0,
	CAN2_RX = 1
}CAN_Set_t;

#define M2006_Speed_Change 0.0004629630
#define Fric_Motor_Speed_Change 0.0031415926
//转速 线速度

//电容相关信息
typedef struct 
{
	int16_t Cap_I;//电容电流
	int16_t Cap_V;//电容电压
	uint16_t Cap_State;//电容状态
}Super_C_Msg_t;

typedef struct 
{
	int16_t Angle;//机械角度
	int16_t Speed;//速度
	int16_t Current;
	int16_t Temp;
}Motor_Msg_t;

void CAN1_Init(void);
void CAN2_Init(void);

void CAN1_Send(int16_t Stdid,uint8_t * Data);
void CAN2_Send(int16_t Stdid,uint8_t * Data);

void CAN1_Motor_Control(int16_t Stdid,uint16_t Num1,uint16_t Num2,uint16_t Num3,uint16_t Num4);
void CAN2_Motor_Control(int16_t Stdid,uint16_t Num1,uint16_t Num2,uint16_t Num3,uint16_t Num4);

void CAN1_Msg_Parser(CanRxMsg *Can_Rx_Msg);
void CAN2_Msg_Parser(CanRxMsg *Can_Rx_Msg);

void CAN2_SuperCap_Control(int16_t Stdid,uint16_t num1,uint16_t num2,uint16_t num3,uint16_t num4);

Motor_Msg_t *Get_DJI_Motor_Data(CAN_Set_t CAN_Set,uint16_t Motor_Stdid);
Super_C_Msg_t *Get_Cap_Data(void);

#endif 
