#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "usart.h"
#include "pwm.h"
#include "led.h"
#include "can.h"
#include "adc.h"
#include "usart1_dbus.h"
#include "oled.h"
#include "oled_mode.h"
#include "spi.h"
#include "gpio.h"
#include "flash.h"
#include "buzzer.h"

#include "delay.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define PI 3.14159265358979f

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;


void MPU6500_GET(void);
void sys_assert_fault_handler();


#endif 
