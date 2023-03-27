/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    judge_task.c
	* @brief   裁判系统任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 定期发送
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     12-4-2020      JackyJuu            Done
  *  V1.2.0      4-2-2020      JackyJuu            Done	
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe OLED显示屏任务
*/
#include "judge_task.h"

#include "chassis_task.h"

#include "Judge_Data.h"
#include "DJI_Remote_Control.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#define Robot_Com_Id 0x0103

Judge_Show_Msg_t Judge_Show_Msg;

uint8_t Chassis_Mode_Word[8][40] = {"Chassis_Zero",//无控制，传送0
																	"Chassis_Motionless",//无控制，用电机角度保持当前位置
																	"Chassis_RC_Control",//遥控器控制
																	"Chassis_Follow_Gimbal",//底盘跟随云台
																	"Chassis_Follow_Chassis",//我跟我自己
																	"Chassis_Spin_Right",
																	"Chassis_Spin_Left",
																	"Chassis_Spin"};//小陀螺模式

uint8_t Gimbal_Mode_Word[7][40] = {"Gimbal_Zero",//无控制，传送0
																	"Gimbal_Motionless",//无控制，用电机角度保持当前位置
																	"Gimbal_Motor_Control",//电机角度控制
																	"Gimbal_IMU_Control",//陀螺仪角度控制
																	"Gimbal_Vision_Control",//视觉控制接口
																	"Gimbal_Follow_Chassis",//云台跟随底盘，yaw轴固定不动
																	"Gimbal_Spin"};//小陀螺模式	

uint8_t Super_Word[40] = {"SuperC_Limit: %"};

extern DJI_Judge_t DJI_Judge; 
extern RC_Ctl_t RC_Ctl_Data;

uint8_t Judge_Test_Send_Data[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
uint8_t Judge_Test_Graphic_Name[] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};

//uint8_t	Judge_Test_Graphic_Data[200];

int test_size;

uint32_t test_judge_graphic_data;

int write_mode = 0;

//uint8_t	Judge_Test_Graphic_Data[] = "Hello World";
uint8_t Judge_Test_Graphic_Data_Send[45];

float test_nummmm = 66.66;
int32_t test_nummmmmm = 12888;

uint8_t	Judge_Super_Cap_Data_Send[] = "Super-Cap:";

uint8_t Judge_Test_Graphic_Data[3][2][30] = {{"DN1","Super-Cap:"},\
																						 {"DN2","Version_Ready"},\
																						 {"DN3","Version_Off"}};

int Judge_Set_Init[10][4] = {{3,20,750,780},//超级电容数据显示
														 {3,20,750,740},//视觉自瞄是否瞄到显示
														 {2,15,800,400},//底盘模式
														 {3,20,800,300},//云台模式
														 {3,20,800,300},
														 {3,20,800,300},
														 {3,20,800,300},
														 {3,20,800,300},
														 {3,20,800,300},
														 {3,20,800,300}};

int Location_x[10],Location_y[10];
int Data_Width[10],Data_Sizw[10];
int Judge_Num_Set;
int key_last;
														 
uint8_t Student_Judge_Send_Data[] = "FUCK YOU";
														 
int Judge_Data_Send_Update;
														 
void Move_Step(int Step)
{
	//Location_x[Judge_Num_Set] = 
}	

void Judge_Send_Data_Update(uint8_t* Graphic_Update_Name)
{
	Judge_Character_operate_tpye_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Graphic_Update_Name,Graphic_Add);
	Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
	Judge_Character_operate_tpye_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Graphic_Update_Name,Graphic_Add);
	Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
}

//uint8_t Leida_Data[14] = {}
int Show_Num;

void Judge_Chassis_Mode_Show_Add(Chassis_Mode_Judge_Msg_Show_t* Judge_Msg_Show,int Locate_X,int Locate_Y)
{
		Show_Num = (int)*Judge_Msg_Show->Chassis_Mode_Show;
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Msg_Show->Chassis_Mode_Name,\
		Character,Graphic_Add,Judge_Chassis_Show_Floor,3,15,14,2,Locate_X,Locate_Y,Judge_Msg_Show->Chassis_Mode_Word_Add[Show_Num]);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
}

void Judge_Gimbal_Mode_Show_Add(Gimbal_Mode_Judge_Msg_Show_t* Judge_Msg_Show,int Locate_X,int Locate_Y)
{
		Show_Num = (int)*Judge_Msg_Show->Gimbal_Mode_Show;
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Msg_Show->Gimbal_Mode_Name,\
		Character,Graphic_Add,Judge_Gimbal_Show_Floor,3,15,14,2,Locate_X,Locate_Y,Judge_Msg_Show->Gimbal_Mode_Word_Add[Show_Num]);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
}


void Judge_Chassis_Mode_Show_Set(Chassis_Mode_Judge_Msg_Show_t* Judge_Msg_Show,int Locate_X,int Locate_Y)
{
		Show_Num = (int)*Judge_Msg_Show->Chassis_Mode_Show;
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Msg_Show->Chassis_Mode_Name,\
		Character,Graphic_Change,Judge_Chassis_Show_Floor,3,15,14,2,Locate_X,Locate_Y,Judge_Msg_Show->Chassis_Mode_Word_Add[Show_Num]);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
}

void Judge_Gimbal_Mode_Show_Set(Gimbal_Mode_Judge_Msg_Show_t* Judge_Msg_Show,int Locate_X,int Locate_Y)
{
		Show_Num = (int)*Judge_Msg_Show->Gimbal_Mode_Show;
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Msg_Show->Gimbal_Mode_Name,\
		Character,Graphic_Change,Judge_Gimbal_Show_Floor,3,15,14,2,Locate_X,Locate_Y,Judge_Msg_Show->Gimbal_Mode_Word_Add[Show_Num]);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
}

void Judge_Vision_Show_Add(int Vision_flag,int Locate_X,int Locate_Y)
{
	if(Vision_flag == 1)
	{
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,(uint8_t*)"VSD",\
		Character,Graphic_Add,Judge_Vision_Floor,3,15,14,2,Locate_X,Locate_Y,Judge_Test_Graphic_Data[1][1]);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
		
	}
	else if(Vision_flag == 0)
	{
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,(uint8_t*)"VSD",\
		Character,Graphic_Add,Judge_Vision_Floor,3,15,14,2,Locate_X,Locate_Y,Judge_Test_Graphic_Data[2][1]);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);		
	}
}

void Judge_Vision_Show_Set(int Vision_flag,int Locate_X,int Locate_Y)
{
	if(Vision_flag == 1)
	{
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,(uint8_t*)"VSD",\
		Character,Graphic_Change,Judge_Vision_Floor,3,15,14,2,Locate_X,Locate_Y,Judge_Test_Graphic_Data[1][1]);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
	}
	else if(Vision_flag == 0)
	{
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,(uint8_t*)"VSD",\
		Character,Graphic_Change,Judge_Vision_Floor,3,15,14,2,Locate_X,Locate_Y,Judge_Test_Graphic_Data[2][1]);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);		
	}
}

void Judge_Super_C_Show_Add(Super_C_Judge_Msg_Show_t* Super_C_Judge_Msg_Show,int Locate_X,int Locate_Y)
{
		Super_C_Judge_Msg_Show->Super_C_Limit = (int)((float)(Super_C_Judge_Msg_Show->Super_C_Msg_Show->Voilt_C) / (float)(Super_C_Judge_Msg_Show->Super_C_Msg_Show->Voilt_In) * 100.00f);
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Super_C_Judge_Msg_Show->Super_C_Name,\
		Character,Graphic_Add,Judge_Super_C_Show_Floor,3,15,14,2,Locate_X,Locate_Y,Super_C_Judge_Msg_Show->Super_C_Word);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);		
		Judge_Word_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Word_data_struct,Judge_Test_Graphic_Name,\
		Integer,Graphic_Add,Judge_Super_C_Show_Floor,3,15,14,2,Locate_X + 183,Locate_Y,0,Super_C_Judge_Msg_Show->Super_C_Limit);		
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,15);		

}

void Judge_Super_C_Show_Set(Super_C_Judge_Msg_Show_t* Super_C_Judge_Msg_Show,int Locate_X,int Locate_Y)
{
		Super_C_Judge_Msg_Show->Super_C_Limit = (int)(((float)(Super_C_Judge_Msg_Show->Super_C_Msg_Show->Voilt_C) - 1500.00f) / ((float)(Super_C_Judge_Msg_Show->Super_C_Msg_Show->Voilt_In) - 1500.00f) * 100.00f);
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Super_C_Judge_Msg_Show->Super_C_Name,\
		Character,Graphic_Change,Judge_Super_C_Show_Floor,3,15,14,2,Locate_X,Locate_Y,Super_C_Judge_Msg_Show->Super_C_Word);
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);		
		Judge_Word_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Word_data_struct,Judge_Test_Graphic_Name,\
		Integer,Graphic_Change,Judge_Super_C_Show_Floor,3,15,14,2,Locate_X + 183,Locate_Y,0,Super_C_Judge_Msg_Show->Super_C_Limit);		
		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,15);		
}

void Robot_Mode_Judge_Show(Judge_Show_Msg_t* Judge_Show_Mode)
{
			Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Show_Mode->Chassis_Mode_Judge_Msg_Show.Chassis_Mode_Name,\
			Character,Graphic_Add,2,3,Data_Sizw[2],14,Data_Width[2],1700,700,Judge_Show_Mode->Chassis_Mode_Judge_Msg_Show.Chassis_Mode_Word_Add[1]);
			Judge_Data_Send_To_Client(&DJI_Judge,0x0110,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
}

extern int Vision_Get_Flag;
static float Judge_Angle_Get;
int Data_Lennnn;
int super_C_Num_Test;

void Judge_Get_Stydent_Msg()
{
	
}

void Judge_Task(void *pvParameters)
{
	vTaskDelay(500);
	
	Judge_Show_Msg.Chassis_Mode_Judge_Msg_Show.Chassis_Mode_Show = Return_Chassis_Mode_Add();
	Judge_Show_Msg.Gimbal_Mode_Judge_Msg_Show.Gimbal_Mode_Show = Return_Gimbal_Mode_Add();
	Judge_Show_Msg.Super_C_Judge_Msg_Show.Super_C_Msg_Show = Get_Cap_Data();
	
	Judge_Show_Msg.Chassis_Mode_Judge_Msg_Show.Chassis_Mode_Name = (uint8_t*)"CJS";
	Judge_Show_Msg.Gimbal_Mode_Judge_Msg_Show.Gimbal_Mode_Name = (uint8_t*)"GJS";
	Judge_Show_Msg.Super_C_Judge_Msg_Show.Super_C_Name = (uint8_t*)"SCJ";
	Judge_Show_Msg.Super_C_Judge_Msg_Show.Super_C_Word = Super_Word;
	
	for(int  i = 0;i < 7;i++)
	{
		Judge_Show_Msg.Chassis_Mode_Judge_Msg_Show.Chassis_Mode_Word_Add[i] = Chassis_Mode_Word[i];	
	}
	
	for(int  i = 0;i < 7;i++)
	{
		Judge_Show_Msg.Gimbal_Mode_Judge_Msg_Show.Gimbal_Mode_Word_Add[i] = Gimbal_Mode_Word[i];	
	}

	
	for(int i = 0;i < 10;i++)
	{
		Data_Width[i] = Judge_Set_Init[i][0];
		Data_Sizw[i] = Judge_Set_Init[i][1];	
		Location_x[i] = Judge_Set_Init[i][2];
		Location_y[i] = Judge_Set_Init[i][3];
	}

//	Judge_Word_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Word_data_struct,Judge_Super_Cap_Data_Send,\
//	Floating,Graphic_Add,0,2,20,3,3,800,200,test_nummmm,0);	
//	Judge_Data_Send_To_Client(&DJI_Judge,0x0101,0x0103, DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,15);	
//	
//	Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[0][0],\
//	Character,Graphic_Add,0,3,Data_Sizw[0],10,Data_Width[0],Location_x[0],Location_y[0],Judge_Test_Graphic_Data[0][1]);
//	Judge_Data_Send_To_Client(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);

//	Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[1][0],\
//	Character,Graphic_Add,0,3,Data_Sizw[0],10,Data_Width[0],Location_x[0],Location_y[0],Judge_Test_Graphic_Data[0][1]);
//	Judge_Data_Send_To_Client(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);

//	if(Vision_Get_Flag == 1)
//	{
//		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[1][0],\
//		Character,Graphic_Add,1,3,Data_Sizw[1],14,Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[1][1]);
//		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
//		
//	}
//	else if(Vision_Get_Flag == 0)
//	{
//		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[2][0],\
//		Character,Graphic_Add,1,3,Data_Sizw[1],14,Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[2][1]);
//		Judge_Data_Send_To_Client(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);		
//	}
	
	
	Judge_Gimbal_Mode_Show_Add(&Judge_Show_Msg.Gimbal_Mode_Judge_Msg_Show,1600,790);
	Judge_Chassis_Mode_Show_Add(&Judge_Show_Msg.Chassis_Mode_Judge_Msg_Show,1600,760);
	Judge_Vision_Show_Add(Vision_Get_Flag,1600,730);
	Judge_Super_C_Show_Add(&Judge_Show_Msg.Super_C_Judge_Msg_Show,1600,700);
	
	Judge_Num_Set = 0;

	
	while(1)
	{
		
		//Robot_Mode_Judge_Show(&Judge_Show_Msg);
		
		Judge_Gimbal_Mode_Show_Set(&Judge_Show_Msg.Gimbal_Mode_Judge_Msg_Show,1600,790);
		Judge_Chassis_Mode_Show_Set(&Judge_Show_Msg.Chassis_Mode_Judge_Msg_Show,1600,760);
		Judge_Vision_Show_Set(Vision_Get_Flag,1600,730);
		Judge_Super_C_Show_Set(&Judge_Show_Msg.Super_C_Judge_Msg_Show,1600,700);
				
		if(Judge_Data_Send_Update > (1000/Judge_Set_Update_Time_Set))
		{
			Judge_Gimbal_Mode_Show_Add(&Judge_Show_Msg.Gimbal_Mode_Judge_Msg_Show,1600,790);
			Judge_Chassis_Mode_Show_Add(&Judge_Show_Msg.Chassis_Mode_Judge_Msg_Show,1600,760);
			Judge_Vision_Show_Add(Vision_Get_Flag,1600,730);
			Judge_Super_C_Show_Add(&Judge_Show_Msg.Super_C_Judge_Msg_Show,1600,700);
		}
		
		
		
//		if(Vision_Get_Flag == 1)
//		{
//			Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[1][0],\
//			Character,Graphic_Change,1,3,Data_Sizw[1],14,Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[1][1]);
//			Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
//		}
//		else if(Vision_Get_Flag == 0)
//		{
//			Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[2][0],\
//			Character,Graphic_Change,1,3,Data_Sizw[1],14,Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[2][1]);
//			Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
//		}
		
		
//			Data_Lennnn = strlen((char *)&DJI_Judge.DJI_Judge_Mes.Judge_Student_Data.Judge_Student_Data_Get);
//			Judge_Data_Send(&DJI_Judge,0x0201,3,Student_Judge_Send_Data,sizeof(Student_Judge_Send_Data));

			
	/*	
		if(Judge_Data_Send_Update > (1000/Judge_Set_Update_Time_Set))
		{
			Judge_Data_Send_Update = 0;
			Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[0][0],\
			Character,Graphic_Add,0,3,Data_Sizw[0],10,Data_Width[0],Location_x[0],Location_y[0],Judge_Test_Graphic_Data[0][1]);
			Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
			
			if(Vision_Get_Flag == 1)
			{
				Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[1][0],\
				Character,Graphic_Add,1,3,Data_Sizw[1],14,Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[1][1]);
				Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);		
			}
			else if(Vision_Get_Flag == 0)
			{
				Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[2][0],\
				Character,Graphic_Add,1,3,Data_Sizw[1],14,Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[2][1]);
				Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
			}
		}
		
		if(RC_Ctl_Data.rc.s2 == 0x01)
			Judge_Num_Set = 2;
		else if(RC_Ctl_Data.rc.s2 == 0x03)
			Judge_Num_Set = 1;
		else if(RC_Ctl_Data.rc.s2 == 0x02)
			Judge_Num_Set = 0;
		
		Location_x[Judge_Num_Set] += (float)((RC_Ctl_Data.rc.ch0)/66.000f);
		Location_y[Judge_Num_Set] += (float)((RC_Ctl_Data.rc.ch1)/66.000f);
		Data_Width[Judge_Num_Set] += (float)((RC_Ctl_Data.rc.ch2)/220.000f);
		Data_Sizw[Judge_Num_Set] += (float)((RC_Ctl_Data.rc.ch3)/220.000f);
					
		Judge_Word_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Word_data_struct,Judge_Super_Cap_Data_Send,\
		Floating,Graphic_Add,4,2,20,3,3,800,200,test_nummmm,0);					
		Judge_Data_Send(&DJI_Judge,0x0101,0x0103, DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Graphic_Single_Long);	
		
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[0][0],\
		Character,Graphic_Change,0,3,Data_Sizw[0],10,Data_Width[0],Location_x[0],Location_y[0],Judge_Test_Graphic_Data[0][1]);
		Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
				
	if(Vision_Get_Flag == 1)
	{
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[1][0],\
		Character,Graphic_Change,1,3,Data_Sizw[1],14,Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[1][1]);
		Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
	}
	else if(Vision_Get_Flag == 0)
	{
		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[2][0],\
		Character,Graphic_Change,1,3,Data_Sizw[1],14,Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[2][1]);
		Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
	}
	
	
	*/
		
		
//	Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Seven.grapic_data_struct[0],Judge_Test_Graphic_Name,\
//	Straight_Line,Graphic_Add,5,7,0,0,5,0,0,0,1920,1080);
//	Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Seven.grapic_data_struct[1],Judge_Test_Graphic_Name+1,\
//	Rectangle,Graphic_Add,4,6,0,0,10,480,270,0,1440,810);
//	Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Seven.grapic_data_struct[2],Judge_Test_Graphic_Name+2,\
//	All_Circle,Graphic_Add,3,5,0,0,15,960,540,270,0,0);		
//	Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Seven.grapic_data_struct[3],Judge_Test_Graphic_Name+3,\
//	Oval_Circle,Graphic_Add,2,4,0,0,20,960,540,0,960,540);	
//	Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Seven.grapic_data_struct[4],Judge_Test_Graphic_Name+4,\
//	Arc_Circle,Graphic_Add,1,3,0,270,25,960,540,0,540,270);			
//	Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Seven.grapic_data_struct[5],Judge_Test_Graphic_Name+5,\
//	Arc_Circle,Graphic_Add,1,5,90,330,25,960,540,0,270,270);		
//	Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Seven.grapic_data_struct[6],Judge_Test_Graphic_Name+6,\
//	All_Circle,Graphic_Add,3,5,0,0,15,960,540,70,0,0);	
//	Judge_Data_Send(&DJI_Judge,0x0104,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Seven.Judge_client_custom_graphic_seven_Data,Graphic_seven_Long);


//		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[1][0],\
//			Character,Graphic_Change,1,3,Data_Sizw[1],sizeof(Judge_Test_Graphic_Data[1][1]),Data_Width[1],Location_x[1],Location_y[1],Judge_Test_Graphic_Data[1][1]);
//		Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
		
//		Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Data[2][0],\
//		Character,Graphic_Change,2,3,Data_Sizw[2],sizeof(Judge_Test_Graphic_Data[2][1]),Data_Width[2],Location_x[2],Location_y[2],Judge_Test_Graphic_Data[2][1]);
//		Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);


//			Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Name,\
//				Character,Graphic_Change,0,3,Data_Sizw[0],sizeof(Judge_Super_Cap_Data_Send),Data_Width[0],Location_x[0],Location_y[0],Judge_Super_Cap_Data_Send);
//			Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
//			
//						Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Name,\
//				Character,Graphic_Change,1,3,Data_Sizw[1],sizeof(Judge_Super_Cap_Data_Send),Data_Width[1],Location_x[1],Location_y[2],Judge_Super_Cap_Data_Send);
//			Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
//			
//						Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Name,\
//				Character,Graphic_Change,2,3,Data_Sizw[2],sizeof(Judge_Super_Cap_Data_Send),Data_Width[2],Location_x[3],Location_y[3],Judge_Super_Cap_Data_Send);
//			Judge_Data_Send(&DJI_Judge,0x0110,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);
	
//			Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Name,\
//			Character,Graphic_Add,2,2,20,10,5,960,540,Judge_Test_Graphic_Data);	
			//memcpy(DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character.data,(uint8_t *)&Judge_Super_Cap_Data_Send,30);
			//Judge_Data_Send(&DJI_Judge,0x0101,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,45);
	//	Judge_Data_Send(&DJI_Judge,0x0101,0x0103, DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,Graphic_character_Long);	
/*		
		
			if((KEY_Scan() == 1) && (key_last != 1))				
			{

				Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Straight_Line,Graphic_Add,5,7,0,0,5,0,0,0,1920,1080);
				Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Rectangle,Graphic_Add,4,6,0,0,10,480,270,0,1440,810);
				Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						All_Circle,Graphic_Add,3,5,0,0,15,960,540,270,0,0);		
				Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Oval_Circle,Graphic_Add,2,4,0,0,20,960,540,0,960,540);	
				Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Arc_Circle,Graphic_Add,1,3,0,270,25,960,540,0,540,270);						
				
				switch(write_mode)
				{	
					case 1:																																																			
						Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Straight_Line,Graphic_Add,5,7,0,0,5,0,0,0,1920,1080);				
					break;
					case 2:
						Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Rectangle,Graphic_Add,4,6,0,0,10,480,270,0,1440,810);					
					break;
					case 3:
						Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						All_Circle,Graphic_Add,3,5,0,0,15,960,540,270,0,0);					
					break;
					case 4:
						Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Oval_Circle,Graphic_Add,2,4,0,0,20,960,540,0,960,540);					
					break;
					case 5:
						Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Arc_Circle,Graphic_Add,1,3,0,270,25,960,540,0,540,270);					
					break;
					case 6:
						Judge_Word_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single,Judge_Test_Graphic_Name,\
						Floating,Graphic_Add,9,2,100,4,10,480,270,test_nummmm,0);	
//						memcpy(DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data+11,(uint8_t *)&test_nummmm,4); 
					break;
					case 7:
						Judge_Word_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single,Judge_Test_Graphic_Name,\
						Integer,Graphic_Add,8,1,50,0,5,480,810,0,test_nummmmmm);		
//						memcpy(DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data+11,(uint8_t *)&test_nummmmmm,4); 					
					break;
					case 8:
						Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
						Arc_Circle,Graphic_Add,5,3,8,270,25,960,540,0,540,270);					
					break;
				}

//					Judge_Graphic_Data_Set((graphic_data_struct_t*)&DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,Judge_Test_Graphic_Name,\
//					Oval_Circle,Graphic_Add,2,4,0,0,20,960,540,0,960,540);
				Judge_Data_Send(&DJI_Judge,0x0101,0x0103, DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,15);											


				Judge_Character_Data_Set(&DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character,Judge_Test_Graphic_Name,\
				Character,Graphic_Add,5,3,50,test_size,5,960,540,Judge_Test_Graphic_Data);		
				
//				memcpy(Judge_Test_Graphic_Data_Send,DJI_Judge.DJI_Judge_Send_Msg.Judge_Graphic_Client_Single.Judge_client_custom_graphic_single_Data,15);
//				memcpy(Judge_Test_Graphic_Data_Send + 15,Judge_Test_Graphic_Data,test_size);
				memcpy(DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_client_custom_character.data,(uint8_t *)&Judge_Test_Graphic_Data,30);
				Judge_Data_Send(&DJI_Judge,0x0101,0x0103,DJI_Judge.DJI_Judge_Send_Msg.Judge_Character_Client.Judge_Character_Data,45);					
				write_mode++;
				if(write_mode >= 9)
				{
					write_mode = 0;
				}
			}
			key_last = KEY_Scan();
			*/
//			Judge_Data_Send_Update++;
//			Judge_Angle_Get = -(DJI_Judge.DJI_Judge_Mes.Judge_game_robot_pos.yaw - 180.00f) / 180.000f * 3.1415926f;			
			vTaskDelay(Judge_Set_Update_Time_Set);
	}
}


float* Judge_Angle_Return(void)
{
	return &Judge_Angle_Get;
}
