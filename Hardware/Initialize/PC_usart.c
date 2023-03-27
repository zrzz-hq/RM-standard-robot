#include "PC_usart.h"

float vision_y_add = -1;//2.84201
float vision_x_add = 0;//-5.08175
float vision_x_fliter[5];
float vision_y_fliter[5];


//pitch = -18.02636784,yaw = -25.54673996
//left yaw -- right yaw ++
//up pitch -- down pitch ++

void PC_Usart_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3); 
	
	/* Configure USART1 as alternate function  */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_DeInit(USART3);
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);        //usart rx idle interrupt  enabled
	
	/* Enable USART */
  USART_Cmd(USART3, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
 
}

int rexflag = 0;
uint8_t angle_get_buf[16];
float Shoot_Delay;
float yaw_angle,pitch_angle;
float yaw_angle_change,pitch_angle_change;
float yaw_angle_get,pitch_angle_get;
float Armor_Long,Armor_Long_Get;
int Vision_Get_Flag;
void USART3_IRQHandler(void)                
{
	
        if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
        {

					 if(rexflag > 8)
						{
								rexflag = 0;
						}
					
						angle_get_buf[rexflag] = USART_ReceiveData(USART3);

						if(angle_get_buf[rexflag] == 's')
						{
							rexflag = 0;
						}
						
						if(rexflag > 8)
						{
							yaw_angle = (float)((angle_get_buf[3]) | (angle_get_buf[4]<<8));
							pitch_angle =(float)((angle_get_buf[1] ) | (angle_get_buf[2]<<8));
							Armor_Long =(float)((angle_get_buf[5] ) | (angle_get_buf[6]<<8));
							Shoot_Delay =(float)((angle_get_buf[7] ) | (angle_get_buf[8]<<8));
		//					Vision_Get_Flag = (int)angle_get_buf[7];
						}		
							
						rexflag++;	
							
						yaw_angle_change = -(yaw_angle * 100 / 32767);
						pitch_angle_change = -(pitch_angle * 100 / 32767);
						Armor_Long_Get = -(Armor_Long * 100 / 32767);
						
//						if(yaw_angle_change < -90)
//						{
//							yaw_angle_get = yaw_angle_change + 180 + vision_x_add;
//						}
//						else if(yaw_angle_change > -90 && yaw_angle_change<0)
//						{
//						  yaw_angle_get = yaw_angle_change + vision_x_add;
//							
//						}
//						else if(yaw_angle_change == 0)
//						{
//							yaw_angle_get = 0;
//						}						

//						if(pitch_angle_change < -90)
//						{
//							pitch_angle_get = pitch_angle_change + 180 + vision_y_add;							
//						}
//						else if(pitch_angle_change > -90 && yaw_angle_change<0)
//						{
//						 pitch_angle_get = pitch_angle_change + vision_y_add;
//						}
//						else if(pitch_angle_change == 0)
//						{
//						pitch_angle_get = 0;
//						}
						
        } 
//        USART_ClearFlag(USART6,USART_FLAG_TC);
}

