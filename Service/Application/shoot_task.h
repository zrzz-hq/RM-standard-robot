#ifndef _SHOOT_TASK_H_
#define _SHOOT_TASK_H_

#include "main.h"
#include "DJI_Remote_Control.h"
#include "pid.h"

#include "Judge_Data.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#define FRIC_MOTOR_LEFT_KP 1200
#define FRIC_MOTOR_LEFT_KI 0
#define FRIC_MOTOR_LEFT_KD 1000
#define FRIC_MOTOR_LEFT_MAXOUT 10000
#define FRIC_MOTOR_LEFT_IMAXOUT 1000

#define FRIC_MOTOR_RIGHT_KP 1200
#define FRIC_MOTOR_RIGHT_KI 0
#define FRIC_MOTOR_RIGHT_KD 1000
#define FRIC_MOTOR_RIGHT_MAXOUT 10000
#define FRIC_MOTOR_RIGHT_IMAXOUT 1000

#define TRIGGER_MOTOR_KP 1200
#define TRIGGER_MOTOR_KI 0
#define TRIGGER_MOTOR_KD 0
#define TRIGGER_MOTOR_MAXOUT 10000
#define TRIGGER_MOTOR_IMAXOUT 5000

//���������������
#define SHOOT_START_STOP_KEY KEY_PRESSED_OFFSET_R
#define Shoot_Bullut_Open_KEY KEY_PRESSED_OFFSET_C

//�����ֲ�һȦ�ܲ�����������
#define BULLETS_PER_ROTATION  8
//Ħ����ֱ��
#define FRIC_WHEEL_DIAMETER  0.06
//Ĭ������
#define SHOOT_MOTOR_SPEED_SET 12

//�����ٶ�����
#define Trigger_Speed_Set_Test 6
#define Trigger_Speed_Set_Fast 2
#define Trigger_Speed_Set_Slow 2

//΢�����غ궨��

#define SHOOT_KEY_OFF 0
#define SHOOT_KEY_ON 1

//���ӿ�������
#define Shoot_Cover_Close 1070
#define Shoot_Cover_Open 2140
#define Shoot_Cover_Speed_Set 160

#define FRIC_MOTOR_LEFT_ID 0x201
#define FRIC_MOTOR_RIGHT_ID 0x202
#define TRIGGER_MOTOR_ID 0x203
#define SHOOT_MOTOR_ALL_ID 0x200
#define SHOOT_CAN CAN1_RX

#define SHOOT_BULLET_TIME_LIMIT 500
#define SHOOT_MODE_SWITCH_DOWN_TIME_LIMIT 1000
#define SHOOT_STALL_TIME_LIMIT 1000
#define TRIGGER_MOTOR_MIN_SPEED 1 

#define LOAD_BULLET_SPEED 2
#define UNLOAD_BULLET_SPEED -2
#define TRIGGER_MOTOR_LOW_SPEED_TIME_LIMIT 1000

#define DEFAULT_SHOOT_SPEED_LIMIT 10
#define DEFAULT_SHOOT_FREQ_LIMIT  1
//#define MAX_SHOOT_FREQ 5 //��е���ܹ��ﵽ���������

typedef enum
{
	SHOOT_STOP,
	SHOOT_START,
	SHOOT_READY,
	SHOOT_BULLET,
	SHOOT_STALL
}Shoot_Mode_t;

typedef enum
{
	Shoot_Bullut_Close,
	Shoot_Bullut_Open
}Shoot_Bullut_Mode_t;

typedef struct
{
	Shoot_Mode_t Shoot_Mode;//����ģʽ
	Shoot_Mode_t Last_Shoot_Mode;
	
	Motor_Msg_t* Fric_Motor_Msg_Get[2];
	Motor_Msg_t* Trigger_Motor_Msg_Get;
	DJI_Judge_Msg_t* Shoot_Judge_Msg_Get;
	const RC_Ctl_t* Shoot_RC_Ctl_Data;	
	
	PID Fric_Motor_Pid[2];
	PID Trigger_Motor_Pid;
	
	float Trigger_Motor_Speed_Set;//
	float Fric_Motor_Speed_Set;		//
	float Trigger_Motor_Speed_Get;//
	float Fric_Motor_Speed_Get[2];//
	float Fric_Motor_Current_Send[2];//
	float Trigger_Motor_Current_Send;//
	
	uint8_t Fric_Reverse_Flag;    //Ħ���ֵ����ת
	float Fric_Wheel_Diameter;		//Ħ����ֱ��
	int8_t Trigger_Motor_Min_Speed;//���������Сת��
	int8_t Load_Bullet_Speed;      //����������ص���ת��
	int8_t Unload_Bullet_Speed;		 //��������˵���ת��
	uint8_t Bullets_Per_Rotation;  //��������װ���ٸ�����
	
	uint8_t Default_Shoot_Freq_Limit;			//Ĭ����Ƶ�����û�в���ϵͳ���ݣ�ʹ�ø�����
	uint16_t Default_Shoot_Speed_Limit;      //Ĭ�����٣����û�в���ϵͳ���ݣ�ʹ�ø�����
	//uint8_t Max_Shoot_Freq;										//��е���ܹ��ﵽ���������
	
	uint16_t Trigger_Motor_Low_Speed_Time_Limit; //�������ά������ٶȵ��ʱ�䣬������ʱ����Ϊ�����ת
	uint16_t Shoot_Bullet_Time_Limit;						 //
	uint16_t Shoot_Start_Time_Limit;
	uint16_t Shoot_Stall_Time_Limit;
	
	uint16_t Shoot_Stall_Time;
	uint16_t Shoot_Bullet_Time;
	uint16_t Shoot_Start_Time;
	uint16_t Shoot_Mode_Switch_Down_Time;
	uint16_t Trigger_Motor_Low_Speed_Time;
	
//	uint8_t No_Bullet_Flag;
	
	uint8_t Shoot_Key;
	uint8_t Last_Shoot_Key;
	uint8_t Last_Shoot_Mode_Switch;
	uint16_t Last_Shoot_Mode_Key;
	uint8_t Last_Shoot_Mouse_Key;
	
	uint16_t Need_Shoot_Count;
	
	
	uint16_t Judge_Shoot_Speed_Limit;//��ǰ��������
	uint8_t Judge_Robot_ID;
	uint16_t Judge_Shoot_Cooling_Rate;//��ǰÿ��������ȴֵ
	uint16_t Judge_Shoot_Cooling_Limit;//��ǰ��������
	uint16_t Judge_Shoot_Cooling_Heat; // ��ǰ����
	float Judge_Shoot_Heat_Percent;
	float Judge_Shoot_Cool_Percent;
	
}Shoot_t;


void Shoot_Task(void *pvParameters);

#endif

