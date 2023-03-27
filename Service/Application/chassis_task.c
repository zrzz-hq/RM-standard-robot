/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    chassis_task.c
	* @brief   底盘控制任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 包含六种模式，在App_Set.h中进行设置
	* @Question 	 等超级电容加上后需要进行闭环测试【4.9】
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     4-9-2021     	 JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe 底盘控制任务
*/
#include "chassis_task.h"
#include "gimbal_task.h"
#include "data_task.h"

#include "math.h"
#include "arm_math.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "can.h"
#include "pid.h"
#include "DJI_Remote_Control.h"

Chassis_t Chassis;

extern Gimbal_t Gimbal;

void Chassis_Add_Variable(Chassis_t* Chassis_Variable)
{
		Add_Variable_By_Name_In_Task("Chassis Motor Kp",&Chassis_Variable->Chassis_Motor_Pid[0].kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Chassis Motor Ki",&Chassis_Variable->Chassis_Motor_Pid[0].ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Chassis Motor Kd",&Chassis_Variable->Chassis_Motor_Pid[0].kd,VAR_TYPE_FP32,0);
	
		Add_Variable_By_Name_In_Task("Chassis Turn Kp",&Chassis_Variable->Chassis_Angle_Pid.kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Chassis Turn Ki",&Chassis_Variable->Chassis_Angle_Pid.ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Chassis Turn Kd",&Chassis_Variable->Chassis_Angle_Pid.kd,VAR_TYPE_FP32,0);	
	
		Add_Variable_By_Name_In_Task("Chassis X Speed Set",&Chassis_Variable->Chassis_X_Speed_Set,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Y Speed Set",&Chassis_Variable->Chassis_Y_Speed_Set,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis_WZ_Speed_Set",&Chassis_Variable->Chassis_WZ_Speed_Set,VAR_TYPE_FP32,1);
	
		Add_Variable_By_Name_In_Task("Chassis Motor1 Speed",&Chassis_Variable->Chassis_Motor_Speed_Get[0],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor2 Speed",&Chassis_Variable->Chassis_Motor_Speed_Get[1],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor3 Speed",&Chassis_Variable->Chassis_Motor_Speed_Get[1],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor4 Speed",&Chassis_Variable->Chassis_Motor_Speed_Get[2],VAR_TYPE_FP32,1);
	
		Add_Variable_By_Name_In_Task("Chassis Motor1 Speed Set",&Chassis_Variable->Chassis_Motor_Speed_Set[0],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor2 Speed Set",&Chassis_Variable->Chassis_Motor_Speed_Set[1],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor3 Speed Set",&Chassis_Variable->Chassis_Motor_Speed_Set[2],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor4 Speed Set",&Chassis_Variable->Chassis_Motor_Speed_Set[3],VAR_TYPE_FP32,1);
		
		Add_Variable_By_Name_In_Task("Chassis Motor1 Given Current",&Chassis_Variable->Chassis_Motor_Curent_Send[0],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor2 Given Current",&Chassis_Variable->Chassis_Motor_Curent_Send[1],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor3 Given Current",&Chassis_Variable->Chassis_Motor_Curent_Send[2],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Chassis Motor4 Given Current",&Chassis_Variable->Chassis_Motor_Curent_Send[3],VAR_TYPE_FP32,1);
		
		Add_Variable_By_Name_In_Task("Chassis X Max Speed",&Chassis_Variable->Chassis_X_Max_Speed,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Chassis Y Max Speed",&Chassis_Variable->Chassis_Y_Max_Speed,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Chassis WZ Max Speed",&Chassis_Variable->Chassis_WZ_Max_Speed,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Chassis WZ Spin Max Speed",&Chassis_Variable->Chassis_WZ_Spin_Max_Speed,VAR_TYPE_FP32,0);
				
//		Add_Variable_By_Name_In_Task("Chassis X Rc",&Chassis_Variable->Chassis_RC_Ctl_Data->rc.ch0,VAR_TYPE_INT16,1,0);
//		Add_Variable_By_Name_In_Task("Chassis Y Rc",&Chassis_Variable->Chassis_RC_Ctl_Data->rc.ch1,VAR_TYPE_INT16,1,0);
//		Add_Variable_By_Name_In_Task("Chassis WZ Rc",&Chassis_Variable->Chassis_RC_Ctl_Data->rc.ch4,VAR_TYPE_INT16,1,0);
		
		Add_Variable_By_Name_In_Task("Chassis Mode",&Chassis_Variable->Chassis_Mode,VAR_TYPE_UINT8,1);
}

void Chassis_Init(Chassis_t* Chassis_Data_Init)
{
	//底盘模式初始化
	Chassis_Data_Init->Chassis_Mode = CHASSIS_ZERO;
	
	//底盘电机PID初始化
	pid_init(&Chassis_Data_Init->Chassis_Motor_Pid[0],CHASSIS_MOTOR_SPEED_KP,CHASSIS_MOTOR_SPEED_KI,CHASSIS_MOTOR_SPEED_KD,CHASSIS_MOTOR_SPEED_MAXOUT,CHASSIS_MOTOR_SPEED_IMAXOUT,PID_DELTA);
	pid_init(&Chassis_Data_Init->Chassis_Motor_Pid[1],CHASSIS_MOTOR_SPEED_KP,CHASSIS_MOTOR_SPEED_KI,CHASSIS_MOTOR_SPEED_KD,CHASSIS_MOTOR_SPEED_MAXOUT,CHASSIS_MOTOR_SPEED_IMAXOUT,PID_DELTA);
	pid_init(&Chassis_Data_Init->Chassis_Motor_Pid[2],CHASSIS_MOTOR_SPEED_KP,CHASSIS_MOTOR_SPEED_KI,CHASSIS_MOTOR_SPEED_KD,CHASSIS_MOTOR_SPEED_MAXOUT,CHASSIS_MOTOR_SPEED_IMAXOUT,PID_DELTA);
	pid_init(&Chassis_Data_Init->Chassis_Motor_Pid[3],CHASSIS_MOTOR_SPEED_KP,CHASSIS_MOTOR_SPEED_KI,CHASSIS_MOTOR_SPEED_KD,CHASSIS_MOTOR_SPEED_MAXOUT,CHASSIS_MOTOR_SPEED_IMAXOUT,PID_DELTA);
	pid_init(&Chassis_Data_Init->Chassis_Angle_Pid,CHASSIS_ANGLE_KP,CHASSIS_ANGLE_KI,CHASSIS_ANGLE_KD,CHASSIS_ANGLE_MAXOUT,CHASSIS_ANGLE_IMAXOUT,PID_DELTA);
	
	//获取底盘电机数据地址
	Chassis_Data_Init->Chassis_Motor_Msg_Get[0] = Get_DJI_Motor_Data(CHASSIS_CAN,RI_MOTOR_ID);
	Chassis_Data_Init->Chassis_Motor_Msg_Get[1] = Get_DJI_Motor_Data(CHASSIS_CAN,LI_MOTOR_ID);
	Chassis_Data_Init->Chassis_Motor_Msg_Get[2] = Get_DJI_Motor_Data(CHASSIS_CAN,LB_MOTOR_ID);
	Chassis_Data_Init->Chassis_Motor_Msg_Get[3] = Get_DJI_Motor_Data(CHASSIS_CAN,RB_MOTOR_ID);
	
	//获取超级电容数据地址
	Chassis_Data_Init->Chassis_Cap_Msg_Get = Get_Cap_Data();
	
	//底盘控制数据清零
	for(int i = 0;i < 4;i++)
	{
		Chassis_Data_Init->Chassis_Motor_Speed_Set[i] = 0;
		Chassis_Data_Init->Chassis_Motor_Curent_Send[i] = 0;
	}
	
	//底盘跟随云台角度获取地址
	Chassis_Data_Init->Chassis_Follow_Gimbal_Angle_TM = Get_Gimbal_Yaw_Angle();
	
	//遥控器控制值获取地址
	Chassis_Data_Init->Chassis_RC_Ctl_Data = Get_DJI_RC_Data_Address();
	
	//裁判系统数据获取
	Chassis_Data_Init->Chassis_Judge_Msg_Get = Get_Judge_Msg();
	//底盘最大速度获取
	Chassis_Data_Init->Chassis_X_Max_Speed = CHASSIS_X_MAX_SPEED;
	Chassis_Data_Init->Chassis_Y_Max_Speed = CHASSIS_Y_MAX_SPEED;
	Chassis_Data_Init->Chassis_WZ_Speed_Set = CHASSIS_WZ_MAX_SPEED;
	Chassis_Data_Init->Chassis_WZ_Spin_Max_Speed = CHASSIS_WZ_SPIN_MAX_SPEED;

}

float Ramp_Calc(float Min,float Max,float Setup_Time,float Current_Time)
{
		float Out = 0;
		if(Max>Min)
		{
			if(Current_Time>=Setup_Time)
				Out = Max;
			else if(Current_Time>=0&&Current_Time<Setup_Time)
				Out = (Max-Min)/Setup_Time*Current_Time+Min;
		}
		else
		{
				if(Current_Time>=Setup_Time)
					Out = Min;
				else if(Current_Time>=0&&Current_Time<Setup_Time)
					Out = Min-(Min-Max)/Setup_Time*Current_Time;
		}
		return Out;
}

void Rc_Data_To_XY_Speed_Set(Chassis_t* Chassis_Rc,float* Speed_X,float* Speed_Y)
{
		//遥控器通道值转化为速度设定值
		float Chassis_X_Rc_Speed_Set = (float)RC_DeadBand_Limit(Chassis_Rc->Chassis_RC_Ctl_Data->rc.ch0,CHASSIS_RC_DEADLINE)/660*Chassis_Rc->Chassis_X_Max_Speed;
		float Chassis_Y_Rc_Speed_Set = (float)RC_DeadBand_Limit(Chassis_Rc->Chassis_RC_Ctl_Data->rc.ch1,CHASSIS_RC_DEADLINE)/660*Chassis_Rc->Chassis_Y_Max_Speed;
		//按键计时
		if(Chassis_Rc->Chassis_RC_Ctl_Data->key.v & CHASSIS_FORWARD_KEY)
		{
				if(Chassis_Rc->Chassis_Forward_Key_Time<CHASSIS_SPEED_SETUP_TIME)
						Chassis_Rc->Chassis_Forward_Key_Time++;
		}
		else
		{
				if(Chassis_Rc->Chassis_Forward_Key_Time>0)
						Chassis_Rc->Chassis_Forward_Key_Time--;
		}
		
		if(Chassis_Rc->Chassis_RC_Ctl_Data->key.v & CHASSIS_BACKWARD_KEY)
		{
				if(Chassis_Rc->Chassis_Backward_Key_Time<CHASSIS_SPEED_SETUP_TIME)
						Chassis_Rc->Chassis_Backward_Key_Time++;
		}
		else
		{
				if(Chassis_Rc->Chassis_Backward_Key_Time>0)
						Chassis_Rc->Chassis_Backward_Key_Time--;
		}
		
		if(Chassis_Rc->Chassis_RC_Ctl_Data->key.v & CHASSIS_MOVE_LEFT_KEY)
		{
				if(Chassis_Rc->Chassis_Move_Left_Key_Time<CHASSIS_SPEED_SETUP_TIME)
						Chassis_Rc->Chassis_Move_Left_Key_Time++;
		}
		else
		{
				if(Chassis_Rc->Chassis_Move_Left_Key_Time>0)
						Chassis_Rc->Chassis_Move_Left_Key_Time--;
		}
		
		if(Chassis_Rc->Chassis_RC_Ctl_Data->key.v & CHASSIS_MOVE_RIGHT_KEY)
		{
				if(Chassis_Rc->Chassis_Move_Right_Key_Time<CHASSIS_SPEED_SETUP_TIME)
						Chassis_Rc->Chassis_Move_Right_Key_Time++;
		}
		else
		{
				if(Chassis_Rc->Chassis_Move_Right_Key_Time>0)
						Chassis_Rc->Chassis_Move_Right_Key_Time--;
		}
		//按键斜波函数，防止启动停止过快导致漂移
		float Chassis_X_Key_Speed_Set = Ramp_Calc(0,Chassis_Rc->Chassis_Y_Max_Speed,1000,Chassis_Rc->Chassis_Forward_Key_Time)
																	 +Ramp_Calc(0,-Chassis_Rc->Chassis_Y_Max_Speed,1000,Chassis_Rc->Chassis_Backward_Key_Time);
		float Chassis_Y_Key_Speed_Set = Ramp_Calc(0,Chassis_Rc->Chassis_Y_Max_Speed,1000,Chassis_Rc->Chassis_Move_Left_Key_Time)
																	 +Ramp_Calc(0,-Chassis_Rc->Chassis_Y_Max_Speed,1000,Chassis_Rc->Chassis_Move_Right_Key_Time);
		//按键操作的优先级高
		if(Chassis_X_Key_Speed_Set)
			*Speed_X = Chassis_X_Key_Speed_Set;
		else
			*Speed_X = Chassis_X_Rc_Speed_Set;
		
		if(Chassis_Y_Key_Speed_Set)
			*Speed_Y = Chassis_Y_Key_Speed_Set;
		else
			*Speed_Y = Chassis_Y_Rc_Speed_Set;
}

void Rc_Data_To_WZ_Speed_Set(Chassis_t* Chassis_Rc,float* Speed_WZ)
{
		//遥控器通道值转化为速度设定值
		float Chassis_WZ_Rc_Speed_Set = (float)RC_DeadBand_Limit(Chassis_Rc->Chassis_RC_Ctl_Data->rc.ch4,CHASSIS_RC_DEADLINE)/660*Chassis_Rc->Chassis_WZ_Max_Speed;
		//按键计时
		if(Chassis_Rc->Chassis_RC_Ctl_Data->key.v & CHASSIS_TURN_LEFT_KEY)
		{
				if(Chassis_Rc->Chassis_Turn_Left_Key_Time<CHASSIS_SPEED_SETUP_TIME)
						Chassis_Rc->Chassis_Turn_Left_Key_Time++;
		}
		else
		{
				if(Chassis_Rc->Chassis_Turn_Left_Key_Time>0)
						Chassis_Rc->Chassis_Turn_Left_Key_Time--;
		}
		
		if(Chassis_Rc->Chassis_RC_Ctl_Data->key.v & CHASSIS_TURN_RIGHT_KEY)
		{
				if(Chassis_Rc->Chassis_Turn_Right_Key_Time<CHASSIS_SPEED_SETUP_TIME)
						Chassis_Rc->Chassis_Turn_Right_Key_Time++;
		}
		else
		{
				if(Chassis_Rc->Chassis_Turn_Right_Key_Time>0)
						Chassis_Rc->Chassis_Turn_Right_Key_Time--;
		}	
		//按键斜波函数，防止启动停止过快导致漂移
		float Chassis_WZ_Key_Speed_Set = Ramp_Calc(0,Chassis_Rc->Chassis_WZ_Max_Speed,1000,Chassis_Rc->Chassis_Turn_Left_Key_Time)
																	  +Ramp_Calc(0,-Chassis_Rc->Chassis_WZ_Max_Speed,1000,Chassis_Rc->Chassis_Turn_Right_Key_Time);
		//按键操作的优先级高
		if(Chassis_WZ_Key_Speed_Set)
			*Speed_WZ = Chassis_WZ_Key_Speed_Set;
		else
			*Speed_WZ = Chassis_WZ_Rc_Speed_Set;
}

void Chassis_Mode_Zero(Chassis_t* Chassis_Zero_Set)
{
			Chassis_Zero_Set->Chassis_X_Speed_Set = 0;
			Chassis_Zero_Set->Chassis_Y_Speed_Set = 0;
			Chassis_Zero_Set->Chassis_WZ_Speed_Set = 0;
}

void Chassis_Mode_Motionless(Chassis_t* Chassis_Motionless_Set)
{
			Chassis_Motionless_Set->Chassis_X_Speed_Set = 0;
			Chassis_Motionless_Set->Chassis_Y_Speed_Set = 0;
			Chassis_Motionless_Set->Chassis_WZ_Speed_Set = 0;
}

void Chassis_Mode_Rc(Chassis_t* Chassis_Rc_Mode_Set)
{
			fp32 X_Speed_Set = 0;
			fp32 Y_Speed_Set = 0;
			fp32 WZ_Speed_Set = 0;
			Rc_Data_To_XY_Speed_Set(Chassis_Rc_Mode_Set,&X_Speed_Set,&Y_Speed_Set);
			Rc_Data_To_WZ_Speed_Set(Chassis_Rc_Mode_Set,&WZ_Speed_Set);
	
			Chassis_Rc_Mode_Set->Chassis_X_Speed_Set = X_Speed_Set;
			Chassis_Rc_Mode_Set->Chassis_Y_Speed_Set = Y_Speed_Set;
			Chassis_Rc_Mode_Set->Chassis_WZ_Speed_Set = WZ_Speed_Set;
		
}

void Chassis_Mode_Follow_Gimbal(Chassis_t* Follow_Gimbal_Set)
{
			fp32 X_Speed_Set = 0;
			fp32 Y_Speed_Set = 0;
			fp32 WZ_Speed_Set = 0;
			Rc_Data_To_XY_Speed_Set(Follow_Gimbal_Set,&X_Speed_Set,&Y_Speed_Set);
			
			//差值死区限制
			float Angle_TM = 0;
			if(fabs(*Follow_Gimbal_Set->Chassis_Follow_Gimbal_Angle_TM) > 0.02)
					Angle_TM = *Follow_Gimbal_Set->Chassis_Follow_Gimbal_Angle_TM;
			else
					Angle_TM = 0;
			
			fp32 sin_yaw = arm_sin_f32(-Angle_TM);
      fp32 cos_yaw = arm_cos_f32(-Angle_TM);
			Follow_Gimbal_Set->Chassis_X_Speed_Set = cos_yaw * X_Speed_Set + sin_yaw * Y_Speed_Set;
			Follow_Gimbal_Set->Chassis_Y_Speed_Set = -sin_yaw * X_Speed_Set + cos_yaw * Y_Speed_Set;						
			Follow_Gimbal_Set->Chassis_WZ_Speed_Set = -pid_calc_circular(&Follow_Gimbal_Set->Chassis_Angle_Pid,Angle_TM,0,PI,-PI);
}


void Chassis_Mode_Spin(Chassis_t* Chassis_Spin_Set)
{
			fp32 X_Speed_Set = 0;
			fp32 Y_Speed_Set = 0;
			fp32 WZ_Speed_Set = 0;
			Rc_Data_To_XY_Speed_Set(Chassis_Spin_Set,&X_Speed_Set,&Y_Speed_Set);
	
			Chassis_Spin_Set->Chassis_WZ_Speed_Set = 0;
	
	
			if(Chassis_Spin_Set->Chassis_RC_Ctl_Data->key.v & CHASSIS_TURN_SPEED_UP_KEY)
			{
				
					if(Chassis_Spin_Set->Chassis_Mode == CHASSIS_SPIN_RIGHT)
					{
						WZ_Speed_Set = Chassis_Spin_Set->Chassis_WZ_Spin_Max_Speed;
					}
					else if(Chassis_Spin_Set->Chassis_Mode == CHASSIS_SPIN_LEFT)
					{
						WZ_Speed_Set = -Chassis_Spin_Set->Chassis_WZ_Spin_Max_Speed;
					}
			}
			else
			{
					if(Chassis_Spin_Set->Chassis_Mode == CHASSIS_SPIN_RIGHT)
					{
						WZ_Speed_Set = Chassis_Spin_Set->Chassis_WZ_Max_Speed;
					}
					else if(Chassis_Spin_Set->Chassis_Mode == CHASSIS_SPIN_LEFT)
					{
						WZ_Speed_Set = -Chassis_Spin_Set->Chassis_WZ_Max_Speed;
					}
			}
			
			fp32 sin_yaw = arm_sin_f32(-*Chassis_Spin_Set->Chassis_Follow_Gimbal_Angle_TM);
      fp32 cos_yaw = arm_cos_f32(-*Chassis_Spin_Set->Chassis_Follow_Gimbal_Angle_TM);
			Chassis_Spin_Set->Chassis_X_Speed_Set = cos_yaw * X_Speed_Set + sin_yaw * Y_Speed_Set;
			Chassis_Spin_Set->Chassis_Y_Speed_Set = -sin_yaw * X_Speed_Set + cos_yaw * Y_Speed_Set;
			Chassis_Spin_Set->Chassis_WZ_Speed_Set = WZ_Speed_Set;		

}

void Chassis_Mode_Translate_Move(Chassis_t* Chassis_Translate_Set)
{
		static int Translate_Rev_Flag = 1;
		if((Chassis_Translate_Set->Chassis_Translate_Speed < Chassis_Translate_Max_Speed)&&(Chassis_Translate_Set->Translate_Time_Count==0))
				Chassis_Translate_Set->Chassis_Translate_Speed += Chassis_Translate_Delta_Speed;
		else 
		{
				if(Chassis_Translate_Set->Translate_Time_Count != Chassis_Translate_Max_Speed_Time)
						Chassis_Translate_Set->Translate_Time_Count++;
				else
				{
						if(Chassis_Translate_Set->Chassis_Translate_Speed > 0)
								Chassis_Translate_Set->Chassis_Translate_Speed -= Chassis_Translate_Delta_Speed;
						else
						{
								Chassis_Translate_Set->Translate_Time_Count=0;
								Translate_Rev_Flag = -Translate_Rev_Flag;
						}
				}
		}
		

		Chassis_Translate_Set->Chassis_X_Speed_Set = Translate_Rev_Flag*Chassis_Translate_Set->Chassis_Translate_Speed;
}

void Chassis_Speed_To_Wheel_Speed(Chassis_t* Chassis_Speed_Convert)
{
		Chassis_Speed_Convert->Chassis_Motor_Speed_Set[0] = Chassis_Speed_Convert->Chassis_X_Speed_Set - Chassis_Speed_Convert->Chassis_Y_Speed_Set - Chassis_Speed_Convert->Chassis_WZ_Speed_Set;
		Chassis_Speed_Convert->Chassis_Motor_Speed_Set[1] = Chassis_Speed_Convert->Chassis_X_Speed_Set + Chassis_Speed_Convert->Chassis_Y_Speed_Set - Chassis_Speed_Convert->Chassis_WZ_Speed_Set;
		Chassis_Speed_Convert->Chassis_Motor_Speed_Set[2] = -Chassis_Speed_Convert->Chassis_X_Speed_Set + Chassis_Speed_Convert->Chassis_Y_Speed_Set - Chassis_Speed_Convert->Chassis_WZ_Speed_Set;
		Chassis_Speed_Convert->Chassis_Motor_Speed_Set[3] = -Chassis_Speed_Convert->Chassis_X_Speed_Set - Chassis_Speed_Convert->Chassis_Y_Speed_Set - Chassis_Speed_Convert->Chassis_WZ_Speed_Set;	
}

/*****底盘控制数据更新函数*****/
void Chassis_Control_Data_Get(Chassis_t* Chassis_Control_Data)
{
		switch(Chassis_Control_Data->Chassis_Mode)
		{
			case CHASSIS_ZERO:
				Chassis_Mode_Zero(Chassis_Control_Data);
			break;
			
			case CHASSIS_MOTIONLESS:
				Chassis_Mode_Motionless(Chassis_Control_Data);			
			break;
			
			case CHASSIS_RC_CONTROL:	
					Chassis_Mode_Rc(Chassis_Control_Data);
			break;
			
			case CHASSIS_FOLLOW_GIMBAL:
				Chassis_Mode_Follow_Gimbal(Chassis_Control_Data);
			break;		
			
			case CHASSIS_SPIN_LEFT:	
			case CHASSIS_SPIN_RIGHT:	
				Chassis_Mode_Spin(Chassis_Control_Data);
			
			case CHASSIS_TRANSLATE:
				Chassis_Mode_Translate_Move(Chassis_Control_Data);
			break;
			
		}
		
		Chassis_Speed_To_Wheel_Speed(Chassis_Control_Data);
			
}

/*****底盘模式更新函数*****/
extern int Check_Motionless_Flag;
void Chassis_Mode_Set(Chassis_t* Chassis_Mode)
{
//			//如果云台处于不动状态那么底盘进入不动状态
//			if(*Return_Gimbal_Mode_Add()==GIMBAL_MOTIONLESS)
//			{
//					Chassis_Mode->Chassis_Mode = CHASSIS_MOTIONLESS;
//			}
//			//否则进行正常的模式判断
//			else
//			{
			
					if(Chassis_Mode->Chassis_RC_Ctl_Data->rc.s2 == RC_SW_DOWN)
						Chassis_Mode->Chassis_Mode = CHASSIS_FIRST_MODE;
					else if(Chassis_Mode->Chassis_RC_Ctl_Data->rc.s2 == RC_SW_MID)
						Chassis_Mode->Chassis_Mode = CHASSIS_SECOND_MODE;
					else if(Chassis_Mode->Chassis_RC_Ctl_Data->rc.s2 == RC_SW_UP)
					{
						if((Chassis_Mode->Chassis_Last_Mode_Switch == RC_SW_MID)||(Chassis_Mode->Chassis_Mode == CHASSIS_MOTIONLESS))
						{
							Chassis_Mode->Chassis_Mode = CHASSIS_THIRD_MODE;
						}
						else
						{
							if((!(Chassis_Mode->Chassis_Last_Mode_Key & CHASSIS_TURN_LEFT_KEY)) && (Chassis_Mode->Chassis_RC_Ctl_Data->key.v & CHASSIS_TURN_LEFT_KEY))
							{
								if((Chassis_Mode->Chassis_Mode == CHASSIS_SPIN_LEFT) || (Chassis_Mode->Chassis_Mode == CHASSIS_SPIN_RIGHT))
								{
									Chassis_Mode->Chassis_Mode = CHASSIS_THIRD_MODE;
								}
								else
								{
									Chassis_Mode->Chassis_Mode = CHASSIS_SPIN_LEFT;
								}	
							}
							else if((!(Chassis_Mode->Chassis_Last_Mode_Key & CHASSIS_TURN_RIGHT_KEY)) && (Chassis_Mode->Chassis_RC_Ctl_Data->key.v & CHASSIS_TURN_RIGHT_KEY))
							{
								if((Chassis_Mode->Chassis_Mode == CHASSIS_SPIN_LEFT) || (Chassis_Mode->Chassis_Mode == CHASSIS_SPIN_RIGHT))
								{
									Chassis_Mode->Chassis_Mode = CHASSIS_THIRD_MODE;
								}
								else
								{
									Chassis_Mode->Chassis_Mode = CHASSIS_SPIN_RIGHT;
								}	
							}
							else if((!(Chassis_Mode->Chassis_Last_Mode_Key & CHASSIS_TRANSLATE)) && (Chassis_Mode->Chassis_RC_Ctl_Data->key.v & CHASSIS_TRANSLATE))
							{
								if(Chassis_Mode->Chassis_Mode == CHASSIS_TRANSLATE)
								{
									Chassis_Mode->Chassis_Mode = CHASSIS_THIRD_MODE;
								}
								else
								{
									Chassis_Mode->Chassis_Mode = CHASSIS_TRANSLATE;
									Chassis_Mode->Translate_Time_Count = 0;
									Chassis_Mode->Chassis_Translate_Speed = 0;
								}	
							}
							
						}
					}
//			}
			
	Chassis_Mode->Chassis_Last_Mode_Key = Chassis_Mode->Chassis_RC_Ctl_Data->key.v;
	Chassis_Mode->Chassis_Last_Mode_Switch = Chassis_Mode->Chassis_RC_Ctl_Data->rc.s2;
	
}

/*****底盘PID更新函数*****/
void Chassis_PID_Calculate_Data(Chassis_t* Chassis_PID)
{
	if(Chassis_PID->Chassis_Mode == CHASSIS_ZERO)
	{
		Chassis_PID->Chassis_Motor_Curent_Send[0] = 0;
		Chassis_PID->Chassis_Motor_Curent_Send[1] = 0;
		Chassis_PID->Chassis_Motor_Curent_Send[2] = 0;
		Chassis_PID->Chassis_Motor_Curent_Send[3] = 0;
	}
	else
	{
		Chassis_PID->Chassis_Motor_Curent_Send[0] = pid_calc_linear(&Chassis_PID->Chassis_Motor_Pid[0],Chassis_PID->Chassis_Motor_Speed_Get[0],Chassis_PID->Chassis_Motor_Speed_Set[0]);
		Chassis_PID->Chassis_Motor_Curent_Send[1] = pid_calc_linear(&Chassis_PID->Chassis_Motor_Pid[1],Chassis_PID->Chassis_Motor_Speed_Get[1],Chassis_PID->Chassis_Motor_Speed_Set[1]);
		Chassis_PID->Chassis_Motor_Curent_Send[2] = pid_calc_linear(&Chassis_PID->Chassis_Motor_Pid[2],Chassis_PID->Chassis_Motor_Speed_Get[2],Chassis_PID->Chassis_Motor_Speed_Set[2]);
		Chassis_PID->Chassis_Motor_Curent_Send[3] = pid_calc_linear(&Chassis_PID->Chassis_Motor_Pid[3],Chassis_PID->Chassis_Motor_Speed_Get[3],Chassis_PID->Chassis_Motor_Speed_Set[3]);
	}
}

/*****超级电容数据更新函数*****/
//void Chassis_Cap_Data_Update(Chassis_Cap_Msg_t* Chassis_Cap_Msg_Update)
//{
//	Chassis_Cap_Msg_Update->Chassis_Cap_Power_Percent = ((float)(Chassis_Cap_Msg_Update->Chassis_Cap_Msg_Get->Voilt_C) / (float)(Chassis_Cap_Msg_Update->Chassis_Cap_Msg_Get->Voilt_In) * 100.00f);
//	if(Chassis_Cap_Msg_Update->Chassis_Cap_Msg_Get->Target_power != Chassis_Cap_Msg_Update->Chassis_Cap_Power_Set)
//	CAN1_Super_C_Send(0x210,Chassis_Cap_Msg_Update->Chassis_Cap_Power_Set);	
//	Chassis_Cap_Msg_Update->Chassis_Cap_Power_Set = Chassis_Cap_Msg_Update->Chassis_Cap_Msg_Get->Target_power;
//	//Chassis_Cap_Msg_Update
//}

/*****底盘数据更新函数*****/
void Chassis_Data_Update(Chassis_t* Chassis_Update)
{
	//电机数据更新
	Chassis_Update->Chassis_Motor_Speed_Get[0]=Chassis_Update->Chassis_Motor_Msg_Get[0]->Speed*CHASSIS_3508_RPM_TO_WHEEL_SPEED;
	Chassis_Update->Chassis_Motor_Speed_Get[1]=Chassis_Update->Chassis_Motor_Msg_Get[1]->Speed*CHASSIS_3508_RPM_TO_WHEEL_SPEED;
	Chassis_Update->Chassis_Motor_Speed_Get[2]=Chassis_Update->Chassis_Motor_Msg_Get[2]->Speed*CHASSIS_3508_RPM_TO_WHEEL_SPEED;
	Chassis_Update->Chassis_Motor_Speed_Get[3]=Chassis_Update->Chassis_Motor_Msg_Get[3]->Speed*CHASSIS_3508_RPM_TO_WHEEL_SPEED;
	//裁判系统数据更新
	Chassis_Update->Chassis_Power_Data_Get = (float)Chassis_Update->Chassis_Judge_Msg_Get->Judge_power_heat_data.chassis_power;
	Chassis_Update->Chassis_Heat_Data_Get = (int)Chassis_Update->Chassis_Judge_Msg_Get->Judge_power_heat_data.chassis_power_buffer;
	Chassis_Update->Chassis_Max_Power_Data_Get = (int)Chassis_Update->Chassis_Judge_Msg_Get->Judge_game_robot_status.max_chassis_power;
	if(Chassis_Update->Chassis_Heat_Data_Get > 60)
	{
			Chassis_Update->Chassis_Max_Heat_Data_Get = Chassis_Max_Heat_Feipo;
	}
	else
	{
			Chassis_Update->Chassis_Max_Heat_Data_Get = Chassis_Max_Heat_Normal;		
	}
	//超级电容数据更新
	
}

void Chassis_Task(void *pvParameters)
{
	vTaskDelay(100);
	Chassis_Init(&Chassis);
	Chassis_Add_Variable(&Chassis);
	
	while(1)
	{
			Chassis_Mode_Set(&Chassis);
			Chassis_Data_Update(&Chassis);
			Chassis_Control_Data_Get(&Chassis);
			Chassis_PID_Calculate_Data(&Chassis);
			CAN2_Motor_Control(CHASSIS_MOTOR_ALL_ID,(int16_t)Chassis.Chassis_Motor_Curent_Send[0],
															 (int16_t)Chassis.Chassis_Motor_Curent_Send[1],
															 (int16_t)Chassis.Chassis_Motor_Curent_Send[2],
															 (int16_t)Chassis.Chassis_Motor_Curent_Send[3]);

			vTaskDelay(1);			
	}
}

Chassis_Mode_t* Return_Chassis_Mode_Add(void)
{
		return &Chassis.Chassis_Mode;
}
