#include "can.h"

Motor_Msg_t ALL_Motor_Msg[16];
Super_C_Msg_t Super_C_Msg;

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, DISABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 5;
    CAN_Init(CAN1, &CAN_InitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, DISABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 5;
    CAN_Init(CAN2, &CAN_InitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber = 14;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void CAN1_Super_C_Send(int16_t Stdid,uint16_t data)
{
    CanTxMsg tx_message;
    tx_message.RTR = CAN_RTR_DATA;  //数据帧
    tx_message.IDE = CAN_ID_STD;    //标准帧
    tx_message.DLC = 0x08;          //帧长度为   
	
		tx_message.StdId = Stdid;
		tx_message.Data[0] =(uint8_t)((data>> 8)&0xff);
		tx_message.Data[1] =(uint8_t)(data&0xff);
		
    CAN_Transmit(CAN1,&tx_message);
}

void CAN1_Send(int16_t Stdid,uint8_t * data)     //车轮电机
{
    CanTxMsg tx_message;
    tx_message.RTR = CAN_RTR_DATA;  //数据帧
    tx_message.IDE = CAN_ID_STD;    //标准帧
    tx_message.DLC = 0x08;          //帧长度为   
	
		tx_message.StdId = Stdid;
		tx_message.Data[0] = data[0];
		tx_message.Data[1] = data[1];
    tx_message.Data[2] = data[2];
    tx_message.Data[3] = data[3];
		tx_message.Data[4] = data[4];
    tx_message.Data[5] = data[5];
		tx_message.Data[6] = data[6];
    tx_message.Data[7] = data[7];
		
    CAN_Transmit(CAN1,&tx_message);
}

void CAN2_Send(int16_t Stdid,uint8_t * data)     //车轮电机
{
    CanTxMsg tx_message;
    tx_message.RTR = CAN_RTR_DATA;  //数据帧
    tx_message.IDE = CAN_ID_STD;    //标准帧
    tx_message.DLC = 0x08;          //帧长度为   
	
		tx_message.StdId = Stdid;
		tx_message.Data[0] = data[0];
		tx_message.Data[1] = data[1];
    tx_message.Data[2] = data[2];
    tx_message.Data[3] = data[3];
		tx_message.Data[4] = data[4];
    tx_message.Data[5] = data[5];
		tx_message.Data[6] = data[6];
    tx_message.Data[7] = data[7];
		
    CAN_Transmit(CAN2,&tx_message);
}


void CAN1_Motor_Control(int16_t stdid,u16 num1,u16 num2,u16 num3,u16 num4)
{
    CanTxMsg tx_message;
    tx_message.RTR = CAN_RTR_DATA;  //数据帧
    tx_message.IDE = CAN_ID_STD;    //标准帧
    tx_message.DLC = 0x08;          //帧长度为   
		tx_message.StdId = stdid;
		tx_message.Data[0] =(unsigned char)((num1>> 8)&0xff);
		tx_message.Data[1] =(unsigned char)(num1&0xff);
    tx_message.Data[2] =(unsigned char)((num2 >> 8)&0xff);
    tx_message.Data[3] =(unsigned char)(num2&0xff);
		tx_message.Data[4] =(unsigned char)((num3 >> 8)&0xff);
    tx_message.Data[5] =(unsigned char)(num3&0xff);
		tx_message.Data[6] =(unsigned char)((num4 >> 8)&0xff);
    tx_message.Data[7] =(unsigned char)(num4&0xff);
	
   CAN_Transmit(CAN1,&tx_message);
}


void CAN2_Motor_Control(int16_t stdid,u16 num1,u16 num2,u16 num3,u16 num4)
{
    CanTxMsg tx_message;
    tx_message.RTR = CAN_RTR_DATA;  //数据帧
    tx_message.IDE = CAN_ID_STD;    //标准帧
    tx_message.DLC = 0x08;          //帧长度为   
		tx_message.StdId = stdid;
		tx_message.Data[0] =(unsigned char)((num1>> 8)&0xff);
		tx_message.Data[1] =(unsigned char)(num1&0xff);
    tx_message.Data[2] =(unsigned char)((num2 >> 8)&0xff);
    tx_message.Data[3] =(unsigned char)(num2&0xff);
		tx_message.Data[4] =(unsigned char)((num3 >> 8)&0xff);
    tx_message.Data[5] =(unsigned char)(num3&0xff);
		tx_message.Data[6] =(unsigned char)((num4 >> 8)&0xff);
    tx_message.Data[7] =(unsigned char)(num4&0xff);
	
   CAN_Transmit(CAN2,&tx_message);
}

/*************************************************************************
                          CAN1_RX0_IRQHandler
描述：CAN1的接收中断函数
*************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg Can1_Rx_Msg;
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)	//判断是否接收结束
		{
				CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		//清除FIFO0消息挂号中断标志位
        CAN_Receive(CAN1, CAN_FIFO0, &Can1_Rx_Msg);	//获取接收数据
				CAN1_Msg_Parser(&Can1_Rx_Msg);				//将数据通过函数处理  		
    }
}

/*************************************************************************
                          CAN2_RX0_IRQHandler
描述：CAN2的接收中断函数
*************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg Can2_Rx_Msg;
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)	//判断是否接收结束
		{
				CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);		//清除FIFO0消息挂号中断标志位
        CAN_Receive(CAN2, CAN_FIFO0, &Can2_Rx_Msg);	//获取接收数据
				CAN2_Msg_Parser(&Can2_Rx_Msg);					//将数据通过函数处理
    }
}

Motor_Msg_t *Get_DJI_Motor_Data(CAN_Set_t CAN_Set,uint16_t Motor_Stdid)
{
	switch(CAN_Set)
	{
		case CAN1_RX:
			switch(Motor_Stdid)
			{
				case 0x201:
					return &ALL_Motor_Msg[0];
				case 0x202:
					return &ALL_Motor_Msg[1];
				case 0x203:
					return &ALL_Motor_Msg[2];
				case 0x204:
					return &ALL_Motor_Msg[3];
				case 0x205:
					return &ALL_Motor_Msg[4];
				case 0x206:
					return &ALL_Motor_Msg[5];
				case 0x207:
					return &ALL_Motor_Msg[6];
				case 0x208:
					return &ALL_Motor_Msg[7];
			}
			break;
		case CAN2_RX:
			switch(Motor_Stdid)
			{
				case 0x201:
					return &ALL_Motor_Msg[8];
				case 0x202:
					return &ALL_Motor_Msg[9];
				case 0x203:
					return &ALL_Motor_Msg[10];
				case 0x204:
					return &ALL_Motor_Msg[11];
				case 0x205:
					return &ALL_Motor_Msg[12];
				case 0x206:
					return &ALL_Motor_Msg[13];
				case 0x207:
					return &ALL_Motor_Msg[14];
				case 0x208:
					return &ALL_Motor_Msg[15];
			}
			break;
		}
	return 0;
}

Super_C_Msg_t *Get_Cap_Data(void)
{
	return &Super_C_Msg;
}

//从CAN1总线上接受编码器数据
void CAN1_Msg_Parser(CanRxMsg *Can1_Rx_Msg) 	//传入接收到数据的指针
{
		float Angle;
//	CanRxMsg rx_message;
	switch(Can1_Rx_Msg->StdId)	//判断标识符（根据电调说明书）
	{
		case 0x201:				
					ALL_Motor_Msg[0].Angle=(Can1_Rx_Msg->Data[0] << 8) + (Can1_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[0].Speed=(Can1_Rx_Msg->Data[2] << 8) + (Can1_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[0].Current=(Can1_Rx_Msg->Data[4] << 8) + (Can1_Rx_Msg->Data[5]);
					ALL_Motor_Msg[0].Temp=(Can1_Rx_Msg->Data[6]);
			break;
		case 0x202:				
					ALL_Motor_Msg[1].Angle=(Can1_Rx_Msg->Data[0] << 8) + (Can1_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[1].Speed=(Can1_Rx_Msg->Data[2] << 8) + (Can1_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[1].Current=(Can1_Rx_Msg->Data[4] << 8) + (Can1_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[1].Temp=(Can1_Rx_Msg->Data[6]);
			break;	
		case 0x203:				
					ALL_Motor_Msg[2].Angle=(Can1_Rx_Msg->Data[0] << 8) + (Can1_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[2].Speed=(Can1_Rx_Msg->Data[2] << 8) + (Can1_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[2].Current=(Can1_Rx_Msg->Data[4] << 8) + (Can1_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[2].Temp=(Can1_Rx_Msg->Data[6]);
			break;	
		case 0x204:				
					ALL_Motor_Msg[3].Angle=(Can1_Rx_Msg->Data[0] << 8) + (Can1_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[3].Speed=(Can1_Rx_Msg->Data[2] << 8) + (Can1_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[3].Current=(Can1_Rx_Msg->Data[4] << 8) + (Can1_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[3].Temp=(Can1_Rx_Msg->Data[6]);
			break;	
		case 0x205:	
					ALL_Motor_Msg[4].Angle=(Can1_Rx_Msg->Data[0] << 8) + (Can1_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[4].Speed=(Can1_Rx_Msg->Data[2] << 8) + (Can1_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[4].Current=(Can1_Rx_Msg->Data[4] << 8) + (Can1_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[4].Temp=(Can1_Rx_Msg->Data[6]);
			break;	
		case 0x206:	
					ALL_Motor_Msg[5].Angle=(Can1_Rx_Msg->Data[0] << 8) + (Can1_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[5].Speed=(Can1_Rx_Msg->Data[2] << 8) + (Can1_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[5].Current=(Can1_Rx_Msg->Data[4] << 8) + (Can1_Rx_Msg->Data[5]);
					ALL_Motor_Msg[5].Temp=(Can1_Rx_Msg->Data[6]);		
			break;	
		case 0x207:				
					ALL_Motor_Msg[6].Angle=(Can1_Rx_Msg->Data[0] << 8) + (Can1_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[6].Speed=(Can1_Rx_Msg->Data[2] << 8) + (Can1_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[6].Current=(Can1_Rx_Msg->Data[4] << 8) + (Can1_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[6].Temp=(Can1_Rx_Msg->Data[6]);
		break;	
		case 0x208:				
					ALL_Motor_Msg[7].Angle=(Can1_Rx_Msg->Data[0] << 8) + (Can1_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[7].Speed=(Can1_Rx_Msg->Data[2] << 8) + (Can1_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[7].Current=(Can1_Rx_Msg->Data[4] << 8) + (Can1_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[7].Temp=(Can1_Rx_Msg->Data[6]);
			break;	
		case 0x211:				
					Super_C_Msg.Voilt_In=(uint16_t)((Can1_Rx_Msg->Data[1] << 8) + (Can1_Rx_Msg->Data[0]));//输入电压
					Super_C_Msg.Voilt_C=(uint16_t)((Can1_Rx_Msg->Data[3] << 8) + (Can1_Rx_Msg->Data[2]));//电容电压
					Super_C_Msg.Current_In=(uint16_t)((Can1_Rx_Msg->Data[5] << 8) + (Can1_Rx_Msg->Data[4]));//输入电流	
					Super_C_Msg.Target_power=((uint16_t)(Can1_Rx_Msg->Data[7] << 8) + (Can1_Rx_Msg->Data[6]));//目标功率	
			break;
		
		}
}

void CAN2_Msg_Parser(CanRxMsg *Can2_Rx_Msg) 	//传入接收到数据的指针
{
		float Angle;
//	CanRxMsg rx_message;
	switch(Can2_Rx_Msg->StdId)	//判断标识符（根据电调说明书）
	{
		case 0x201:				
					ALL_Motor_Msg[8].Angle=(Can2_Rx_Msg->Data[0] << 8) + (Can2_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[8].Speed=(Can2_Rx_Msg->Data[2] << 8) + (Can2_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[8].Current=(Can2_Rx_Msg->Data[4] << 8) + (Can2_Rx_Msg->Data[5]);
					ALL_Motor_Msg[8].Temp=(Can2_Rx_Msg->Data[6]);
			break;
		case 0x202:				
					ALL_Motor_Msg[9].Angle=(Can2_Rx_Msg->Data[0] << 8) + (Can2_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[9].Speed=(Can2_Rx_Msg->Data[2] << 8) + (Can2_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[9].Current=(Can2_Rx_Msg->Data[4] << 8) + (Can2_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[9].Temp=(Can2_Rx_Msg->Data[6]);
			break;	
		case 0x203:				
					ALL_Motor_Msg[10].Angle=(Can2_Rx_Msg->Data[0] << 8) + (Can2_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[10].Speed=(Can2_Rx_Msg->Data[2] << 8) + (Can2_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[10].Current=(Can2_Rx_Msg->Data[4] << 8) + (Can2_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[10].Temp=(Can2_Rx_Msg->Data[6]);
			break;	
		case 0x204:				
					ALL_Motor_Msg[11].Angle=(Can2_Rx_Msg->Data[0] << 8) + (Can2_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[11].Speed=(Can2_Rx_Msg->Data[2] << 8) + (Can2_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[11].Current=(Can2_Rx_Msg->Data[4] << 8) + (Can2_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[11].Temp=(Can2_Rx_Msg->Data[6]);
			break;	
		case 0x205:	
					ALL_Motor_Msg[12].Angle=(Can2_Rx_Msg->Data[0] << 8) + (Can2_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[12].Speed=(Can2_Rx_Msg->Data[2] << 8) + (Can2_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[12].Current=(Can2_Rx_Msg->Data[4] << 8) + (Can2_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[12].Temp=(Can2_Rx_Msg->Data[6]);
			break;	
		case 0x206:			
					ALL_Motor_Msg[13].Angle=(Can2_Rx_Msg->Data[0] << 8) + (Can2_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[13].Speed=(Can2_Rx_Msg->Data[2] << 8) + (Can2_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[13].Current=(Can2_Rx_Msg->Data[4] << 8) + (Can2_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[13].Temp=(Can2_Rx_Msg->Data[6]);
			break;	
		case 0x207:				
					ALL_Motor_Msg[14].Angle=(Can2_Rx_Msg->Data[0] << 8) + (Can2_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[14].Speed=(Can2_Rx_Msg->Data[2] << 8) + (Can2_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[14].Current=(Can2_Rx_Msg->Data[4] << 8) + (Can2_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[14].Temp=(Can2_Rx_Msg->Data[6]);
			break;	
		case 0x208:				
					ALL_Motor_Msg[15].Angle=(Can2_Rx_Msg->Data[0] << 8) + (Can2_Rx_Msg->Data[1]);//电机1的机械角度值 
					ALL_Motor_Msg[15].Speed=(Can2_Rx_Msg->Data[2] << 8) + (Can2_Rx_Msg->Data[3]);//电机1的速度值
					ALL_Motor_Msg[15].Current=(Can2_Rx_Msg->Data[4] << 8) + (Can2_Rx_Msg->Data[5]);	
					ALL_Motor_Msg[15].Temp=(Can2_Rx_Msg->Data[6]);
			break;	
		
		}
}

