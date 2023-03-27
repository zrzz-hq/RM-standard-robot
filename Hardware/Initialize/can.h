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
//ת�� ���ٶ�

//���������Ϣ
typedef struct 
{
	uint16_t Voilt_In;//�����ѹ
	uint16_t Voilt_C;//���ݵ�ѹ
	uint16_t Current_In;//�������
	uint16_t Target_power;//Ŀ�깦��
}Super_C_Msg_t;

typedef struct 
{
	int16_t Angle;//��е�Ƕ�
	int16_t Speed;//�ٶ�
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

void CAN1_Super_C_Send(int16_t Stdid,uint16_t data);

Motor_Msg_t *Get_DJI_Motor_Data(CAN_Set_t CAN_Set,uint16_t Motor_Stdid);
Super_C_Msg_t *Get_Cap_Data(void);

#endif 
