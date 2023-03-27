#ifndef __SPI_H
#define __SPI_H
#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
    										
void GPIO_SPI_Init(void);  
void SPI1_Init(void);	

void SPI5_Init(void);

void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
void OLED_SPI_SendData(uint8_t data);
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
		 
int IMU_SPI5_ReadByte(void);
void IMU_SPI5_SendData(uint8_t data);		 
		 
//官方代码陀螺仪初始化		 
void SPI5Init(void);
void SPI5SetSpeedAndDataSize(uint16_t Speed, uint16_t DataSize);
void SPI5_DMA_Init(uint32_t tx_buf, uint32_t rx_buf, uint16_t num);
void SPI5_DMA_Enable(uint16_t ndtr);		 
		 
		 
#endif

