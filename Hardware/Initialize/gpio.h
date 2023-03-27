#ifndef _GPIO_H_
#define _GPIO_H_
#include "main.h"
#include "stm32f4xx.h"


#define Shoot_Key_Read()  GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_0)
#define Usr_Key_Read()  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
#define Laser_On() GPIO_SetBits(GPIOG, GPIO_Pin_13)
#define Laser_Off() GPIO_ResetBits(GPIOG, GPIO_Pin_13)

void Usr_Key_Init(void);
void Usr_Key_Register_Callback(void* Callback);
void Shoot_Key_Init(void);
void Laser_Configuration(void);


#endif



//KEY_IO
