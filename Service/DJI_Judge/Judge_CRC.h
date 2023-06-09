#ifndef _JUDGE_CEC_H_
#define _JUDGE_CEC_H_

#include "main.h"
#include "stm32f4xx.h"

uint8_t verify_crc8_check_sum(uint8_t* pchMessage, uint16_t dwLength, uint8_t ucReal);
uint8_t verify_crc16_check_sum(uint8_t* pchMessage, uint32_t dwLength, uint16_t wReal);

void append_crc8_check_sum(uint8_t* pchMessage, uint16_t dwLength, uint8_t* pucCRC);
void append_crc16_check_sum(uint8_t* pchMessage, uint32_t dwLength, uint16_t* pwCRC);

#endif

