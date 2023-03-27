#ifndef __LED_H__
#define __LED_H__

#include "main.h"

//void Led_Configuration(void);

#define  LED_RED_ON()      GPIO_ResetBits(GPIOE, GPIO_Pin_11)
#define  LED_RED_OFF()     GPIO_SetBits(GPIOE, GPIO_Pin_11)
#define  LED_RED_TOGGLE()  GPIO_ToggleBits(GPIOE, GPIO_Pin_11)

#define  LED_GREEN_ON()        GPIO_ResetBits(GPIOF, GPIO_Pin_14)
#define  LED_GREEN_OFF()       GPIO_SetBits(GPIOF, GPIO_Pin_14)
#define  LED_GREEN_TOGGLE()    GPIO_ToggleBits(GPIOF, GPIO_Pin_14)

#define  DXT_L()			GPIO_ResetBits(GPIOF, GPIO_Pin_0);
#define  DXT_H()			GPIO_SetBits(GPIOF, GPIO_Pin_0);

void Led_Init(void);

extern void led_green_on(void);
extern void led_green_off(void);
extern void led_green_toggle(void);

extern void led_red_on(void);
extern void led_red_off(void);
extern void led_red_toggle(void);

extern void flow_led_on(uint16_t num);
extern void flow_led_off(uint16_t num);
extern void flow_led_toggle(uint16_t num);

#endif
