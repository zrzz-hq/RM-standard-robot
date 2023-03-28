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

//按键开启发射机构
#define SHOOT_START_STOP_KEY KEY_PRESSED_OFFSET_R
#define Shoot_Bullut_Open_KEY KEY_PRESSED_OFFSET_C

//拨弹轮拨一圈能拨出几发弹丸
#define BULLETS_PER_ROTATION  8
//摩擦轮直径
#define FRIC_WHEEL_DIAMETER  0.06
//默认射速
#define SHOOT_MOTOR_SPEED_SET 12

//拨弹速度设置
#define Trigger_Speed_Set_Test 6
#define Trigger_Speed_Set_Fast 2
#define Trigger_Speed_Set_Slow 2

//微动开关宏定义

#define SHOOT_KEY_OFF 0
#define SHOOT_KEY_ON 1

//盖子开关设置
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
//#define MAX_SHOOT_FREQ 5 //机械上能够达到的最大射速

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
	Shoot_Mode_t Shoot_Mode;//发射模式
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
	
	uint8_t Fric_Reverse_Flag;    //摩擦轮电机反转
	float Fric_Wheel_Diameter;		//摩擦轮直径
	int8_t Trigger_Motor_Min_Speed;//拨弹电机最小转速
	int8_t Load_Bullet_Speed;      //拨弹电机加载弹丸转速
	int8_t Unload_Bullet_Speed;		 //拨弹电机退弹丸转速
	uint8_t Bullets_Per_Rotation;  //拨盘上能装多少个弹丸
	
	uint8_t Default_Shoot_Freq_Limit;			//默认射频，如果没有裁判系统数据，使用该数据
	uint16_t Default_Shoot_Speed_Limit;      //默认射速，如果没有裁判系统数据，使用该数据
	//uint8_t Max_Shoot_Freq;										//机械上能够达到的最大射速
	
	uint16_t Trigger_Motor_Low_Speed_Time_Limit; //拨弹电机维持最低速度的最长时间，超过该时间认为电机堵转
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
	
	
	uint16_t Judge_Shoot_Speed_Limit;//当前射速上线
	uint8_t Judge_Robot_ID;
	uint16_t Judge_Shoot_Cooling_Rate;//当前每秒热量冷却值
	uint16_t Judge_Shoot_Cooling_Limit;//当前热量上限
	uint16_t Judge_Shoot_Cooling_Heat; // 当前热量
	float Judge_Shoot_Heat_Percent;
	float Judge_Shoot_Cool_Percent;
	
}Shoot_t;


void Shoot_Task(void *pvParameters);

#endif

