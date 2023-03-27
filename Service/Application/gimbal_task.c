/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    gimbal_task.c
	* @brief   云台控制任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 包含六种模式，在App_Set.h中进行设置
	* @Question 	 云台PID还需要进行测试【4.9】
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     4-9-2021     	 JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe 云台控制任务
*/

#include "gimbal_task.h"
#include "chassis_task.h"
#include "IMU_Task.h"

#include "arm_math.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "data_task.h"

#include "App_Set.h"

#include "can.h"
#include "pid.h"
#include "led.h"

#include "DJI_Remote_Control.h"

//extern RC_Ctl_t RC_Ctl_Data;
//extern Chassis_t Chassis;
//#define Gimbal_Motor_Angle_Change 0.0439453125

Gimbal_t Gimbal;



float* Get_Gimbal_Yaw_Angle(void)
{
	return &Gimbal.Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get;
}

//把云台电机编码器角度转化为相对于中值的角度
//float Gimbal_Motor_Angle_Abs(float Now_Angle,float Mid_Angle)
//{
//    float Relative_Angle = Angle_Limit(Now_Angle - Mid_Angle);
//		return Relative_Angle;
//}

//将角度值限制在（-pi，pi）的范围内
#define Angle_Limit(Angle_Set) \
{															 \
															 \
    if (Angle_Set >= PI)			 \
    {													 \
        Angle_Set -= 2*PI;		 \
    }													 \
    else if (Angle_Set <= -PI) \
    {													 \
        Angle_Set += 2*PI;		 \
    }													 \
}

float Gimbal_Motor_Angle_Abs(float Now_Angle,float Mid_Angle)
{
		float Delta_Angle = Now_Angle - Mid_Angle;
		Angle_Limit(Delta_Angle);
		return Delta_Angle;
}
//限制云台电机转动的范围
void Gimbal_Motor_Angle_Limit(float *Angle_Set,float Max_Angle,float Min_Angle)
{
	if(*Angle_Set > Max_Angle)
		*Angle_Set = Max_Angle;
	else if(*Angle_Set < Min_Angle)
		*Angle_Set = Min_Angle;
}

void Gimbal_Add_Variable(Gimbal_t* Gimbal_Variable)
{
		Add_Variable_By_Name_In_Task("Gimbal Pitch Motor Angle Kp",&Gimbal_Variable->Gimbal_Pitch_Motor_Angle_Pid.kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Pitch Motor Angle Ki",&Gimbal_Variable->Gimbal_Pitch_Motor_Angle_Pid.ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Pitch Motor Angle Kd",&Gimbal_Variable->Gimbal_Pitch_Motor_Angle_Pid.kd,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Yaw Motor Angle Kp",&Gimbal_Variable->Gimbal_Yaw_Motor_Angle_Pid.kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Yaw Motor Angle Ki",&Gimbal_Variable->Gimbal_Yaw_Motor_Angle_Pid.ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Yaw Motor Angle Kd",&Gimbal_Variable->Gimbal_Yaw_Motor_Angle_Pid.kd,VAR_TYPE_FP32,0);
	
		Add_Variable_By_Name_In_Task("Gimbal Pitch IMU Angle Kp",&Gimbal_Variable->Gimbal_Pitch_IMU_Angle_Pid.kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Gimbal Pitch IMU Angle Ki",&Gimbal_Variable->Gimbal_Pitch_IMU_Angle_Pid.ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Pitch IMU Angle Kd",&Gimbal_Variable->Gimbal_Pitch_IMU_Angle_Pid.kd,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Yaw IMU Angle Kp",&Gimbal_Variable->Gimbal_Yaw_IMU_Angle_Pid.kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Yaw IMU Angle Ki",&Gimbal_Variable->Gimbal_Yaw_IMU_Angle_Pid.ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Yaw IMU Angle Kd",&Gimbal_Variable->Gimbal_Yaw_IMU_Angle_Pid.kd,VAR_TYPE_FP32,0);
	
		Add_Variable_By_Name_In_Task("Gimbal Pitch IMU Angle",&Gimbal_Variable->Gimbal_Pitch_Motor.Gimbal_IMU_Angle_Get,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Gimbal Pitch Motor Angle",&Gimbal_Variable->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Gimbal Pitch Motor Speed",&Gimbal_Variable->Gimbal_Pitch_Motor.Gimbal_Motor_Aspeed_Get,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Gimbal Yaw IMU Angle",&Gimbal_Variable->Gimbal_Yaw_Motor.Gimbal_IMU_Angle_Get,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Gimbal Yaw Motor Angle",&Gimbal_Variable->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Gimbal Yaw Motor Speed",&Gimbal_Variable->Gimbal_Yaw_Motor.Gimbal_Motor_Aspeed_Get,VAR_TYPE_FP32,1);
		
		Add_Variable_By_Name_In_Task("Gimbal Pitch Mid Angle",&Gimbal_Variable->Gimbal_Pitch_Motor.Gimbal_Motor_Mid_Angle,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Yaw Mid Angle",&Gimbal_Variable->Gimbal_Yaw_Motor.Gimbal_Motor_Mid_Angle,VAR_TYPE_FP32,0);
		
		Add_Variable_By_Name_In_Task("Gimbal Pitch Motor Given Current",&Gimbal_Variable->Gimbal_Pitch_Motor.Gimbal_Motor_Current_Set,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Gimbal Yaw Motor Given Current",&Gimbal_Variable->Gimbal_Yaw_Motor.Gimbal_Motor_Current_Set,VAR_TYPE_FP32,1);
		
		Add_Variable_By_Name_In_Task("Gimbal Yaw Angle Set",&Gimbal_Variable->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Gimbal Pitch Angle Set",&Gimbal_Variable->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set,VAR_TYPE_FP32,1);
		
		Add_Variable_By_Name_In_Task("Gimbal Yaw Rc Sen",&Gimbal_Variable->Gimbal_Yaw_Rc_Sen,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Pitch Rc Sen",&Gimbal_Variable->Gimbal_Pitch_Rc_Sen,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Yaw Mouse Sen",&Gimbal_Variable->Gimbal_Yaw_Mouse_Sen,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Gimbal Pitch Mouse Sen",&Gimbal_Variable->Gimbal_Pitch_Mouse_Sen,VAR_TYPE_FP32,0);
		
//		Add_Variable_By_Name_In_Task("Gimbal Yaw Rc",&Gimbal_Variable->Gimbal_Rc_Ctrl_Data->rc.ch2,VAR_TYPE_INT16,1,0);
//		Add_Variable_By_Name_In_Task("Gimbal Pitch Rc",&Gimbal_Variable->Gimbal_Rc_Ctrl_Data->rc.ch3,VAR_TYPE_INT16,1,0);
		
		Add_Variable_By_Name_In_Task("Gimbal Mode",&Gimbal_Variable->Gimbal_Mode,VAR_TYPE_UINT8,1);
		
}

//云台初始化
void Gimbal_Init(Gimbal_t* Gimbal_Init)
{
	//pid数据初始化
	pid_math_init(&Gimbal_Init->Gimbal_Pitch_Motor_Angle_Pid,GIMBAL_PITCH_MOTOR_ANGLE_KP,GIMBAL_PITCH_MOTOR_ANGLE_KI,GIMBAL_PITCH_MOTOR_ANGLE_KD,GIMBAL_PITCH_MOTOR_ANGLE_MAXOUT,GIMBAL_PITCH_MOTOR_ANGLE_IMAXOUT,0.5f,0.05f,0.9f,0.01f);
	pid_math_init(&Gimbal_Init->Gimbal_Yaw_Motor_Angle_Pid,GIMBAL_YAW_MOTOR_ANGLE_KP,GIMBAL_YAW_MOTOR_ANGLE_KI,GIMBAL_YAW_MOTOR_ANGLE_KD,GIMBAL_YAW_MOTOR_ANGLE_MAXOUT,GIMBAL_YAW_MOTOR_ANGLE_IMAXOUT,0.5f,0.05f,0.9f,0.04f);	
	pid_math_init(&Gimbal_Init->Gimbal_Yaw_IMU_Angle_Pid,GIMBAL_YAW_IMU_ANGLE_KP,GIMBAL_YAW_IMU_ANGLE_KI,GIMBAL_YAW_IMU_ANGLE_KD,GIMBAL_YAW_IMU_ANGLE_MAXOUT,GIMBAL_YAW_IMU_ANGLE_IMAXOUT,0.5f,0.05f,0.2f,0.01f);	
	
	pid_init(&Gimbal_Init->Gimbal_Pitch_IMU_Angle_Pid,GIMBAL_PITCH_IMU_ANGLE_KP,GIMBAL_PITCH_IMU_ANGLE_KI,GIMBAL_PITCH_IMU_ANGLE_KD,GIMBAL_PITCH_IMU_ANGLE_MAXOUT,GIMBAL_PITCH_IMU_ANGLE_IMAXOUT,1);
	pid_init(&Gimbal_Init->Gimbal_Yaw_Vision_Angle_Pid,GIMBAL_YAW_VISION_ANGLE_KP,GIMBAL_YAW_VISION_ANGLE_KI,GIMBAL_YAW_VISION_ANGLE_KD,GIMBAL_YAW_VISION_ANGLE_MAXOUT,GIMBAL_YAW_VISION_ANGLE_IMAXOUT,1);
	
	pid_init(&Gimbal_Init->Gimbal_Yaw_Speed_Pid,GIMBAL_YAW_SPEED_KP,GIMBAL_YAW_SPEED_KI,GIMBAL_YAW_SPEED_KD,GIMBAL_YAW_SPEED_MAXOUT,GIMBAL_YAW_SPEED_IMAXOUT,1);
	pid_init(&Gimbal_Init->Gimbal_Pitch_Speed_Pid,GIMBAL_PITCH_SPEED_KP,GIMBAL_PITCH_SPEED_KI,GIMBAL_PITCH_SPEED_KD,GIMBAL_PITCH_SPEED_MAXOUT,GIMBAL_PITCH_SPEED_IMAXOUT,1);

	//电机数据获取地址
	Gimbal_Init->Gimbal_Yaw_Motor.Gimbal_Motor_Msg_Get = Get_DJI_Motor_Data(GIMBAL_CAN,YAW_MOTOR_ID);
	Gimbal_Init->Gimbal_Pitch_Motor.Gimbal_Motor_Msg_Get = Get_DJI_Motor_Data(GIMBAL_CAN,PITCH_MOTOR_ID);
	//陀螺仪数据获取地址
	Gimbal_Init->Gimbal_Yaw_Motor.Gimbal_IMU_Angle_Msg_Get = get_INS_angle_point() + GIMBAL_YAW_IMU_ANGLE_ADDRESS_OFFSET;
	Gimbal_Init->Gimbal_Pitch_Motor.Gimbal_IMU_Angle_Msg_Get =  get_INS_angle_point() + GIMBAL_PITCH_IMU_ANGLE_ADDRESS_OFFSET;
	Gimbal_Init->Gimbal_Yaw_Motor.Gimbal_IMU_Aspeed_Msg_Get = get_MPU6500_Gyro_Data_Point() + GIMBAL_YAW_IMU_ASPEED_ADDRESS_OFFSET;
	Gimbal_Init->Gimbal_Pitch_Motor.Gimbal_IMU_Aspeed_Msg_Get = get_MPU6500_Gyro_Data_Point() + GIMBAL_PITCH_IMU_ASPEED_ADDRESS_OFFSET;
	
	//云台最大最小值设置
	Gimbal_Init->Gimbal_Yaw_Motor.Gimbal_Motor_Max_Angle = GIMBAL_YAW_MAX_ANGLE;
	Gimbal_Init->Gimbal_Yaw_Motor.Gimbal_Motor_Min_Angle = GIMBAL_YAW_MIN_ANGLE;
	Gimbal_Init->Gimbal_Pitch_Motor.Gimbal_Motor_Max_Angle = GIMBAL_PITCH_MAX_ANGLE;
	Gimbal_Init->Gimbal_Pitch_Motor.Gimbal_Motor_Min_Angle = GIMBAL_PITCH_MIN_ANGLE;
	
	//遥控器控制值获取地址
	Gimbal_Init->Gimbal_Rc_Ctrl_Data = Get_DJI_RC_Data_Address();
	
	//电机中点数据初始化
	Gimbal_Init->Gimbal_Yaw_Motor.Gimbal_Motor_Mid_Angle = GIMBAL_YAW_MID_ANGLE;
	Gimbal_Init->Gimbal_Pitch_Motor.Gimbal_Motor_Mid_Angle = GIMBAL_PITCH_MID_ANGLE;
	
	//电机电流值清零
 	//Gimbal_Init->Gimbal_Motor_Current_Send[0] = 0;
	//Gimbal_Init->Gimbal_Motor_Current_Send[1] = 0;
	//遥控器比例初始化
	Gimbal_Init->Gimbal_Pitch_Rc_Sen = GIMBAL_PITCH_RC_SEN;
	Gimbal_Init->Gimbal_Yaw_Rc_Sen = GIMBAL_YAW_RC_SEN;
	Gimbal_Init->Gimbal_Pitch_Mouse_Sen = GIMBAL_PITCH_MOUSE_SEN;
	Gimbal_Init->Gimbal_Yaw_Mouse_Sen = GIMBAL_YAW_MOUSE_SEN;
	
}

void Gimbal_Data_Update(Gimbal_t* Gimbal_Data)
{
	//读取云台电机机械编码器值
	Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_Motor_Encoder_Get = Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_Motor_Msg_Get->Angle;
	Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_Motor_Encoder_Get = Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_Motor_Msg_Get->Angle;
	//云台电机机械编码器值转化为云台角度
	float Gimbal_Pitch_Motor_Angle = ((float)Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_Motor_Encoder_Get - GIMBAL_MOTOR_ENCODER_MID_VAL) /GIMBAL_MOTOR_ENCODER_MID_VAL * PI;
	float Gimbal_Yaw_Motor_Angle = ((float)Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_Motor_Encoder_Get - GIMBAL_MOTOR_ENCODER_MID_VAL) /GIMBAL_MOTOR_ENCODER_MID_VAL * PI;
	
	Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get = Gimbal_Motor_Angle_Abs(Gimbal_Pitch_Motor_Angle,Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_Motor_Mid_Angle);
	Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get = Gimbal_Motor_Angle_Abs(Gimbal_Yaw_Motor_Angle,Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_Motor_Mid_Angle);
	//读取云台电机速度
	Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_Motor_Aspeed_Get = (Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_Motor_Msg_Get->Speed)/(60*2*3.14);
	Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_Motor_Aspeed_Get = (Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_Motor_Msg_Get->Speed)/(60*2*3.14);
	//读取IMU角度值
	Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_IMU_Angle_Get = *(Gimbal_Data->Gimbal_Pitch_Motor.Gimbal_IMU_Angle_Msg_Get);
	Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_IMU_Angle_Get= *(Gimbal_Data->Gimbal_Yaw_Motor.Gimbal_IMU_Angle_Msg_Get);
}

void Gimbal_Mode_Set(Gimbal_t* Mode_Set)
{
	
	//遥控器无操作时间统计
//	if(DJI_RC_Motion_Check())
//	{
//			RC_Motionless_Count = 0;
//	}
//	else
//	{
//			if(RC_Motionless_Count <= Gimbal_Motionless_Time)
//				RC_Motionless_Count ++;
//	}
//	
//	//模式切换时，清空计数器
//	if(Mode_Set->Gimbal_Rc_Ctrl_Data->rc.s2 != Last_RC)
//	{
//			RC_Motionless_Count = 0;
//	}
//	
//	//底盘小陀螺时，云台不能进入不动模式
//	if((*Return_Chassis_Mode_Add() == CHASSIS_SPIN_LEFT)||(*Return_Chassis_Mode_Add()== Chassis_Spin_Right))
//	{
//			RC_Motionless_Count = 0;
//	}
//	
//	
//	//如果底盘是两点移动模式，云台不动。
//	//如果云台是Spin模式且长时间没有操作，那么云台不动
//	if(((RC_Motionless_Count > Gimbal_Motionless_Time)&&(Mode_Set->Gimbal_Mode == GIMBAL_SPIN))||(*Return_Chassis_Mode_Add()==CHASSIS_TRANSLATE))
//	{
//			Mode_Set->Gimbal_Mode = GIMBAL_MOTIONLESS;		
//	}
//	else
//	{
			if(Mode_Set->Gimbal_Mode==GIMBAL_INIT&&Mode_Set->Gimbal_Init_Time<GIMBAL_MAX_INIT_TIME)
			{
					if(!(Mode_Set->Gimbal_Pitch_Init_Finish_Flag&&Mode_Set->Gimbal_Yaw_Init_Finish_Flag))
					{
							Mode_Set->Gimbal_Init_Time++;
							return;
					}
			}

			if(Mode_Set->Gimbal_Rc_Ctrl_Data->rc.s2 == RC_SW_MID)
			{
				Mode_Set->Gimbal_Mode = GIMBAL_SECOND_MODE;
			}
			else if(Mode_Set->Gimbal_Rc_Ctrl_Data->rc.s2 == RC_SW_UP)
			{
				Mode_Set->Gimbal_Mode = GIMBAL_THIRD_MODE;
			}	
			else if(Mode_Set->Gimbal_Rc_Ctrl_Data->rc.s2 == RC_SW_DOWN)
			{
				Mode_Set->Gimbal_Mode = GIMBAL_FIRST_MODE;
			}	
			
			if(Mode_Set->Gimbal_Mode!=GIMBAL_ZERO&&Mode_Set->Last_Gimbal_Mode==GIMBAL_ZERO)
			{
					Mode_Set->Gimbal_Mode = GIMBAL_INIT;
			}
			
//	}
	
//	Last_RC = Mode_Set->Gimbal_Rc_Ctrl_Data->rc.s2;
}

void Gimbal_Mode_Change(Gimbal_t* Mode_Change)
{
	
	if((Mode_Change->Last_Gimbal_Mode != GIMBAL_MOTIONLESS) && (Mode_Change->Gimbal_Mode == GIMBAL_MOTIONLESS))
	{
			Mode_Change->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set= Mode_Change->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get;
			Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get;	
	}
	
	if((Mode_Change->Last_Gimbal_Mode != GIMBAL_MOTOR_CONTROL) && (Mode_Change->Gimbal_Mode == GIMBAL_MOTOR_CONTROL))
	{
			Mode_Change->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get;
			Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get;
	}
	
	if((Mode_Change->Last_Gimbal_Mode != GIMBAL_IMU_CONTROL) && (Mode_Change->Gimbal_Mode == GIMBAL_IMU_CONTROL))
	{
			Mode_Change->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Yaw_Motor.Gimbal_IMU_Angle_Get;
			Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get;
	}
		
	if((Mode_Change->Last_Gimbal_Mode != GIMBAL_SPIN) && (Mode_Change->Gimbal_Mode == GIMBAL_SPIN))
	{
			Mode_Change->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Yaw_Motor.Gimbal_IMU_Angle_Get;
			Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get;
	}
	
	if((Mode_Change->Gimbal_Mode == GIMBAL_INIT)&&(Mode_Change->Last_Gimbal_Mode!=GIMBAL_INIT))
	{
			Mode_Change->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get;
			Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set = Mode_Change->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get;
			Mode_Change->Gimbal_Pitch_Init_Finish_Flag = 0;
			Mode_Change->Gimbal_Yaw_Init_Finish_Flag = 0;
			Mode_Change->Gimbal_Init_Time = 0;
	}
	
	Mode_Change->Last_Gimbal_Mode = Mode_Change->Gimbal_Mode;
}

void Gimbal_Motor_Pid_Calc(Gimbal_Motor_t* Gimbal_Motor)
{
		if(Gimbal_Motor->Gimbal_Motor_Mode == GIMBAL_MOTOR_ENCODER_ANGLE)
		{
				Gimbal_Motor->Gimbal_Motor_ASpeed_Set = \
				pid_calc_circular(Gimbal_Motor->Gimbal_Motor_Angle_Pid,\
				Gimbal_Motor->Gimbal_Motor_Angle_Get,Gimbal_Motor->Gimbal_Motor_Angle_Set,PI,-PI);
		}
		else if(Gimbal_Motor->Gimbal_Motor_Mode == GIMBAL_MOTOR_IMU_ANGLE)
		{
				Gimbal_Motor->Gimbal_Motor_ASpeed_Set = \
				pid_calc_circular(Gimbal_Motor->Gimbal_Motor_Angle_Pid,\
				Gimbal_Motor->Gimbal_IMU_Angle_Get,Gimbal_Motor->Gimbal_Motor_Angle_Set,PI,-PI);
		}
		
		if(Gimbal_Motor->Gimbal_Motor_Mode!=GIMBAL_MOTOR_NO_FORCE)
		{
				Gimbal_Motor->Gimbal_Motor_Current_Set = \
				pid_calc_linear(Gimbal_Motor->Gimbal_Motor_Speed_Pid,\
				Gimbal_Motor->Gimbal_Motor_Aspeed_Get,Gimbal_Motor->Gimbal_Motor_ASpeed_Set);
		}
		else
		{
				Gimbal_Motor->Gimbal_Motor_Current_Set = 0;
		}
}

void Rc_Data_To_Yaw_Delta_Angle(Gimbal_t* Gimbal_Rc,float* Delta_Angle)
{
		float Gimbal_Yaw_Rc_Add_Angle = (float)(RC_DeadBand_Limit(Gimbal_Rc->Gimbal_Rc_Ctrl_Data->rc.ch2,GIMBAL_RC_DEADLINE))/Gimbal_Rc->Gimbal_Yaw_Rc_Sen;
		float Gimbal_Yaw_Mouse_Add_Angle = (float)(Gimbal_Rc->Gimbal_Rc_Ctrl_Data->mouse.x)/Gimbal_Rc->Gimbal_Yaw_Mouse_Sen;
	
		*Delta_Angle = Gimbal_Yaw_Rc_Add_Angle+Gimbal_Yaw_Mouse_Add_Angle; 
}

void Rc_Data_To_Pitch_Delta_Angle(Gimbal_t* Gimbal_Rc,float* Delta_Angle)
{
		float Gimbal_Pitch_Rc_Add_Angle = (float)(RC_DeadBand_Limit(Gimbal_Rc->Gimbal_Rc_Ctrl_Data->rc.ch3,GIMBAL_RC_DEADLINE))/Gimbal_Rc->Gimbal_Pitch_Rc_Sen; 
		float Gimbal_Pitch_Mouse_Add_Angle = (float)(Gimbal_Rc->Gimbal_Rc_Ctrl_Data->mouse.y)/Gimbal_Rc->Gimbal_Pitch_Mouse_Sen;
	
		*Delta_Angle = Gimbal_Pitch_Rc_Add_Angle+Gimbal_Pitch_Mouse_Add_Angle; 
}

//云台陀螺仪控制，无限制模式
void Gimbal_IMU_Control_NL_Set(Gimbal_t* Gimbal_Control_Set)
{
	//设定电机模式
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_ENCODER_ANGLE;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_IMU_ANGLE;
	//设定电机PID
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Pid = &Gimbal_Control_Set->Gimbal_Pitch_Motor_Angle_Pid;
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Speed_Pid = &Gimbal_Control_Set->Gimbal_Pitch_Speed_Pid;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Pid = &Gimbal_Control_Set->Gimbal_Yaw_IMU_Angle_Pid;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Speed_Pid = &Gimbal_Control_Set->Gimbal_Yaw_Speed_Pid;
}

//云台IMU陀螺仪数据控制模式
void Gimbal_IMU_Control_Set(Gimbal_t* Gimbal_Control_Set)
{
	//云台角度限制
	Gimbal_Motor_Angle_Limit(&Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set,
											Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Max_Angle,
											Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Min_Angle);
	//设定电机模式
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_ENCODER_ANGLE;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_IMU_ANGLE;
	//设定电机PID
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Pid = &Gimbal_Control_Set->Gimbal_Pitch_Motor_Angle_Pid;
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Speed_Pid = &Gimbal_Control_Set->Gimbal_Pitch_Speed_Pid;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Pid = &Gimbal_Control_Set->Gimbal_Yaw_IMU_Angle_Pid;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Speed_Pid = &Gimbal_Control_Set->Gimbal_Yaw_Speed_Pid;
}    

void Gimbal_Motor_Control_Set(Gimbal_t* Gimbal_Control_Set)
{
	//云台角度限制
	Gimbal_Motor_Angle_Limit(&Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set,
											Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Max_Angle,
											Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Min_Angle);
	
	Gimbal_Motor_Angle_Limit(&Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set,
											Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Max_Angle,
											Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Min_Angle);
	
	//设定电机模式
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_ENCODER_ANGLE;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_ENCODER_ANGLE;
	//设定电机PID
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Pid = &Gimbal_Control_Set->Gimbal_Pitch_Motor_Angle_Pid;
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Speed_Pid = &Gimbal_Control_Set->Gimbal_Pitch_Speed_Pid;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Pid = &Gimbal_Control_Set->Gimbal_Yaw_Motor_Angle_Pid;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Speed_Pid = &Gimbal_Control_Set->Gimbal_Yaw_Speed_Pid;
}

void Gimbal_Init_Control_Set(Gimbal_t* Gimbal_Control_Set)
{
		//设定电机模式
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_ENCODER_ANGLE;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_ENCODER_ANGLE;
	//设定电机PID
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Pid = &Gimbal_Control_Set->Gimbal_Pitch_Motor_Angle_Pid;
	Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Speed_Pid = &Gimbal_Control_Set->Gimbal_Pitch_Speed_Pid;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Pid = &Gimbal_Control_Set->Gimbal_Yaw_Motor_Angle_Pid;
	Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Speed_Pid = &Gimbal_Control_Set->Gimbal_Yaw_Speed_Pid;
}

void Gimbal_Zero_Control_Set(Gimbal_t* Gimbal_Control_Set)
{
		Gimbal_Control_Set->Gimbal_Pitch_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_NO_FORCE;
		Gimbal_Control_Set->Gimbal_Yaw_Motor.Gimbal_Motor_Mode = GIMBAL_MOTOR_NO_FORCE;
}

void Gimbal_Control_Data_Set(Gimbal_t* Gimbal_Control_Set)
{
	switch(Gimbal_Control_Set->Gimbal_Mode)
	{
		case GIMBAL_INIT:
			Gimbal_Init_Control_Set(Gimbal_Control_Set);
			break;
		case GIMBAL_MOTOR_CONTROL:
		case GIMBAL_MOTIONLESS:
			Gimbal_Motor_Control_Set(Gimbal_Control_Set);
			break;
		case GIMBAL_IMU_CONTROL:
			Gimbal_IMU_Control_Set(Gimbal_Control_Set);
			break;
		case GIMBAL_SPIN:
			Gimbal_IMU_Control_NL_Set(Gimbal_Control_Set);
			break;
		case GIMBAL_ZERO:
			Gimbal_Zero_Control_Set(Gimbal_Control_Set);
			break;
		default:
			break;
	}
	
}

void Gimbal_Control_Data_Get(Gimbal_t* Gimbal_Control_Get)
{
	float Yaw_Add_Angle = 0;
	float Pitch_Add_Angle = 0;
	
	switch(Gimbal_Control_Get->Gimbal_Mode)
	{
		case GIMBAL_MOTOR_CONTROL:
		case GIMBAL_SPIN:
			Rc_Data_To_Yaw_Delta_Angle(Gimbal_Control_Get,&Yaw_Add_Angle);
			Rc_Data_To_Pitch_Delta_Angle(Gimbal_Control_Get,&Pitch_Add_Angle);
			break;
		case GIMBAL_IMU_CONTROL:
			Rc_Data_To_Yaw_Delta_Angle(Gimbal_Control_Get,&Yaw_Add_Angle);
			Rc_Data_To_Pitch_Delta_Angle(Gimbal_Control_Get,&Pitch_Add_Angle);
		
			float Old_Angle = Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get;
			float Delta_Angle = Gimbal_Motor_Angle_Abs(Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set,
																								 Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_IMU_Angle_Get);
			float Max_Angle = Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_Motor_Max_Angle;
			float Min_Angle = Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_Motor_Min_Angle;
			
			if(Old_Angle + Delta_Angle + Yaw_Add_Angle > Max_Angle)
			{
				Yaw_Add_Angle = Max_Angle - Old_Angle - Delta_Angle; 
			}
			
			if(Old_Angle + Delta_Angle + Yaw_Add_Angle < Min_Angle)
			{
				Yaw_Add_Angle = Min_Angle - Old_Angle - Delta_Angle; 
			}
			break;
		case GIMBAL_INIT:
			if (fabs(0 - Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get) > GIMBAL_INIT_ANGLE_ERROR)
			{
					Yaw_Add_Angle = (0 - Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Get) * GIMBAL_INIT_YAW_SPEED;
			}
			else
			{
					Yaw_Add_Angle = 0;
					Gimbal_Control_Get->Gimbal_Yaw_Init_Finish_Flag = 1;
			}
			
			if (fabs(0 - Gimbal_Control_Get->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get) > GIMBAL_INIT_ANGLE_ERROR)
			{
					Pitch_Add_Angle = (0 - Gimbal_Control_Get->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Get) * GIMBAL_INIT_PITCH_SPEED;
			}
			else
			{
					Pitch_Add_Angle = 0;
					Gimbal_Control_Get->Gimbal_Pitch_Init_Finish_Flag = 1;
			}
			break;
		default:
			break;
	}
	Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set+=Yaw_Add_Angle;
	Gimbal_Control_Get->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set+=Pitch_Add_Angle;
	//限制角度在（-pi，pi）
	Angle_Limit(Gimbal_Control_Get->Gimbal_Yaw_Motor.Gimbal_Motor_Angle_Set);
	Angle_Limit(Gimbal_Control_Get->Gimbal_Pitch_Motor.Gimbal_Motor_Angle_Set);
}

void Gimbal_Task(void *pvParameters)
{
	vTaskDelay(250);
	Gimbal_Init(&Gimbal);
	Gimbal_Add_Variable(&Gimbal);
	while(1)
	{

		Gimbal_Mode_Set(&Gimbal);
		Gimbal_Data_Update(&Gimbal);
		Gimbal_Mode_Change(&Gimbal);
		Gimbal_Control_Data_Get(&Gimbal);
		Gimbal_Control_Data_Set(&Gimbal);
		Gimbal_Motor_Pid_Calc(&Gimbal.Gimbal_Pitch_Motor);
		Gimbal_Motor_Pid_Calc(&Gimbal.Gimbal_Yaw_Motor);
		CAN2_Motor_Control(GIMBAL_MOTOR_ALL_ID,(int16_t)Gimbal.Gimbal_Yaw_Motor.Gimbal_Motor_Current_Set,
																					 (int16_t)Gimbal.Gimbal_Pitch_Motor.Gimbal_Motor_Current_Set,0,0);
		
		vTaskDelay(1);	
	}
}


Gimbal_Mode_t* Return_Gimbal_Mode_Add(void)
{
	return &Gimbal.Gimbal_Mode;
}
