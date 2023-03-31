/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    shoot_task.c
	* @brief   �����������           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 �Ѿ���ɶ�ȡ����ϵͳ�����ջ�
	* @Question 	 Ħ����PID������Ҫ��������Ҫ��ȡĦ����ִ�����ߡ�4.9��
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     4-9-2021     	 JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe �����������
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

static TaskHandle_t ShootTask_Handler;
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
		Add_Variable_By_Name_In_Task("Default Shoot Speed Limit",&Shoot_Variable->Default_Shoot_Speed_Limit,VAR_TYPE_UINT16,0);
		Add_Variable_By_Name_In_Task("Default Shoot Freq Limit",&Shoot_Variable->Default_Shoot_Freq_Limit,VAR_TYPE_UINT8,0);
		//Add_Variable_By_Name_In_Task("Max Shoot Freq",&Shoot_Variable->Max_Shoot_Freq,VAR_TYPE_UINT8,0);
	
}

//float Shoot_Freq_To_Trigger_Motor_Speed(uint8_t Convert_Factor,uint8_t Freq)
//{
//		return Freq/Convert_Factor*60.0;
//}

void Shoot_Mode_Set(Shoot_t* Mode_Set)
{
		uint8_t Shoot_Mode_Switch = Mode_Set->Shoot_RC_Ctl_Data->rc.s1;
		uint16_t Shoot_Mode_Key = Mode_Set->Shoot_RC_Ctl_Data->key.v;
		uint8_t Shoot_Mouse_Key = Mode_Set->Shoot_RC_Ctl_Data->mouse.press_l;
	
		uint8_t Shoot_Start_Stop_Signal = (Shoot_Mode_Switch==RC_SW_UP&&Mode_Set->Last_Shoot_Mode_Switch!=RC_SW_UP)
																			||((Shoot_Mode_Key&SHOOT_START_STOP_KEY)&&!(Mode_Set->Last_Shoot_Mode_Key&SHOOT_START_STOP_KEY));
	
	
		//��ֹͣ���״̬���ϲ�,���߰���R�������뿪ʼ���״̬
		if(Mode_Set->Shoot_Mode == SHOOT_STOP)
		{
				Laser_Off();
				if(Shoot_Start_Stop_Signal)
				{
						if(Mode_Set->Shoot_Key == SHOOT_KEY_ON)
								Mode_Set->Shoot_Mode = SHOOT_READY;
						else
								Mode_Set->Shoot_Mode = SHOOT_START;
				}
		}
		else 
		{
				Laser_On();
				//�ڳ���ֹͣ���֮�������״̬���ٴ��ϲ����߰���R��������ֹͣ���״̬
				if(Shoot_Start_Stop_Signal)
				{
						Mode_Set->Shoot_Mode = SHOOT_STOP;
				}
				//�������������ģʽ�ж�
				else
				{
						/****************�źŲ�������*****************/
						//ͳ�ư������������²�ʱ��
						if(Shoot_Mode_Switch==RC_SW_DOWN||Shoot_Mouse_Key)
						{
								if(Mode_Set->Shoot_Mode_Switch_Down_Time<SHOOT_MODE_SWITCH_DOWN_TIME_LIMIT)
										Mode_Set->Shoot_Mode_Switch_Down_Time++;
						}
						else 
								Mode_Set->Shoot_Mode_Switch_Down_Time=0;
						//��������������һֱ�����²�״̬�����������ź�
						uint8_t Shoot_Bullet_No_Break_Signal = Mode_Set->Shoot_Mode_Switch_Down_Time==SHOOT_MODE_SWITCH_DOWN_TIME_LIMIT;
						//����������������²�һ�Σ����������ź�
						uint8_t Shoot_Bullet_Once_Signal = (Shoot_Mode_Switch==RC_SW_DOWN&&Mode_Set->Last_Shoot_Mode_Switch!=RC_SW_DOWN)
																				||(Shoot_Mouse_Key&&!Mode_Set->Last_Shoot_Mouse_Key);
						//ÿ�μ�⵽�����źţ���Ҫ����ĵ�����Ŀ��һ
						if(Shoot_Bullet_Once_Signal)
							Mode_Set->Need_Shoot_Count++;
						//ÿ�μ�⵽�����źţ���Ҫ����ĵ�����Ŀ��һ
						if(Shoot_Bullet_No_Break_Signal)
							Mode_Set->Need_Shoot_Count=1;
						
						//�����ת�ź�
						uint8_t Trigger_Motor_Stall_Signal = 0;
						//�������ٶ�С��һ��ֵ����һ��ʱ�䣬˵�������ת��
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
						/****************״̬��ת�Ʋ���*****************/
						
						if(Mode_Set->Shoot_Mode == SHOOT_START)
						{
								//�ڿ�ʼ���״̬�£�������赽��΢�����أ���ô����׼�����״̬
								if(Mode_Set->Shoot_Key == SHOOT_KEY_ON)
										Mode_Set->Shoot_Mode = SHOOT_READY;
								//����ܳ�ʱ�䶼û�м����ϵ��裬�жϵ���Ƿ��ת
								else if(Mode_Set->Shoot_Start_Time>Mode_Set->Shoot_Start_Time_Limit)
								{
										//��������ת�ˣ�˵�������ˣ�����ز�״̬
										if(Trigger_Motor_Stall_Signal)
												Mode_Set->Shoot_Mode = SHOOT_STALL;
										//�������û�е�����
//										else
//												Mode_Set->No_Bullet_Flag = 1;
								}
						}
						//��׼�����״̬�£������Ҫ����ĵ�����Ŀ����0�����뷢��״̬
						else if(Mode_Set->Shoot_Mode == SHOOT_READY&&Mode_Set->Need_Shoot_Count)
						{
								Mode_Set->Shoot_Mode = SHOOT_BULLET;
						}
						//�ڿ���״̬�£�����ز�ʱ��ﵽ�趨ֵ�����»ص�׼����ʼ����״̬
						else if(Mode_Set->Shoot_Mode == SHOOT_STALL
									&&Mode_Set->Shoot_Stall_Time==Mode_Set->Shoot_Stall_Time_Limit)
						{
									Mode_Set->Shoot_Mode = SHOOT_START;
						}
						else if(Mode_Set->Shoot_Mode == SHOOT_BULLET)
						{
							//�ڷ��䵯��״̬�����������������뿪ʼ����״̬����Ҫ����ĵ�����Ŀ��һ
							if(Mode_Set->Shoot_Key==SHOOT_KEY_OFF)//&&Mode_Set->Last_Shoot_Key==SHOOT_KEY_ON)
							{
									Mode_Set->Shoot_Mode=SHOOT_START;
									Mode_Set->Need_Shoot_Count--;
							}
							//�ڷ��䵯��״̬�£������ʱ��δ������������Ƿ�ס��
							if(Mode_Set->Shoot_Bullet_Time>Mode_Set->Shoot_Bullet_Time_Limit)
							{
									//��������ס�ˣ�����ز�ģʽ
									if(Trigger_Motor_Stall_Signal)
											Mode_Set->Shoot_Mode=SHOOT_STALL;
									//�������û�е�����
//									else
//											Mode_Set->No_Bullet_Flag = 1;
							}
						}
				}
		}
		
		//������ڷ��䵯��״̬��ͳ�Ʒ���ʱ�䣬������״̬���뷢�䵯��״̬������ʱ������
		if(Mode_Set->Shoot_Mode==SHOOT_BULLET)
		{
				if(Mode_Set->Last_Shoot_Mode!=SHOOT_BULLET)
						Mode_Set->Shoot_Bullet_Time=0;
				else
						Mode_Set->Shoot_Bullet_Time++;
		}
		//������ڿ����ز�״̬��ͳ�ƿ����ز�ʱ�䣬������״̬���뿨���ز�״̬�������ز�ʱ������
		if(Mode_Set->Shoot_Mode==SHOOT_STALL)
		{
				if(Mode_Set->Last_Shoot_Mode!=SHOOT_STALL)
						Mode_Set->Shoot_Stall_Time=0;
				else
						Mode_Set->Shoot_Stall_Time++;
		}
		//������ڿ�ʼ���״̬��ͳ�ƿ�ʼ���״̬��ʱ�䣬������״̬���뿪ʼ���״̬��ʱ������
		if(Mode_Set->Shoot_Mode==SHOOT_START)
		{
				if(Mode_Set->Last_Shoot_Mode!=SHOOT_START)
						Mode_Set->Shoot_Start_Time=0;
				else
						Mode_Set->Shoot_Start_Time++;
		}
		//������ģʽ����ֹͣ���ģʽ����Ҫ�������Ŀ����
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
				
				Pid_Calc->Trigger_Motor_Current_Send = pid_calc_linear(&Pid_Calc->Trigger_Motor_Pid,Pid_Calc->Trigger_Motor_Speed_Get,Pid_Calc->Trigger_Motor_Speed_Set);
		}
		else
		{
//				Pid_Calc->Fric_Motor_Current_Send[0] = 0;
//				Pid_Calc->Fric_Motor_Current_Send[1] = 0;
				Pid_Calc->Trigger_Motor_Current_Send = 0;
		}
		int8_t Factor = Pid_Calc->Fric_Reverse_Flag?-1:1;
		Pid_Calc->Fric_Motor_Current_Send[0] = pid_calc_linear(&Pid_Calc->Fric_Motor_Pid[0],Pid_Calc->Fric_Motor_Speed_Get[0],Factor*-Pid_Calc->Fric_Motor_Speed_Set);
		Pid_Calc->Fric_Motor_Current_Send[1] = pid_calc_linear(&Pid_Calc->Fric_Motor_Pid[1],Pid_Calc->Fric_Motor_Speed_Get[1],Factor*Pid_Calc->Fric_Motor_Speed_Set);
		
}

void Shoot_Init(Shoot_t* Data_Init)
{
		//������ݻ�ȡ
		Data_Init->Fric_Motor_Msg_Get[0] = Get_DJI_Motor_Data(CAN1_RX,FRIC_MOTOR_LEFT_ID); 
		Data_Init->Fric_Motor_Msg_Get[1] = Get_DJI_Motor_Data(CAN1_RX,FRIC_MOTOR_RIGHT_ID); 
		Data_Init->Trigger_Motor_Msg_Get = Get_DJI_Motor_Data(CAN1_RX,TRIGGER_MOTOR_ID);
		
		Data_Init->Shoot_Mode = SHOOT_STOP;
		//ң�������ݻ�ȡ
		Data_Init->Shoot_RC_Ctl_Data = Get_DJI_RC_Data_Address();
		//����ϵͳ���ݻ�ȡ
		Data_Init->Shoot_Judge_Info_Get = Get_Judge_Info();
		//��ʼ��PID
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
		Data_Init->Default_Shoot_Freq_Limit = DEFAULT_SHOOT_FREQ_LIMIT;
		Data_Init->Default_Shoot_Speed_Limit = DEFAULT_SHOOT_SPEED_LIMIT;
		//Data_Init->Max_Shoot_Freq = MAX_SHOOT_FREQ;
}

uint8_t Get_Shoot_Freq_From_Judge_System(Shoot_t* Get_Shoot_Freq)
{
		if(Get_Shoot_Freq->Judge_Robot_ID!=0)
		{
				if(Get_Shoot_Freq->Judge_Shoot_Heat_Percent >= 95.0f)
				{
						return 0;
				}
				if((Get_Shoot_Freq->Judge_Shoot_Heat_Percent >= 0) && (Get_Shoot_Freq->Judge_Shoot_Heat_Percent < 60))
				{
						if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=50)
							return 3;
						else if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=100)
							return 8;
						else if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=150)
							return 12;
						else if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=280)
							return 18;
						else if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=400)
							return 26;	
				}
				else if((Get_Shoot_Freq->Judge_Shoot_Heat_Percent >= 60) 
						 && (Get_Shoot_Freq->Judge_Shoot_Heat_Percent <= (100.0f-Get_Shoot_Freq->Judge_Shoot_Cool_Percent)))
				{
						if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=50)
							return 1;
						else if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=100)
							return 2;
						else if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=150)
							return 3;
						else if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=280)
							return 4;
						else if(Get_Shoot_Freq->Judge_Shoot_Cooling_Limit <=400)
							return 5;
				}
				else if(Get_Shoot_Freq->Judge_Shoot_Heat_Percent > (100.0f-Get_Shoot_Freq->Judge_Shoot_Cool_Percent))
				{
						return (Get_Shoot_Freq->Judge_Shoot_Cooling_Limit / 10);
				}
		}
		return Get_Shoot_Freq->Default_Shoot_Freq_Limit;
}

float Get_Fric_Speed_From_Judge_System(Shoot_t* Get_Fric_Speed)
{
		if(Get_Fric_Speed->Judge_Robot_ID!=0)
		{
			//TODO �ж���û�г�����
			return Get_Fric_Speed->Judge_Shoot_Speed_Limit;
		}
		return Get_Fric_Speed->Default_Shoot_Speed_Limit;
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
				//����Ƶ���������ţ�����ص����ٶȵ�������һ��
				if(Control_Data_Set->Load_Bullet_Speed>0)
						Control_Data_Set->Trigger_Motor_Speed_Set = Freq_Set;
				else
						Control_Data_Set->Trigger_Motor_Speed_Set = -Freq_Set;
		}
		else if(Control_Data_Set->Shoot_Mode == SHOOT_STALL)
		{
				Control_Data_Set->Trigger_Motor_Speed_Set = Control_Data_Set->Unload_Bullet_Speed;
		}
		
		if(Control_Data_Set->Shoot_Mode != SHOOT_STOP)
		{
				Control_Data_Set->Fric_Motor_Speed_Set = Get_Fric_Speed_From_Judge_System(Control_Data_Set);
		}
		else
		{
				Control_Data_Set->Fric_Motor_Speed_Set = 0;
		}
}

void Shoot_Data_Update(Shoot_t* Data_Update)
{
		//������ݸ���
		Data_Update->Fric_Motor_Speed_Get[0]=Data_Update->Fric_Motor_Msg_Get[0]->Speed/60*PI*Data_Update->Fric_Wheel_Diameter;
		Data_Update->Fric_Motor_Speed_Get[1]=Data_Update->Fric_Motor_Msg_Get[1]->Speed/60*PI*Data_Update->Fric_Wheel_Diameter;
		Data_Update->Trigger_Motor_Speed_Get=Data_Update->Trigger_Motor_Msg_Get->Speed/60/36*Data_Update->Bullets_Per_Rotation;
		Data_Update->Shoot_Key = Shoot_Key_Read();
		//����ϵͳ���ݸ���
		Data_Update->Judge_Robot_ID = Data_Update->Shoot_Judge_Info_Get->Judge_game_robot_status.robot_id;
		if(Data_Update->Judge_Robot_ID!=0)
		{
				//�ж��Ƿ���Ӣ�ۣ������Ӣ��ʹ��42mm�����������
				if(Data_Update->Judge_Robot_ID == Red_Hero||Data_Update->Judge_Robot_ID == Blue_Hero)
				{
						Data_Update->Judge_Shoot_Cooling_Limit = Data_Update->Shoot_Judge_Info_Get->Judge_game_robot_status.shooter_id1_42mm_cooling_limit;
						Data_Update->Judge_Shoot_Cooling_Rate = Data_Update->Shoot_Judge_Info_Get->Judge_game_robot_status.shooter_id1_42mm_cooling_rate;
						Data_Update->Judge_Shoot_Cooling_Heat = Data_Update->Shoot_Judge_Info_Get->Judge_power_heat_data.shooter_id1_42mm_cooling_heat;
						Data_Update->Judge_Shoot_Speed_Limit = Data_Update->Shoot_Judge_Info_Get->Judge_game_robot_status.shooter_id1_42mm_speed_limit;
				}
				else
				{
						Data_Update->Judge_Shoot_Cooling_Limit = Data_Update->Shoot_Judge_Info_Get->Judge_game_robot_status.shooter_id1_17mm_cooling_limit;
						Data_Update->Judge_Shoot_Cooling_Rate = Data_Update->Shoot_Judge_Info_Get->Judge_game_robot_status.shooter_id1_17mm_cooling_rate;
						Data_Update->Judge_Shoot_Cooling_Heat = Data_Update->Shoot_Judge_Info_Get->Judge_power_heat_data.shooter_id1_17mm_cooling_heat;
						Data_Update->Judge_Shoot_Speed_Limit = Data_Update->Shoot_Judge_Info_Get->Judge_game_robot_status.shooter_id1_17mm_speed_limit;
				}
				Data_Update->Judge_Shoot_Heat_Percent = (float)Data_Update->Judge_Shoot_Cooling_Heat/Data_Update->Judge_Shoot_Cooling_Limit;
				Data_Update->Judge_Shoot_Cool_Percent = (float)Data_Update->Judge_Shoot_Cooling_Rate/Data_Update->Judge_Shoot_Cooling_Limit;
		}
		
}

void Shoot_Task(void *pvParameters)
{
	vTaskDelay(100);
	Shoot_Init(&Shoot);
	Shoot_Add_Variable(&Shoot);

	while(1)
	{

		Shoot_Mode_Set(&Shoot);
		//��ȡ����״̬
		Shoot_Data_Update(&Shoot);
		//����ϵͳ״̬����
		Shoot_Control_Data_Set(&Shoot);
		
		Shoot_Pid_Calc(&Shoot);

		CAN1_Motor_Control(SHOOT_MOTOR_ALL_ID,(int16_t)Shoot.Fric_Motor_Current_Send[0],
																					(int16_t)Shoot.Fric_Motor_Current_Send[1],
																					(int16_t)Shoot.Trigger_Motor_Current_Send,0);

		vTaskDelay(1);		
	}
}

void Create_Shoot_Task()
{
	xTaskCreate((TaskFunction_t)Shoot_Task,
	  (const char *)"Shoot_Task",
		(uint16_t)SHOOT_STK_SIZE,
		(void *)NULL,
		(UBaseType_t)SHOOT_TASK_PRIO,
		(TaskHandle_t *)&ShootTask_Handler);	
}
