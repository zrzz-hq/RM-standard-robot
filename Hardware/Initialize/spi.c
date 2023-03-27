#include "spi.h"

void GPIO_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE );//PORTB时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE );	
//				GPIO_ResetBits(GPIOB, OLED_DC_Pin|OLED_RST_Pin);
	GPIO_WriteBit(GPIOB, OLED_DC_Pin,Bit_RESET);
	GPIO_WriteBit(GPIOB, OLED_RST_Pin,Bit_RESET);
	GPIO_WriteBit(GPIOF, GPIO_Pin_6,Bit_SET);
	GPIO_InitStructure.GPIO_Pin = OLED_DC_Pin|OLED_RST_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //PB4/3复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	

}

void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1时钟使能 	
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA, ENABLE );//PORTB时钟使能 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //PB4/3复用推挽输出 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //PA7复用推挽输出 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 10;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
//	SPI1_ReadWriteByte(0xff);//启动传输		 
 
} 

void OLED_SPI_SendData(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
	}
  SPI_I2S_SendData(SPI1, data);
//	delay_us(1);
}


uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}


    /**SPI5 GPIO Configuration    
    PF7     ------> SPI5_SCK
    PF9     ------> SPI5_MOSI
    PF8     ------> SPI5_MISO 
    */
void SPI5_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOF, ENABLE );//PORTF时钟使能 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI5,  ENABLE );//SPI5时钟使能 	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_SPI5); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_SPI5); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_SPI5); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;//PF7/8/9复用推挽输出 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF
	
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI5, &SPI_InitStructure);
			SPI_Cmd(SPI5, ENABLE); //使能SPI外设
}

void IMU_SPI5_SendData(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET)
	{
	}
  SPI_I2S_SendData(SPI5, data);
}

int IMU_SPI5_ReadByte(void)
{
while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		}	
		return SPI_I2S_ReceiveData(SPI5);
}
#include "spi.h"
#include "stm32f4xx.h"

void SPI5Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5, DISABLE);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI5, &SPI_InitStructure);

    SPI_Cmd(SPI5, ENABLE);
}

void SPI5SetSpeedAndDataSize(uint16_t Speed, uint16_t DataSize)
{
    SPI5->CR1 &= 0xF7C7;
    SPI5->CR1 |= Speed;
    SPI5->CR1 |= DataSize;
    SPI5->CR1 |= 1 << 6;
}

void SPI5_DMA_Init(uint32_t tx_buf, uint32_t rx_buf, uint16_t num)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    /* -------------- Enable Module Clock Source ----------------------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_DeInit(DMA2_Stream5);

    while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE)
    {
        ;
    }
    DMA_InitStructure.DMA_Channel = DMA_Channel_7;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI5->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = rx_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = num;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream5, &DMA_InitStructure);

    DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA2_Stream5, DISABLE); //Add a disable

    DMA_DeInit(DMA2_Stream4);

    while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE)
    {
        ;
    }
    DMA_InitStructure.DMA_Channel = DMA_Channel_2;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI5->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = tx_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = num;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream4, &DMA_InitStructure);

    DMA_Cmd(DMA2_Stream4, DISABLE); //Add a disable
}

void SPI5_DMA_Enable(uint16_t ndtr)
{
    DMA_Cmd(DMA2_Stream4, DISABLE);
    DMA_Cmd(DMA2_Stream5, DISABLE);
    while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE)
    {
        ;
    }
    while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE)
    {
        ;
    }

    DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5);
    DMA_ClearFlag(DMA2_Stream4, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4);

    DMA_SetCurrDataCounter(DMA2_Stream4, ndtr);
    DMA_SetCurrDataCounter(DMA2_Stream5, ndtr);
    DMA_Cmd(DMA2_Stream5, ENABLE);
    DMA_Cmd(DMA2_Stream4, ENABLE);
}
