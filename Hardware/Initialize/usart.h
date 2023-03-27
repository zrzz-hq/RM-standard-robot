#ifndef __USART_H__
#define __USART_H__

#include "main.h"
#include "stm32f4xx.h"

void USART1_Init(void);
void USART2_Init(void);
void USART3_Init(void);
void USART6_Init(void);
void UART7_Init(void);
void UART8_Init(void);


void USART1_SendData(uint8_t data);
void USART2_SendData(uint8_t data);
void USART3_SendData(uint8_t data);
void USART6_SendData(uint8_t data);
void UART7_SendData(uint8_t data);
void UART8_SendData(uint8_t data);

void USART1_printf (char *fmt, ...);
void USART2_printf (char *fmt, ...);
void USART3_printf (char *fmt, ...);
void USART6_printf (char *fmt, ...);
void UART7_printf (char *fmt, ...);
void UART8_printf (char *fmt, ...);



#endif
