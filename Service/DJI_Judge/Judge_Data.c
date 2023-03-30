/**
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
  * @file    Judge_Data.c
	* @brief   DJI裁判系统数据发送函数           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 所有设计裁判系统发送接收函数如下所示
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     12-2-2020      JackyJuu            Done
  *  V1.2.0     4-2-2021      JackyJuu            Done
  ****************************(C) COPYRIGHT 2021 HRBUST_AIR****************************
	* @describe DJI裁判系统串口数据
*/
#include "Judge_Data.h"
#include "Judge_CRC.h"

#define    FALSE    0
#define    TRUE     1

#define JUDGE_DATA_STK_SIZE 256
#define JUDGE_DATA_TASK_PRIO 21


Judge_Data_t Judge_Data;

Judge_Info_t* Get_Judge_Info(void)
{
		return &Judge_Data.DJI_Judge_Info;
}

//初始化
void Judge_Usart_Init()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_DMA2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	USART_DeInit(USART6);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  USART_Init(USART6, &USART_InitStructure);

  USART_ClearFlag(USART6, USART_FLAG_IDLE);
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);  

	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE); 
	USART_Cmd(USART6,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(DMA2_Stream1, DISABLE);
	DMA_DeInit(DMA2_Stream1);
	DMA_InitStructure.DMA_Channel	= DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART6->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Judge_Data.Judge_Usart_Rx_Buff[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = DJI_JUDGE_BUF_MAX_LEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_Init(DMA2_Stream1,&DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DMA2_Stream1, (uint32_t)Judge_Data.Judge_Usart_Rx_Buff[1], DMA_Memory_0);
  DMA_DoubleBufferModeCmd(DMA2_Stream1, ENABLE);
	
	//DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream1,DISABLE);
	DMA_Cmd(DMA2_Stream1,ENABLE);
	
	DMA_Cmd(DMA2_Stream7, DISABLE);
	DMA_DeInit(DMA2_Stream7);
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Judge_Data.Judge_Usart_Tx_Buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = DJI_JUDGE_BUF_MAX_LEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);

	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream7, DISABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void Judge_Info_Update(uint8_t* Data,uint16_t Data_Len,uint16_t CmdID)
{
		int Judge_Data_Len;
		void* Target_Structure = NULL;
		switch(CmdID)
		{
			case Judge_CmdID_Game_State:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_game_status;
				break;
			}
			case Judge_CmdID_Game_Result:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_game_result;
				break;
			}
			case Judge_CmdID_Game_Robot_HP:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_game_robot_HP;
				break;
			}
			case Judge_CmdID_Dart_Status:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_dart_client_cmd;
				break;
			}
			case Judge_CmdID_Event:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_event_data;
				break;
			}
			case Judge_CmdID_Supply_Projectile_Action:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_supply_projectile_action;
				break;
			}
			case Judge_CmdID_Referee_Warning:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_referee_warning;
				break;
			}
			case Judge_CmdID_Dart_Remaining_Time:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_dart_remaining_time;
				break;
			}
			case Judge_CmdID_Game_Robot_Status:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_game_robot_status;
				break;
			}
			case Judge_CmdID_Power_Heat_Data:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_power_heat_data;
				break;
			}
			case Judge_CmdID_Game_Robot_Pos:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_game_robot_pos;
				break;
			}
			case Judge_CmdID_Buff:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_buff;
				break;
			}
			case Judge_CmdID_Aerial_Robot_Energy:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_aerial_robot_energy;
				break;
			}
			case Judge_CmdID_Robot_Hurt:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_robot_hurt;
				break;
			}
			case Judge_CmdID_Shoot_Data:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_shoot_data;
				break;
			}
			case Judge_CmdID_Bullet_Remaining:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_bullet_remaining;
				break;
			}
			case Judge_CmdID_RFID_Status:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_RFID_status;
				break;
			}
			case Judge_CmdID_Dart_Client_Cmd:
			{
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_dart_client_cmd;
				break;
			}
			case Judge_CmdID_Robot_Interactive_Data:
				Target_Structure = &Judge_Data.DJI_Judge_Info.Judge_Student_Data;
				break;
			default:
				break;
		}
		if(Target_Structure)
			memcpy(Target_Structure, Data, Data_Len);
}

void Judge_Data_Init()
{	
		Judge_Data.Judge_Data_Send_Mutex = xSemaphoreCreateBinary();
		Judge_Data.Judge_Data_Receive_Mutex = xSemaphoreCreateBinary();
		xSemaphoreGive(Judge_Data.Judge_Data_Send_Mutex);
		xSemaphoreGive(Judge_Data.Judge_Data_Receive_Mutex);
}


static void Data_Parse_Handler(uint8_t* Judge_Data_Buff)
{
		Judge_Frame_Header_t*  Judge_Frame_Header = (Judge_Frame_Header_t*)Judge_Data_Buff;
		if(verify_crc8_check_sum(Judge_Data_Buff,JUDGE_HEADER_LEN,Judge_Frame_Header->crc8))
		{
				//读取帧尾数据并CRC校验
				uint16_t Data_Len = Judge_Frame_Header->data_length;
				uint16_t Data_Frame_Len = Data_Len + JUDGE_HEADER_LEN + JUDGE_CMDID_LEN +JUDGE_CRC16_LEN;
				uint16_t* CRC16 = (uint16_t*)(&Judge_Data_Buff[Data_Frame_Len-JUDGE_CRC16_LEN]);
				if(verify_crc16_check_sum(Judge_Data_Buff,Data_Frame_Len,*CRC16))
				{
						uint8_t* Data = Judge_Data_Buff + JUDGE_HEADER_LEN + JUDGE_CMDID_LEN;
						Judge_Info_Update(Data,Data_Len,Judge_Frame_Header->cmd_id);
				}
		}
		memset(Judge_Data_Buff,0,DJI_JUDGE_BUF_MAX_LEN);
		
}

void Judge_Usart_DMA_Send(uint32_t Size)
{
		DMA_SetCurrDataCounter(DMA2_Stream7,Size);						//设置数据传输长度
		DMA_Cmd(DMA2_Stream7,ENABLE);	
}

//uint8_t Judge_Student_Data_Get_Receive_Len()
//{
//		xSemaphoreTake(Judge_Data.Judge_Data_Receive_Mutex,portMAX_DELAY);
//		Judge_Data.DJI_Judge_Info.Judge_Student_Data.Judge_Student_Data_Header.
//		xSemaphoreGive(Judge_Data.Judge_Data_Receive_Mutex)
//}

//void Judge_Student_Data_Receive()
//{
//		xSemaphoreTake(Judge_Data.Judge_Data_Receive_Mutex,portMAX_DELAY);
//		
//		xSemaphoreGive(Judge_Data.Judge_Data_Receive_Mutex);
//}

uint16_t Generate_Target_ID(Judge_Student_Data_Target_t Data_Target)
{
		uint16_t Robot_ID = Judge_Data.DJI_Judge_Info.Judge_game_robot_status.robot_id;
		uint8_t Robot_Role = Robot_ID/100;//红方0，蓝方1
		uint16_t Target_ID;
		if(Data_Target==Target_Client)
		{
			Target_ID = Robot_ID%100 + Robot_Role?0x164:0x100;
		}
		else
		{
			Target_ID = Robot_Role*100 + Data_Target;
		}
		return Target_ID;
}

void Judge_Student_Data_Send(uint8_t* Stuent_Data,uint16_t Student_Data_Len,uint16_t Student_CmdID,Judge_Student_Data_Target_t Data_Target)
{
		uint16_t Target_ID = Generate_Target_ID(Data_Target);
		xSemaphoreTake(Judge_Data.Judge_Data_Send_Mutex,portMAX_DELAY);
		//学生数据帧头地址=起始地址+数据帧头长度+命令码长度
		Judge_Student_Data_Header_t* Judge_Student_Data_Header = (Judge_Student_Data_Header_t*)(Judge_Data.Judge_Usart_Tx_Buff+JUDGE_HEADER_LEN+JUDGE_CMDID_LEN);
		//设置学生数据帧头内容
		Judge_Student_Data_Header->data_cmd_id = Student_CmdID;
		Judge_Student_Data_Header->sender_ID = Judge_Data.DJI_Judge_Info.Judge_game_robot_status.robot_id;
		Judge_Student_Data_Header->receiver_ID = Target_ID;
		//学生数据帧头地址=起始地址+数据帧头长度+命令码长度+学生数据帧头长度
		uint8_t* Student_Data_Address = (uint8_t*)(Judge_Data.Judge_Usart_Tx_Buff+JUDGE_HEADER_LEN+JUDGE_CMDID_LEN+JUDGE_STUDENT_DATA_HEADER_LEN);
		//复制学生数据
		memcpy(Student_Data_Address,Stuent_Data,Student_Data_Len);
		//帧数据长度=学生数据长度+学生数据帧头长度
		uint16_t Data_Len = Student_Data_Len + JUDGE_STUDENT_DATA_HEADER_LEN;
		//数据帧头地址等于起始地址
		Judge_Frame_Header_t* Judge_Frame_Header = (Judge_Frame_Header_t*)Judge_Data.Judge_Usart_Tx_Buff;
		//设置数据帧头内容
		Judge_Frame_Header->cmd_id = Judge_CmdID_Robot_Interactive_Data;
		Judge_Frame_Header->sof = DJI_JUDGE_HEADER_SOF;
		Judge_Frame_Header->seq = Judge_Data.Judge_Data_Send_Count++;
		Judge_Frame_Header->data_length = Data_Len;
		append_crc8_check_sum(Judge_Data.Judge_Usart_Tx_Buff,JUDGE_HEADER_LEN,&Judge_Frame_Header->crc8);
		//数据帧总长=帧数据长度+数据帧头长度+命令码长度+数据帧尾长度
		uint16_t Data_Frame_Len = Data_Len + JUDGE_HEADER_LEN + JUDGE_CMDID_LEN +JUDGE_CRC16_LEN;
		//数据帧尾地址=起始地址+数据帧总长-数据帧尾长度
		uint16_t* CRC16 = (uint16_t*)(&Judge_Data.Judge_Usart_Tx_Buff[Data_Frame_Len-JUDGE_CRC16_LEN]);
		//设置数据帧尾内容：CRC16校验值
		append_crc16_check_sum(Judge_Data.Judge_Usart_Tx_Buff,Data_Frame_Len,CRC16);
		//DMA 发送
		Judge_Usart_DMA_Send(Data_Frame_Len);
}

void USART6_IRQHandler(void)
{

    if (USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
    {
			USART_ClearFlag(USART6,USART_FLAG_IDLE);
			USART_ReceiveData(USART6);

			if(DMA_GetCurrentMemoryTarget(DMA2_Stream1) == 0)
			{
					
					DMA_Cmd(DMA2_Stream1,DISABLE);
					DMA_SetCurrDataCounter(DMA2_Stream1,DJI_JUDGE_BUF_MAX_LEN);
					DMA2_Stream1->CR |= DMA_SxCR_CT;
					DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
					DMA_Cmd(DMA2_Stream1, ENABLE);
					Data_Parse_Handler(Judge_Data.Judge_Usart_Rx_Buff[0]);
			}
			else
			{
					DMA_Cmd(DMA2_Stream1,DISABLE);
					DMA_SetCurrDataCounter(DMA2_Stream1,DJI_JUDGE_BUF_MAX_LEN);
					DMA2_Stream1->CR &= ~(DMA_SxCR_CT);
					DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
					DMA_Cmd(DMA2_Stream1, ENABLE);
					Data_Parse_Handler(Judge_Data.Judge_Usart_Rx_Buff[1]);
						
			}
				
    }
//		else if(DMA_GetITStatus(DMA1_Stream2,DMA_IT_DMEIF2) != RESET)
//    {
//        USART_ReceiveData(USART6);
//    }
}

void DMA2_Stream7_IRQHandler(void)
{
		if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)
		{
				DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
				DMA_Cmd(DMA2_Stream7,DISABLE);
				xSemaphoreGiveFromISR(Judge_Data.Judge_Data_Send_Mutex,NULL);
		}
}
