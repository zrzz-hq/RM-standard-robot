#ifndef _oled_mode_h_
#define _oled_mode_h_
#include "main.h"

//#define oled_test//��Ļ�û����ԣ����õ�ʱ��ע�͵�
//#define motot_3510_PID_change_test1		//�ٶȻ�����ģʽ1�����õ�ʱ��ע�͵�
//#define motot_3510_PID_change_test2		//�ٶȻ�����ģʽ2�����õ�ʱ��ע�͵�
//#define motor_GM3510_PID_change_test1	//�ǶȻ�����ģʽ1�����õ�ʱ��ע�͵�
//#define imu_dataread_test;//
#define oled_key_test//oled���水������
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

