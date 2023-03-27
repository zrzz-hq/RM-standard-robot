#ifndef _oled_mode_h_
#define _oled_mode_h_
#include "main.h"

//#define oled_test//屏幕好坏测试，不用的时候注释掉
//#define motot_3510_PID_change_test1		//速度环测试模式1，不用的时候注释掉
//#define motot_3510_PID_change_test2		//速度环测试模式2，不用的时候注释掉
//#define motor_GM3510_PID_change_test1	//角度环测试模式1，不用的时候注释掉
//#define imu_dataread_test;//
#define oled_key_test//oled上面按键测试
//#define oled_keyboard

void motor_speed_PID_test(void);
float data_get(float *a);
float motor_speed_get(int *a);
void motor_angle_PID_test(void);
void oled_test(void);
void oled_key(void);
void key_keyboard(void);

void oled_Osker_show(void);

void Oled_Show_Tem(void);
	
#endif

