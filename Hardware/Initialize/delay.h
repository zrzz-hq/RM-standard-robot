#ifndef __DELAY_H__
#define __DELAY_H__
#include "main.h"
extern void delay_init(uint32_t TICK_RATE_HZ);
extern void delay_us(uint16_t nus);
extern void delay_ms(uint16_t nms);
//void delay_ms(unsigned int t);
//void delay_us(__IO u32 nTime);
void TimingDelay_Decrement(void);
void SysTick_Init(void);
#endif 
