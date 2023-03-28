 #ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_

#include "main.h"
#include "DJI_Remote_Control.h"
#include "pid.h"
#include "Judge_Data.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#define CHASSIS_MOTIONLESS_TIME 1500

#define CHASSIS_RC_DEADLINE  RC_DEADLINE

#define CHASSIS_MOTOR_SPEED_KP 12000
#define CHASSIS_MOTOR_SPEED_KI 10
#define CHASSIS_MOTOR_SPEED_KD 0
#define CHASSIS_MOTOR_SPEED_MAXOUT 16000
#define CHASSIS_MOTOR_SPEED_IMAXOUT 2000

#define CHASSIS_ANGLE_KP 10
#define CHASSIS_ANGLE_KI 0
#define CHASSIS_ANGLE_KD 1
#define CHASSIS_ANGLE_MAXOUT 5
#define CHASSIS_ANGLE_IMAXOUT 0


#define CHASSIS_3508_RPM_TO_WHEEL_SPEED 0.0012708333f

#define Chassis_Max_Heat_Normal 60
#define Chassis_Max_Heat_Feipo 250

#define CHASSIS_SPEED_SETUP_TIME 1000

#define CHASSIS_TURN_SPEED_UP_KEY KEY_PRESSED_OFFSET_SHIFT
#define CHASSIS_FORWARD_KEY KEY_PRESSED_OFFSET_W
#define CHASSIS_BACKWARD_KEY KEY_PRESSED_OFFSET_S
#define CHASSIS_MOVE_LEFT_KEY KEY_PRESSED_OFFSET_A
#define CHASSIS_MOVE_RIGHT_KEY KEY_PRESSED_OFFSET_D
#define CHASSIS_TURN_LEFT_KEY KEY_PRESSED_OFFSET_E
#define CHASSIS_TURN_RIGHT_KEY KEY_PRESSED_OFFSET_Q
#define CHASSIS_TRANSLATE_KEY 	KEY_PRESSED_OFFSET_X

#define CHASSIS_X_MAX_SPEED 5.0f
#define CHASSIS_Y_MAX_SPEED 5.0f
#define CHASSIS_WZ_MAX_SPEED 5.0f
#define CHASSIS_WZ_SPIN_MAX_SPEED 10.0f

#define Chassis_Follow_Gimbal_Set 6

//底盘定点移动时的速度增量和最大速度
#define Chassis_Translate_Delta_Speed 0.005
#define Chassis_Translate_Max_Speed  1
//底盘定点移动时保持最大速度的时间（ms）
#define Chassis_Translate_Max_Speed_Time 2000

#define CHASSIS_FIRST_MODE CHASSIS_ZERO
#define CHASSIS_SECOND_MODE CHASSIS_RC_CONTROL
#define CHASSIS_THIRD_MODE CHASSIS_FOLLOW_GIMBAL

#define RI_MOTOR_ID 0x201
#define LI_MOTOR_ID 0x202
#define LB_MOTOR_ID 0x203
#define RB_MOTOR_ID 0x204
#define CHASSIS_MOTOR_ALL_ID 0x200
#define CHASSIS_CAN CAN2_RX

typedef enum
{
	CHASSIS_ZERO = 0,//无控制，传送0
	CHASSIS_MOTIONLESS,//无控制，用电机角度保持当前位置
	CHASSIS_RC_CONTROL,//遥控器控制
	CHASSIS_FOLLOW_GIMBAL,//底盘跟随云台
	CHASSIS_SPIN_LEFT,//小陀螺模式
	CHASSIS_SPIN_RIGHT,//小陀螺模式
	CHASSIS_TRANSLATE
}Chassis_Mode_t;


typedef struct
{
	//底盘模式
	Chassis_Mode_t Chassis_Mode;
	
	uint32_t Translate_Time_Count;
	float Chassis_Translate_Speed;
	
	const float* Chassis_IMU_Angle;//陀螺仪相关数据获取
	const float* Chassis_IMU_Aspeed;
	const RC_Ctl_t* Chassis_RC_Ctl_Data;//遥控器控制值获取	
	const Motor_Msg_t* Chassis_Motor_Msg_Get[4];
	const DJI_Judge_Msg_t* Chassis_Judge_Msg_Get;
	const Super_C_Msg_t* Chassis_Cap_Msg_Get;
	
	float Chassis_X_Max_Speed;
	float Chassis_Y_Max_Speed;
	float Chassis_WZ_Max_Speed;
	float Chassis_WZ_Spin_Max_Speed;
	
	float Chassis_X_Speed_Set;	//底盘速度设置	
	float Chassis_Y_Speed_Set;
	float Chassis_WZ_Speed_Set;
	float* Chassis_Follow_Gimbal_Angle_TM;//底盘跟随云台-云台机械角度差
	
	uint16_t Chassis_Forward_Key_Time;
	uint16_t Chassis_Backward_Key_Time;
	uint16_t Chassis_Move_Left_Key_Time;
	uint16_t Chassis_Move_Right_Key_Time;
	uint16_t Chassis_Turn_Left_Key_Time;
	uint16_t Chassis_Turn_Right_Key_Time;
	
	PID Chassis_Motor_Pid[4];//底盘电机相关pid设置
	PID Chassis_Angle_Pid;
	
	float Chassis_Motor_Speed_Set[4];//底盘电机速度设置
	float Chassis_Motor_Speed_Get[4];//底盘电机当前速度
	float Chassis_Motor_Curent_Send[4];//底盘发送电流值
	
	uint8_t Chassis_X_Reverse_Flag;
	uint8_t Chassis_Y_Reverse_Flag;
	uint8_t Chassis_WZ_Reverse_Flag;
	
	uint16_t Chassis_Last_Mode_Key;
	uint8_t Chassis_Last_Mode_Switch;
	
	float Chassis_Power;	//底盘功率数据
	uint16_t Chassis_Power_Buffer;		//底盘热量数据
	uint16_t Chassis_Max_Power;	//当前底盘最大功率

}Chassis_t;

void Chassis_Task(void *pvParameters);

uint8_t Is_Chassis_Spin();

#endif

