#ifndef __GIMBAL_TASK_H__
#define __GIMBAL_TASK_H__

#include "main.h"
#include "DJI_Remote_Control.h"
#include "pid.h" 

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#define GIMBAL_RC_DEADLINE  RC_DEADLINE

#define GIMBAL_VISION_KEY KEY_PRESSED_OFFSET_CTRL

#define Gimbal_Motionless_Time 1500

#define Gimbal_Turn_Head_Key KEY_PRESSED_OFFSET_F
#define Gimbal_Turn_Head_Speed 0.005f

#define GIMBAL_FIRST_MODE GIMBAL_ZERO
#define GIMBAL_SECOND_MODE GIMBAL_MOTOR_CONTROL
#define GIMBAL_THIRD_MODE GIMBAL_SPIN

#define GIMBAL_YAW_VISION_ANGLE_KP 1000
#define GIMBAL_YAW_VISION_ANGLE_KI 0
#define GIMBAL_YAW_VISION_ANGLE_KD 0
#define GIMBAL_YAW_VISION_ANGLE_MAXOUT 10
#define GIMBAL_YAW_VISION_ANGLE_IMAXOUT 10

#define GIMBAL_YAW_MOTOR_ANGLE_KP 10
#define GIMBAL_YAW_MOTOR_ANGLE_KI 0
#define GIMBAL_YAW_MOTOR_ANGLE_KD 60
#define GIMBAL_YAW_MOTOR_ANGLE_MAXOUT 10
#define GIMBAL_YAW_MOTOR_ANGLE_IMAXOUT 10

#define GIMBAL_YAW_IMU_ANGLE_KP 20
#define GIMBAL_YAW_IMU_ANGLE_KI 0
#define GIMBAL_YAW_IMU_ANGLE_KD 400
#define GIMBAL_YAW_IMU_ANGLE_MAXOUT 9
#define GIMBAL_YAW_IMU_ANGLE_IMAXOUT 0

#define GIMBAL_YAW_SPEED_KP 5000
#define GIMBAL_YAW_SPEED_KI 40
#define GIMBAL_YAW_SPEED_KD 0
#define GIMBAL_YAW_SPEED_MAXOUT 25000
#define GIMBAL_YAW_SPEED_IMAXOUT 5000

#define GIMBAL_PITCH_VISION_ANGLE_KP 800
#define GIMBAL_PITCH_VISION_ANGLE_KI 0
#define GIMBAL_PITCH_VISION_ANGLE_KD 0
#define GIMBAL_PITCH_VISION_ANGLE_MAXOUT 10
#define GIMBAL_PITCH_VISION_ANGLE_IMAXOUT 10

#define GIMBAL_PITCH_MOTOR_ANGLE_KP 18
#define GIMBAL_PITCH_MOTOR_ANGLE_KI 0
#define GIMBAL_PITCH_MOTOR_ANGLE_KD 20
#define GIMBAL_PITCH_MOTOR_ANGLE_MAXOUT 10
#define GIMBAL_PITCH_MOTOR_ANGLE_IMAXOUT 10

#define GIMBAL_PITCH_IMU_ANGLE_KP 10
#define GIMBAL_PITCH_IMU_ANGLE_KI 0
#define GIMBAL_PITCH_IMU_ANGLE_KD 0
#define GIMBAL_PITCH_IMU_ANGLE_MAXOUT 10
#define GIMBAL_PITCH_IMU_ANGLE_IMAXOUT 0

#define GIMBAL_PITCH_SPEED_KP 5000
#define GIMBAL_PITCH_SPEED_KI 1
#define GIMBAL_PITCH_SPEED_KD 0
#define GIMBAL_PITCH_SPEED_MAXOUT 25000
#define GIMBAL_PITCH_SPEED_IMAXOUT 5000

#define GIMBAL_YAW_IMU_ANGLE_ADDRESS_OFFSET  0
#define GIMBAL_PITCH_IMU_ANGLE_ADDRESS_OFFSET  2
#define GIMBAL_YAW_IMU_ASPEED_ADDRESS_OFFSET  0
#define GIMBAL_PITCH_IMU_ASPEED_ADDRESS_OFFSET  2

#define GIMBAL_YAW_MAX_ANGLE 1.59
#define GIMBAL_YAW_MIN_ANGLE -1.59
#define GIMBAL_PITCH_MAX_ANGLE 0.46
#define GIMBAL_PITCH_MIN_ANGLE -0.34

//中点设置
#define GIMBAL_YAW_MID_ANGLE  2.10          //  -0.0069029131    // 3.09
#define GIMBAL_PITCH_MID_ANGLE  0.53         //    0.0383495167  //  0.53
//鼠标控制的比例
#define GIMBAL_YAW_RC_SEN 278000.0f
#define GIMBAL_YAW_MOUSE_SEN 20000.0f
#define GIMBAL_PITCH_RC_SEN 756000.0f
#define GIMBAL_PITCH_MOUSE_SEN 20000.0f

#define GIMBAL_MOTOR_ENCODER_MID_VAL 4096

#define YAW_MOTOR_ID 0x205
#define PITCH_MOTOR_ID 0x206
#define GIMBAL_MOTOR_ALL_ID 0x1FF
#define GIMBAL_CAN CAN2_RX 

#define GIMBAL_INIT_ANGLE_ERROR  0.1f
#define GIMBAL_INIT_PITCH_SPEED  0.004f
#define GIMBAL_INIT_YAW_SPEED    0.001f
#define GIMBAL_MAX_INIT_TIME 		 5000

#define GIMBAL_MAX_NO_OPT_TIME 1000

typedef enum
{
	GIMBAL_ZERO = 0,//无控制，传送0
	GIMBAL_INIT,//初始化模式，慢慢转动到设定角度，防止转动过猛导致结构损坏
	GIMBAL_MOTIONLESS,//无控制，用电机角度保持当前位置
	GIMBAL_MOTOR_CONTROL,//电机角度控制
	GIMBAL_IMU_CONTROL,//陀螺仪角度控制
	GIMBAL_VISION_CONTROL,//视觉控制接口
	GIMBAL_FOLLOW_CHASSIS,//云台跟随底盘，yaw轴固定不动
	GIMBAL_SPIN//小陀螺模式
}Gimbal_Mode_t;

typedef enum
{
		GIMBAL_MOTOR_NO_FORCE,//电机无力模式，电流为0
		GIMBAL_MOTOR_ENCODER_ANGLE,//电机编码器角度控制模式
		GIMBAL_MOTOR_IMU_ANGLE,//电机IMU角度控制模式
		GIMBAL_MOTOR_SPEED,//电机速度控制模式
}Gimbal_Motor_Mode_t;

typedef struct
{
	Motor_Msg_t* Shoot_Motor_Msg_Get;
	int Motoe_Now_Angle;
	int Motoe_Now_Speed;
	int Motor_Now_Current;
	int Motor_Last_Speed;	
	int Motor_Last_Angle;
}Gimbal_Motor_Msg_t;


typedef struct
{
	//云台电机模式
	Gimbal_Motor_Mode_t Gimbal_Motor_Mode;
	
	//云台电机相关数据
	Motor_Msg_t* Gimbal_Motor_Msg_Get;	
	//陀螺仪角度获取地址
	const float* Gimbal_IMU_Angle_Msg_Get;
	//陀螺仪角速度获取地址
	const float* Gimbal_IMU_Aspeed_Msg_Get;
	
	//云台机械编码器值
	int16_t Gimbal_Motor_Encoder_Get;
	//云台机械角度值
	float Gimbal_Motor_Angle_Get;
	//云台陀螺仪角度值
	float Gimbal_IMU_Angle_Get;
	//云台电机角度设定值
	float Gimbal_Motor_Angle_Set;
	
	//云台机械角速度值
	float Gimbal_Motor_Aspeed_Get;	
	//云台电机角度设定值
	float Gimbal_Motor_ASpeed_Set;
	
	//云台电机角度中点信息
	float Gimbal_Motor_Mid_Angle;	
	//云台电机最大角度信息
	float Gimbal_Motor_Max_Angle;
	//云台电机最小角度信息
	float Gimbal_Motor_Min_Angle;
	
	//云台电机角度环PID
	PID* Gimbal_Motor_Angle_Pid;
	PID* Gimbal_Motor_Speed_Pid;
	//云台电机速度环PID
	
	//云台电机电流设定值
	float Gimbal_Motor_Current_Set;
}Gimbal_Motor_t;

typedef struct
{
	//云台模式
	Gimbal_Mode_t Gimbal_Mode;
	Gimbal_Mode_t Last_Gimbal_Mode;
	
/**********云台相关PID结构体**********/
	//云台绝对角度计算PID-机械角度
	PID Gimbal_Yaw_Motor_Angle_Pid;
	PID Gimbal_Pitch_Motor_Angle_Pid;
	//云台绝对角度计算PID-陀螺仪角度
	PID Gimbal_Yaw_IMU_Angle_Pid;
	PID Gimbal_Pitch_IMU_Angle_Pid;
	//云台视觉计算PID-陀螺仪角度
	PID Gimbal_Yaw_Vision_Angle_Pid;
	PID Gimbal_Pitch_Vision_Angle_Pid;
	
	//云台角速度计算PID
	PID Gimbal_Yaw_Speed_Pid;	
	PID Gimbal_Pitch_Speed_Pid;

	//云台计算PID
	//PID Gimbal_Yaw_Motor_Pid;	
	//PID Gimbal_Pitch_Motor_Pid;	
	
/**********云台相关信息结构体**********/
	Gimbal_Motor_t Gimbal_Yaw_Motor;
	Gimbal_Motor_t Gimbal_Pitch_Motor;
	
	//陀螺仪相关数据获取
	//const float* Gimbal_IMU_Angle;
	//const float* Gimbal_IMU_Aspeed;
	//遥控器控制值获取
	const RC_Ctl_t* Gimbal_Rc_Ctrl_Data;	
	
	float Gimbal_Init_Time;
	float Gimbal_Yaw_Init_Finish_Flag;
	float Gimbal_Pitch_Init_Finish_Flag;
	
	float Gimbal_Yaw_Init_Start_Angle;
	float Gimbal_Pitch_Init_Start_Angle;
	
	float Gimbal_Yaw_Rc_Sen;
	float Gimbal_Pitch_Rc_Sen;
	float Gimbal_Yaw_Mouse_Sen;
	float Gimbal_Pitch_Mouse_Sen;
	
	uint16_t Gimbal_Yaw_No_Opt_Time;
	uint16_t Gimbal_Pitch_No_Opt_Time;
	//云台发送电流值
	//float Gimbal_Motor_Current_Send[2];

}Gimbal_t;


void Gimbal_Task(void *pvParameters);
float* Get_Gimbal_Yaw_Angle(void);

Gimbal_Mode_t* Return_Gimbal_Mode_Add(void);

#endif 
