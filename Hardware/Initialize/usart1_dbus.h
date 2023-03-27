#ifndef __USART1_DBUS_H__
#define __USART1_DBUS_H__

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"


typedef enum{
		
		L_KEY = 0,
		R_KEY = 1,
		R2 = 2,
		R1 = 3,
		RU = 4,
		RR = 5,
		RD = 6,
		RL = 7,
		ST = 8,
		SE = 9,
		LR = 10,
		LD = 11,
		LL = 12,
		LU = 13,
		L2 = 14,
		L1 = 15,
		None = 16
	
}Osker_Remote_Control_Data_Key_t;


typedef struct{
	struct{ 
		int status;
		int16_t ch0;
		int16_t ch1;
		int16_t ch2;
		int16_t ch3;
	}rc;
 struct{
		uint16_t v;
	}key;
}Osker_Remote_Control_Struct;

extern const Osker_Remote_Control_Struct *Osker_Remote_Control_Address(void);

extern volatile unsigned char Osker_Remote_Control_Buff[25];

void Osker_Remote_Control_Init(void);

void Usart1_Dbus_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);

void RC_restart(uint16_t dma_buf_num);

//void Usart1_Dbus_Init(void);

#endif 
