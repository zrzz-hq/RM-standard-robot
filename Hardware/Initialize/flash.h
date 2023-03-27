#ifndef __FLASH_H__
#define __FLASH_H__
#include "stm32f4xx.h"


//**×Ö**//
int Flash_Wordbyte_Write(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num);
uint32_t Flash_Wordbyte_Read(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num);
///*°ë×Ö**///
int Flash_HalfWordbyte_Write(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num);
uint16_t Flash_HalfWordbyte_Read(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num);
///*×Ö½Ú**///
int Flash_Byte_Write(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num);
uint8_t Flash_Byte_Read(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num);

#endif
