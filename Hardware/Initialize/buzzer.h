#ifndef __BUZZER_H_
#define __BUZZER_H_
#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
    										
extern void buzzer_init(uint16_t arr, uint16_t psc);
extern void buzzer_on(uint16_t psc, uint16_t pwm);
extern void buzzer_off(void);
		 
#endif

