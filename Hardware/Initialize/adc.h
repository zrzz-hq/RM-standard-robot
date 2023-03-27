#ifndef ADC_H
#define ADC_H
#include "stm32f4xx.h"

typedef enum
{
		KEY_NONE,
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_ENTER
}Adc_Key_State_t;


void ADC1_Init(void);
Adc_Key_State_t Oled_Key_Read(void);

#endif
