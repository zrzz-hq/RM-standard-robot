#include "usart1_dbus.h"

volatile unsigned char sbus_rx_buffer[25]; 

//遥控器数据结构体;
Osker_Remote_Control_Struct Osker_Remote_Control_Data;

Osker_Remote_Control_Data_Key_t Osker_Remote_Control_Data_Key;
Osker_Remote_Control_Data_Key_t Osker_Remote_Control_Data_Key_Last;



void Usart1_Dbus_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
	        /* -------------- Enable Module Clock Source ----------------------------*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);

        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1); //PB7  usart1 rx
                                                                  /* -------------- Configure GPIO ---------------------------------------*/
        {
                GPIO_InitTypeDef GPIO_InitStructure;
                USART_InitTypeDef USART_InitStructure;
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
                GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
                GPIO_Init(GPIOB, &GPIO_InitStructure);

                USART_DeInit(USART1);

                USART_InitStructure.USART_BaudRate = 100000;
                USART_InitStructure.USART_WordLength = USART_WordLength_8b;
                USART_InitStructure.USART_StopBits = USART_StopBits_1;
                USART_InitStructure.USART_Parity = USART_Parity_Even;
                USART_InitStructure.USART_Mode = USART_Mode_Rx;
                USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                USART_Init(USART1, &USART_InitStructure);

                USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

                USART_ClearFlag(USART1, USART_FLAG_IDLE);
                USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

                USART_Cmd(USART1, ENABLE);
        }

        /* -------------- Configure NVIC ---------------------------------------*/
        {
                NVIC_InitTypeDef NVIC_InitStructure;
                NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
                NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
                NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStructure);
        }

        //DMA2 stream5 ch4  or (DMA2 stream2 ch4)    !!!!!!! P206 of the datasheet
        /* -------------- Configure DMA -----------------------------------------*/
        {
                DMA_InitTypeDef DMA_InitStructure;
                DMA_DeInit(DMA2_Stream2);

                DMA_InitStructure.DMA_Channel = DMA_Channel_4;
                DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
                DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
                DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
                DMA_InitStructure.DMA_BufferSize = dma_buf_num;
                DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
                DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
                DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
                DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
                DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
                DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
                DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
                DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
                DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
                DMA_Init(DMA2_Stream2, &DMA_InitStructure);
                DMA_DoubleBufferModeConfig(DMA2_Stream2, (uint32_t)rx2_buf, DMA_Memory_0);
                DMA_DoubleBufferModeCmd(DMA2_Stream2, ENABLE);
                DMA_Cmd(DMA2_Stream2, DISABLE); //Add a disable
                DMA_Cmd(DMA2_Stream2, ENABLE);
        }
	
//    USART_InitTypeDef usart1;
//	  GPIO_InitTypeDef  gpio;
//    NVIC_InitTypeDef  nvic;
//		DMA_InitTypeDef   dma;
//	
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//    
//    //GPIO_PinAFConfig(GPIOB,GPIO_PinSource6 ,GPIO_AF_USART1);
//		GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); 
//	
//    gpio.GPIO_Pin = GPIO_Pin_7;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOB,&gpio);
//		
//		USART_DeInit(USART1);
//    usart1.USART_BaudRate = 100000;
//    usart1.USART_WordLength = USART_WordLength_8b;
//    usart1.USART_StopBits = USART_StopBits_1;
//    //usart1.USART_Parity = USART_Parity_No;
//		usart1.USART_Parity = USART_Parity_Even;
//    //usart1.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
//		usart1.USART_Mode =USART_Mode_Rx;
//    usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_Init(USART1,&usart1);

//    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
//    USART_Cmd(USART1,ENABLE);
//		
//		USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
//   
//	  nvic.NVIC_IRQChannel = DMA2_Stream5_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 0;
//    nvic.NVIC_IRQChannelSubPriority = 0;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);
//		
//		DMA_Cmd(DMA2_Stream5, DISABLE);
//    while (DMA2_Stream5->CR & DMA_SxCR_EN);
//		DMA_DeInit(DMA2_Stream5);
//    dma.DMA_Channel= DMA_Channel_4;
//    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
//    dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
//    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
//    dma.DMA_BufferSize = 18;
//    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    dma.DMA_Mode = DMA_Mode_Circular;
//    dma.DMA_Priority = DMA_Priority_VeryHigh;
//    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//    dma.DMA_MemoryBurst = DMA_Mode_Normal;
//    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//    DMA_Init(DMA2_Stream5,&dma);
//		
//		DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
//    DMA_Cmd(DMA2_Stream5,ENABLE);
//		
}




//遥控器数据缓冲区;
volatile unsigned char Osker_Remote_Control_Buff[25];

//初始化遥控器，占用USART1，初始化后USART1不可作为其他用途;
void Osker_Remote_Control_Init(void){
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_DMA2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);

	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	USART_DeInit(USART6);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx;

  USART_Init(USART6, &USART_InitStructure);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); 
	USART_Cmd(USART6,ENABLE);
	
	USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(DMA2_Stream2, DISABLE);
	while (DMA2_Stream2->CR & DMA_SxCR_EN);
	DMA_DeInit(DMA2_Stream2);
	DMA_InitStructure.DMA_Channel= DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART6->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Osker_Remote_Control_Buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 13;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream2,&DMA_InitStructure);
	
	DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream2,ENABLE);
}

void RC_restart(uint16_t dma_buf_num)
{
        USART_Cmd(USART1, DISABLE);
        DMA_Cmd(DMA2_Stream2, DISABLE);
        DMA_SetCurrDataCounter(DMA2_Stream2, dma_buf_num);

        USART_ClearFlag(USART1, USART_FLAG_IDLE);

        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
        DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
        DMA_Cmd(DMA2_Stream2, ENABLE);
        USART_Cmd(USART1, ENABLE);
}


/*

//DMA中断,直接传输数据，不经过CPU;
void DMA2_Stream2_IRQHandler(void){
	if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2)){
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
		if(Osker_Remote_Control_Buff[0] == 0xAA && Osker_Remote_Control_Buff[1] == 0xAA)
		{
			//右侧摇杆左右，中心为1024
			Osker_Remote_Control_Data.rc.ch0 = (uint16_t)Osker_Remote_Control_Buff[5];
			
			//右侧摇杆上下，中心为1024
			Osker_Remote_Control_Data.rc.ch1 = (uint16_t)Osker_Remote_Control_Buff[6];
			
			//左侧摇杆左右，中心为1024
			Osker_Remote_Control_Data.rc.ch2 = (uint16_t)Osker_Remote_Control_Buff[7];
			
			//左侧摇杆上下，中心为1024
			Osker_Remote_Control_Data.rc.ch3 = (uint16_t)Osker_Remote_Control_Buff[8];
			
			Osker_Remote_Control_Data.key.v = (uint16_t)(Osker_Remote_Control_Buff[10] | (Osker_Remote_Control_Buff[9] << 8));
			
			
			
			
			if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 0))
				Osker_Remote_Control_Data_Key = L_KEY;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 1))
				Osker_Remote_Control_Data_Key = R_KEY;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 2))
				Osker_Remote_Control_Data_Key = R2;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 3))
				Osker_Remote_Control_Data_Key = R1;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 4))
				Osker_Remote_Control_Data_Key = RU;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 5))
				Osker_Remote_Control_Data_Key = RR;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 6))
				Osker_Remote_Control_Data_Key = RD;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 7))
				Osker_Remote_Control_Data_Key = RL;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 8))
				Osker_Remote_Control_Data_Key = ST;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 9))
				Osker_Remote_Control_Data_Key = SE;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 10))
				Osker_Remote_Control_Data_Key = LR;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 11))
				Osker_Remote_Control_Data_Key = LD;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 12))
				Osker_Remote_Control_Data_Key = LL;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 13))
				Osker_Remote_Control_Data_Key = LU;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 14))
				Osker_Remote_Control_Data_Key = L2;
			else if(Osker_Remote_Control_Data.key.v & ((uint16_t)1 << 15))
				Osker_Remote_Control_Data_Key = L1;
			else
				Osker_Remote_Control_Data_Key = None;		
			
//			if((Osker_Remote_Control_Data_Key = L_KEY)&&(Osker_Remote_Control_Data_Key_Last == None))
//			{
//				
//			}

			
			

			//通过USB输出实时调试数据
			//USART6_printf("{\"type\":\"REMOTE-CONTROL-READ\",\"Ch0\":%d,\"Ch1\":%d,\"Ch2\":%d,\"Ch3\":%d,\"Key\":%d}\r\n",\
			Osker_Remote_Control_Data.rc.ch0,Osker_Remote_Control_Data.rc.ch1,Osker_Remote_Control_Data.rc.ch2,Osker_Remote_Control_Data.rc.ch3,\
			Osker_Remote_Control_Data.key.v);
			
			Osker_Remote_Control_Data.rc.ch0 -= ((uint16_t)127);
			Osker_Remote_Control_Data.rc.ch1 -= ((uint16_t)127);
			Osker_Remote_Control_Data.rc.ch2 -= ((uint16_t)127);
			Osker_Remote_Control_Data.rc.ch3 -= ((uint16_t)127);
			
			if(Osker_Remote_Control_Data.rc.ch2 != 0 || Osker_Remote_Control_Data.rc.ch3 != 0){
				Osker_Remote_Control_Data.rc.status = 1;
			}
			Osker_Remote_Control_Data_Key_Last = Osker_Remote_Control_Data_Key;
		}

	}
}

*/
