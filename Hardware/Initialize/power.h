#ifndef __POWER_H__
#define __POWER_H__

#define POWER1_24V_OFF()      GPIO_ResetBits(GPIOH, GPIO_Pin_2)
#define POWER1_24V_ON()     	GPIO_SetBits(GPIOH, GPIO_Pin_2)

#define POWER2_24V_OFF()      GPIO_ResetBits(GPIOH, GPIO_Pin_4)
#define POWER2_24V_ON()     	GPIO_SetBits(GPIOH, GPIO_Pin_4)

#define POWER3_24V_OFF()      GPIO_ResetBits(GPIOH, GPIO_Pin_3)
#define POWER3_24V_ON()     	GPIO_SetBits(GPIOH, GPIO_Pin_3)

#define POWER4_24V_OFF()      GPIO_ResetBits(GPIOH, GPIO_Pin_5)
#define POWER4_24V_ON()     	GPIO_SetBits(GPIOH, GPIO_Pin_5)


typedef enum
{
	Power_ON,
	Power_OFF
}Power_State_t;

typedef enum
{
	Power_24V_Channel_1 = 1,
	Power_24V_Channel_2 = 2,
	Power_24V_Channel_3 = 3,
	Power_24V_Channel_4 = 4
}Power_24V_Channel_t;


void Power_24V_Init(void);

void Power_24v_switch(Power_24V_Channel_t Power_24V_Channel,Power_State_t Power_State);

#endif

