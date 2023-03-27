#ifndef __PWM_H__
#define __PWM_H__
#include "stm32f4xx.h"

/* 
* APB1 prescaler = 4, APB1 TIMER Frequency = 2*45 = 90MHz
* APB2 prescaler = 2, APB2 TIMER Frequency = 2*90 = 180MHz
*/
#define TIM_PSC_APB1 ((APB1_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1)
#define PWM_FREQUENCE 100             //Ê±ÖÓ   
#define PWM_RESOLUTION 10000         // 50hz,SERVO_DEFAULT_DUTY = 1500ms* 50 / 100
#define SERVO_DEFAULT_DUTY 1500      // 50hz,1500us,1.5ms
#define TIM_PSC_APB2 ((APB2_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1)
#define APB1_TIMER_CLOCKS 90000000
#define APB2_TIMER_CLOCKS 180000000

void TIM1_Init(void);
void TIM8_Init(void);
void TIM2_Init(void);
void TIM4_Init(void);
void TIM5_Init(void);

//ÍÓÂÝÒÇÎÂ¶ÈPID
void TIM3_Init(uint16_t arr, uint16_t psc);

#endif
