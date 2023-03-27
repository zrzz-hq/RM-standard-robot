/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    shoot_task.c
	* @brief   射击控制任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 已经完成读取裁判系统热量闭环
	* @Question 	 摩擦轮PID可能需要调整，需要获取摩擦力执行曲线【4.9】
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     4-9-2021     	 JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe 射击控制任务
*/
#include <math.h>

#include "shoot_task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "data_task.h"

#include "gpio.h"

#include "can.h"
#include "pid.h"

Shoot_t Shoot;


void Shoot_Add_Variable(Shoot_t* Shoot_Variable)
{
		Add_Variable_By_Name_In_Task("Shoot Left Fric Speed Kp",&Shoot_Variable->Fric_Motor_Pid[0].kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Shoot Left Fric Speed Ki",&Shoot_Variable->Fric_Motor_Pid[0].ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Shoot Left Fric Speed Kd",&Shoot_Variable->Fric_Motor_Pid[0].kd,VAR_TYPE_FP32,0);
	
		Add_Variable_By_Name_In_Task("Shoot Right Fric Speed Kp",&Shoot_Variable->Fric_Motor_Pid[1].kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Shoot Right Fric Speed Ki",&Shoot_Variable->Fric_Motor_Pid[1].ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Shoot Right Fric Speed Kd",&Shoot_Variable->Fric_Motor_Pid[1].kd,VAR_TYPE_FP32,0);
	
		Add_Variable_By_Name_In_Task("Shoot Trigger Speed Kp",&Shoot_Variable->Trigger_Motor_Pid.kp,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Shoot Trigger Speed Ki",&Shoot_Variable->Trigger_Motor_Pid.ki,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Shoot Trigger Speed Kd",&Shoot_Variable->Trigger_Motor_Pid.kd,VAR_TYPE_FP32,0);
	
		Add_Variable_By_Name_In_Task("Shoot Trigger Speed Set",&Shoot_Variable->Trigger_Motor_Speed_Set,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Shoot Fric Speed Set",&Shoot_Variable->Fric_Motor_Speed_Set,VAR_TYPE_FP32,1);
	
		Add_Variable_By_Name_In_Task("Shoot Trigger Speed",&Shoot_Variable->Fric_Motor_Speed_Get[0],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Shoot Fric1 Speed",&Shoot_Variable->Fric_Motor_Speed_Get[1],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Shoot Fric2 Speed",&Shoot_Variable->Fric_Motor_Speed_Get,VAR_TYPE_FP32,1);
	
		Add_Variable_By_Name_In_Task("Shoot Trigger Given Current",&Shoot_Variable->Trigger_Motor_Current_Send,VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Shoot Fric1 Given Current",&Shoot_Variable->Fric_Motor_Current_Send[0],VAR_TYPE_FP32,1);
		Add_Variable_By_Name_In_Task("Shoot Fric2 Given Current",&Shoot_Variable->Fric_Motor_Current_Send[1],VAR_TYPE_FP32,1);
		
		Add_Variable_By_Name_In_Task("Shoot Mode",&Shoot_Variable->Shoot_Mode,VAR_TYPE_UINT8,1);
		
		Add_Variable_By_Name_In_Task("Fric Reverse",&Shoot_Variable->Fric_Reverse_Flag,VAR_TYPE_UINT8,0);
		Add_Variable_By_Name_In_Task("Bullets Per Rotation",&Shoot_Variable->Bullets_Per_Rotation,VAR_TYPE_UINT8,0);
		Add_Variable_By_Name_In_Task("Fric Wheel Diameter",&Shoot_Variable->Fric_Wheel_Diameter,VAR_TYPE_FP32,0);
		Add_Variable_By_Name_In_Task("Load Bullet Speed",&Shoot_Variable->Load_Bullet_Speed,VAR_TYPE_INT8,0);
		Add_Variable_By_Name_In_Task("Unload Bullet Speed",&Shoot_Variable->Unload_Bullet_Speed,VAR_TYPE_INT8,0);
		Add_Variable_By_Name_In_Task("Shoot Start Time Limit",&Shoot_Variable->Shoot_Start_Time_Limit,VAR_TYPE_UINT16,0);
		Add_Variable_By_Name_In_Task("Shoot Bullet Time Limit",&Shoot_Variable->Shoot_Bullet_Time_Limit,VAR_TYPE_UINT16,0);
		Add_Variable_By_Name_In_Task("Shoot Stall Time Limit",&Shoot_Variable->Shoot_Stall_Time_Limit,VAR_TYPE_UINT16,0);
		Add_Variable_By_Name_In_Task("Trigger Motor Min Speed",&Shoot_Variable->Trigger_Motor_Min_Speed,VAR_TYPE_INT8,0);
	
}

//float Shoot_Freq_To_Trigger_Motor_Speed(uint8_t Convert_Factor,uint8_t Freq)
//{
//		return Freq/Convert_Factor*60.0;
//}

void Shoot_Mode_Set(Shoot_t* Mode_Set)
{
		uint8_t Shoot_Mode_Switch = Mode_Set->Shoot_RC_Ctl_Data->rc.s1;
		uint8_t Shoot_Mode_Key = Mode_Set->Shoot_RC_Ctl_Data->key.v;
		uint8_t Shoot_Mouse_Key = Mode_Set->Shoot_RC_Ctl_Data->mouse.press_l;
	
		uint8_t Shoot_Start_Stop_Signal = (Shoot_Mode_Switch==RC_SW_UP&&Mode_Set->Last_Shoot_Mode_Switch!=RC_SW_UP)
																			||(Shoot_Mode_Key&SHOOT_START_STOP_KEY&&!(Mode_Set->Last_Shoot_Mode_Key&SHOOT_START_STOP_KEY));
	
	
		//在停止射击状态下上拨,或者按下R键，进入开始射击状态
		if(Mode_Set->Shoot_Mode == SHOOT_STOP)
		{
				Laser_Off();
				if(Shoot_Start_Stop_Signal)
						Mode_Set->Shoot_Mode = SHOOT_START;
		}
		else 
		{
				Laser_On();
				//在除了停止射击之外的其他状态，再次上拨或者按下R键，进入停止射击状态
				if(Shoot_Start_Stop_Signal)
				{
						Mode_Set->Shoot_Mode = SHOOT_STOP;
				}
				//否则进行正常的模式判断
				else
				{
						/****************信号产生部分*****************/
						//统计按下鼠标左键和下拨时间
						if(Shoot_Mode_Switch==RC_SW_DOWN||Shoot_Mouse_Key)
						{
								if(Mode_Set->Shoot_Mode_Switch_Down_Time<SHOOT_MODE_SWITCH_DOWN_TIME_LIMIT)
										Mode_Set->Shoot_Mode_Switch_Down_Time++;
						}
						else 
								Mode_Set->Shoot_Mode_Switch_Down_Time=0;
						//长按鼠标左键或这一直处于下拨状态，产生连发信号
						uint8_t Shoot_Bullet_No_Break_Signal = Mode_Set->Shoot_Mode_Switch_Down_Time==SHOOT_MODE_SWITCH_DOWN_TIME_LIMIT;
						//点击鼠标左键，或者下拨一次，产生单发信号
						uint8_t Shoot_Bullet_Once_Signal = (Shoot_Mode_Switch==RC_SW_DOWN&&Mode_Set->Last_Shoot_Mode_Switch!=RC_SW_DOWN)
																				||(Shoot_Mouse_Key&&!Mode_Set->Last_Shoot_Mouse_Key);
						//每次检测到单发信号，需要发射的弹丸数目加一
						if(Shoot_Bullet_Once_Signal)
							Mode_Set->Need_Shoot_Count++;
						//每次检测到连发信号，需要发射的弹丸数目置一
						if(Shoot_Bullet_No_Break_Signal)
							Mode_Set->Need_Shoot_Count=1;
						
						//电机堵转信号
						uint8_t Trigger_Motor_Stall_Signal = 0;
						//如果电机速度小于一定值持续一段时间，说明电机堵转了
						if(fabs(Mode_Set->Trigger_Motor_Speed_Get)<Mode_Set->Trigger_Motor_Min_Speed
									&&Mode_Set->Trigger_Motor_Speed_Set!=0)
						{
								if(Mode_Set->Trigger_Motor_Low_Speed_Time<Mode_Set->Trigger_Motor_Low_Speed_Time_Limit)
										Mode_Set->Trigger_Motor_Low_Speed_Time++;
								else
										Trigger_Motor_Stall_Signal = 1;
						}
						else
						{
								Mode_Set->Trigger_Motor_Low_Speed_Time = 0;
						}
						/****************状态机转移部分*****************/
						
						if(Mode_Set->Shoot_Mode == SHOOT_START)
						{
								//在开始射击状态下，如果弹丸到达微动开关，那么进入准备射击状态
								if(Mode_Set->Shoot_Key == SHOOT_KEY_ON)
										Mode_Set->Shoot_Mode = SHOOT_READY;
								//如果很长时间都没有加载上弹丸，判断电机是否堵转
								else if(Mode_Set->Shoot_Start_Time>Mode_Set->Shoot_Start_Time_Limit)
								{
										//如果电机堵转了，说明卡弹了，进入回拨状态
										if(Trigger_Motor_Stall_Signal)
												Mode_Set->Shoot_Mode = SHOOT_STALL;
										//否则表明没有弹丸了
										else
												Mode_Set->No_Bullet_Flag = 1;
								}
						}
						//在准备射击状态下，如果需要发射的弹丸数目大于0，进入发射状态
						else if(Mode_Set->Shoot_Mode == SHOOT_READY&&Mode_Set->Need_Shoot_Count)
						{
								Mode_Set->Shoot_Mode = SHOOT_BULLET;
						}
						//在卡弹状态下，如果回拨时间达到设定值，重新回到准备开始发射状态
						else if(Mode_Set->Shoot_Mode == SHOOT_STALL
									&&Mode_Set->Shoot_Stall_Time==Mode_Set->Shoot_Stall_Time_Limit)
						{
									Mode_Set->Shoot_Mode = SHOOT_START;
						}
						else if(Mode_Set->Shoot_Mode == SHOOT_BULLET)
						{
							//在发射弹丸状态，如果弹丸射出，进入开始发射状态，需要发射的弹丸数目减一
							if(Mode_Set->Shoot_Key==SHOOT_KEY_OFF&&Mode_Set->Last_Shoot_Key==SHOOT_KEY_ON)
							{
									Mode_Set->Shoot_Mode=SHOOT_START;
									Mode_Set->Need_Shoot_Count--;
							}
							//在发射弹丸状态下，如果长时间未发弹，检查电机是否卡住了
							if(Mode_Set->Shoot_Bullet_Time>Mode_Set->Shoot_Bullet_Time_Limit)
							{
									//如果电机卡住了，进入回拨模式
									if(Trigger_Motor_Stall_Signal)
											Mode_Set->Shoot_Mode=SHOOT_STALL;
									//否则表明没有弹丸了
									else
											Mode_Set->No_Bullet_Flag = 1;
							}
						}
				}
		}
		
		//如果处于发射弹丸状态，统计发射时间，从其他状态切入发射弹丸状态，发射时间清零
		if(Mode_Set->Shoot_Mode==SHOOT_BULLET)
		{
				if(Mode_Set->Last_Shoot_Mode!=SHOOT_BULLET)
						Mode_Set->Shoot_Bullet_Time=0;
				else
						Mode_Set->Shoot_Bullet_Time++;
		}
		//如果处于卡弹回拨状态，统计卡弹回拨时间，从其他状态切入卡弹回拨状态，卡弹回拨时间清零
		if(Mode_Set->Shoot_Mode==SHOOT_STALL)
		{
				if(Mode_Set->Last_Shoot_Mode!=SHOOT_STALL)
						Mode_Set->Shoot_Stall_Time=0;
				else
						Mode_Set->Shoot_Stall_Time++;
		}
		//如果处于开始射击状态，统计开始射击状态的时间，从其他状态切入开始射击状态，时间清零
		if(Mode_Set->Shoot_Mode==SHOOT_START)
		{
				if(Mode_Set->Last_Shoot_Mode!=SHOOT_START)
						Mode_Set->Shoot_Start_Time=0;
				else
						Mode_Set->Shoot_Start_Time++;
		}
		//从其他模式切入停止射击模式，需要发射的数目置零
		if(Mode_Set->Shoot_Mode==SHOOT_STOP&&Mode_Set->Last_Shoot_Mode!=SHOOT_STOP)
		{
				Mode_Set->Need_Shoot_Count = 0;
		}
			
		Mode_Set->Last_Shoot_Mode_Switch = Shoot_Mode_Switch;
		Mode_Set->Last_Shoot_Key = Mode_Set->Shoot_Key;
		Mode_Set->Last_Shoot_Mode_Key = Shoot_Mode_Key;
		Mode_Set->Last_Shoot_Mouse_Key = Shoot_Mouse_Key;
		Mode_Set->Last_Shoot_Mode = Mode_Set->Shoot_Mode;
}


void Shoot_Pid_Calc(Shoot_t* Pid_Calc)
{
		if(Pid_Calc->Shoot_Mode!=SHOOT_STOP)
		{
				int8_t Factor = Pid_Calc->Fric_Reverse_Flag?-1:1;
				Pid_Calc->Fric_Motor_Current_Send[0] = pid_calc_linear(&Pid_Calc->Fric_Motor_Pid[0],Pid_Calc->Fric_Motor_Speed_Get[0],Factor*-Pid_Calc->Fric_Motor_Speed_Set);
				Pid_Calc->Fric_Motor_Current_Send[1] = pid_calc_linear(&Pid_Calc->Fric_Motor_Pid[1],Pid_Calc->Fric_Motor_Speed_Get[1],Factor*Pid_Calc->Fric_Motor_Speed_Set);
				Pid_Calc->Trigger_Motor_Current_Send = pid_calc_linear(&Pid_Calc->Trigger_Motor_Pid,Pid_Calc->Trigger_Motor_Speed_Get,Pid_Calc->Trigger_Motor_Speed_Set);
		}
		else
		{
				Pid_Calc->Fric_Motor_Current_Send[0] = 0;
				Pid_Calc->Fric_Motor_Current_Send[1] = 0;
				Pid_Calc->Trigger_Motor_Current_Send = 0;
		}
		
		
}

void Shoot_Init(Shoot_t* Data_Init)
{
		//电机数据获取
		Data_Init->Fric_Motor_Msg_Get[0] = Get_DJI_Motor_Data(CAN1_RX,FRIC_MOTOR_LEFT_ID); 
		Data_Init->Fric_Motor_Msg_Get[1] = Get_DJI_Motor_Data(CAN1_RX,FRIC_MOTOR_RIGHT_ID); 
		Data_Init->Trigger_Motor_Msg_Get = Get_DJI_Motor_Data(CAN1_RX,TRIGGER_MOTOR_ID);
		
		Data_Init->Shoot_Mode = SHOOT_STOP;
		//遥控器数据获取
		Data_Init->Shoot_RC_Ctl_Data = Get_DJI_RC_Data_Address();
		//裁判系统数据获取
		Data_Init->Shoot_Judge_Msg_Get = Get_Judge_Msg();
		//初始化PID
		pid_init(&Data_Init->Fric_Motor_Pid[0],FRIC_MOTOR_LEFT_KP,FRIC_MOTOR_LEFT_KI,FRIC_MOTOR_LEFT_KD,FRIC_MOTOR_LEFT_MAXOUT,FRIC_MOTOR_LEFT_IMAXOUT,PID_POSITON);	
		pid_init(&Data_Init->Fric_Motor_Pid[1],FRIC_MOTOR_RIGHT_KP,FRIC_MOTOR_RIGHT_KI,FRIC_MOTOR_RIGHT_KD,FRIC_MOTOR_RIGHT_MAXOUT,FRIC_MOTOR_RIGHT_IMAXOUT,PID_POSITON);	
		pid_init(&Data_Init->Trigger_Motor_Pid,TRIGGER_MOTOR_KP,TRIGGER_MOTOR_KI,TRIGGER_MOTOR_KD,TRIGGER_MOTOR_MAXOUT,TRIGGER_MOTOR_IMAXOUT,PID_DELTA);	
		
		Data_Init->Bullets_Per_Rotation = BULLETS_PER_ROTATION;
		Data_Init->Fric_Wheel_Diameter = FRIC_WHEEL_DIAMETER;
		Data_Init->Shoot_Bullet_Time_Limit = SHOOT_BULLET_TIME_LIMIT;
		Data_Init->Shoot_Start_Time_Limit = SHOOT_BULLET_TIME_LIMIT;
		Data_Init->Trigger_Motor_Min_Speed = TRIGGER_MOTOR_MIN_SPEED;
		Data_Init->Load_Bullet_Speed = LOAD_BULLET_SPEED;
		Data_Init->Unload_Bullet_Speed = UNLOAD_BULLET_SPEED;
		Data_Init->Shoot_Stall_Time_Limit = SHOOT_STALL_TIME_LIMIT;
		Data_Init->Trigger_Motor_Low_Speed_Time_Limit = TRIGGER_MOTOR_LOW_SPEED_TIME_LIMIT;
}

uint8_t Get_Shoot_Freq_From_Judge_System(Shoot_t* Get_Shoot_Freq)
{
		return 1;
}

float Get_Fric_Speed_From_Judge_System(Shoot_t* Get_Fric_Speed)
{
		return 10;
}


void Shoot_Control_Data_Set(Shoot_t* Control_Data_Set)
{
		if(Control_Data_Set->Shoot_Mode == SHOOT_START)
		{
				Control_Data_Set->Trigger_Motor_Speed_Set = Control_Data_Set->Load_Bullet_Speed;
		}
		else if(Control_Data_Set->Shoot_Mode == SHOOT_READY)
		{
				Control_Data_Set->Trigger_Motor_Speed_Set = 0;
		}
		else if(Control_Data_Set->Shoot_Mode == SHOOT_BULLET)
		{
				uint8_t Freq_Set = Get_Shoot_Freq_From_Judge_System(Control_Data_Set);
				Control_Data_Set->Trigger_Motor_Speed_Set = Freq_Set;
		}
		else if(Control_Data_Set->Shoot_Mode == SHOOT_STALL)
		{
				Control_Data_Set->Trigger_Motor_Speed_Set = Control_Data_Set->Unload_Bullet_Speed;
		}
		
		if(Control_Data_Set->Shoot_Mode != SHOOT_STOP)
		{
				Control_Data_Set->Fric_Motor_Speed_Set = Get_Fric_Speed_From_Judge_System(Control_Data_Set);
		}
}

void Shoot_Data_Update(Shoot_t* Data_Update)
{
		Data_Update->Fric_Motor_Speed_Get[0]=Data_Update->Fric_Motor_Msg_Get[0]->Speed/60*PI*Data_Update->Fric_Wheel_Diameter;
		Data_Update->Fric_Motor_Speed_Get[1]=Data_Update->Fric_Motor_Msg_Get[1]->Speed/60*PI*Data_Update->Fric_Wheel_Diameter;
		Data_Update->Trigger_Motor_Speed_Get=Data_Update->Trigger_Motor_Msg_Get->Speed/60/36*Data_Update->Bullets_Per_Rotation;
		Data_Update->Shoot_Key = Shoot_Key_Read();
	
//		Data_Update->Shoot_Speed_Limit = Data_Update->Shoot_Judge_Msg_Get->Judge_game_robot_status.shooter_heat0_speed_limit;
//		Data_Update->Shoot_Cooling_Rate = Data_Update->Shoot_Judge_Msg_Get->Judge_game_robot_status.shooter_heat0_cooling_rate;
//		Data_Update->Shoot_Cooling_Limit = Data_Update->Shoot_Judge_Msg_Get->Judge_game_robot_status.shooter_heat0_cooling_limit;
//		Data_Update->Shoot_Heat = Data_Update->Shoot_Judge_Msg_Get->Judge_power_heat_data.shooter_heat0;
//		Data_Update->Shoot_Speed = Data_Update->Shoot_Judge_Msg_Get->Judge_shoot_data.bullet_speed;
//		Data_Update->Shoot_Freq = Data_Update->Shoot_Judge_Msg_Get->Judge_shoot_data.bullet_freq;
//		Data_Update->Shoot_Heat_Percent = (((float)Data_Update->Shoot_Heat_Now / (float)Data_Update->Shoot_Heat_Limit) * 100.00f);
//		Data_Update->Shoot_Cool_Percent = (((float)Data_Update->Shoot_Cool_Now / (float)Data_Update->Shoot_Heat_Limit) * 100.00f);
}

//int Shoot_Heat_Limit_Get(float Robot_Heat_Percent,float Robot_Cool_Percent,int Robot_Heat_Limit,int Robot_Cool_Limit)
//{
//	if(Robot_Heat_Percent >= 95.0f)
//	{
//		return 0;
//	}
//	if((Robot_Heat_Percent >= 0) && (Robot_Heat_Percent < 60))
//	{
//		if(Robot_Heat_Limit <=50)
//			return 3;
//		else if(Robot_Heat_Limit <=100)
//			return 8;
//		else if(Robot_Heat_Limit <=150)
//			return 12;
//		else if(Robot_Heat_Limit <=280)
//			return 18;
//		else if(Robot_Heat_Limit <=400)
//			return 26;	
//	}
//	else if((Robot_Heat_Percent >= 60) && (Robot_Heat_Percent <= (100.0f-Robot_Cool_Percent)))
//	{
//		if(Robot_Heat_Limit <=50)
//			return 1;
//		else if(Robot_Heat_Limit <=100)
//			return 2;
//		else if(Robot_Heat_Limit <=150)
//			return 3;
//		else if(Robot_Heat_Limit <=280)
//			return 4;
//		else if(Robot_Heat_Limit <=400)
//			return 5;
//	}
//	else if(Robot_Heat_Percent > (100.0f-Robot_Cool_Percent))
//	{
//		return (Robot_Cool_Limit / 10);
//	}

//	else
//	return 0;
//}

//void Shoot_Judge_Data_Check(Shoot_t* Shoot_Judge,float* Shot_Judge_Speed_Set,float* Shoot_Trigger_Speed_Set)
//{
//	

//	*Shot_Judge_Speed_Set = 9 + (Shoot_Judge->Shoot_Judge_Msg.Shoot_Speed_Limit - 9.0f)/2.3f;
//	*Shoot_Trigger_Speed_Set = Shoot_Heat_Limit_Get(Shoot_Judge->Shoot_Judge_Msg.Shoot_Heat_Percent_Now,Shoot_Judge->Shoot_Judge_Msg.Shoot_Cool_Percent_Now,Shoot_Judge->Shoot_Judge_Msg.Shoot_Heat_Limit,Shoot_Judge->Shoot_Judge_Msg.Shoot_Cool_Now);
//}


void Shoot_Task(void *pvParameters)
{
	vTaskDelay(100);
	Shoot_Init(&Shoot);
	Shoot_Add_Variable(&Shoot);

	while(1)
	{

		Shoot_Mode_Set(&Shoot);
		//获取发射状态
		Shoot_Data_Update(&Shoot);
		//裁判系统状态更新
//		Shoot_Judge_Data_Check(&Shoot,&Shoot_Fric_Speed_Sett,&Trigger_Speed_Sett);
		
		Shoot_Control_Data_Set(&Shoot);
		
		Shoot_Pid_Calc(&Shoot);

		CAN1_Motor_Control(SHOOT_MOTOR_ALL_ID,(int16_t)Shoot.Fric_Motor_Current_Send[0],
																					(int16_t)Shoot.Fric_Motor_Current_Send[1],
																					(int16_t)Shoot.Trigger_Motor_Current_Send,0);

		vTaskDelay(1);		
	}
}

