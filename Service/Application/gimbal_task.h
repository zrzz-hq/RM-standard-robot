#ifndef __GIMBAL_TASK_H__
#define __GIMBAL_TASK_H__

#include "main.h"
#include "DJI_Remote_Control.h"
#include "pid.h" 

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#define GIMBAL_RC_DEADLINE  RC_DEADLINE

#define GIMBAL_VISION_KEY KEY_PRESSED_OFFSET_CTRL

#define Gimbal_Motionless_Time 1500

#define Gimbal_Turn_Head_Key KEY_PRESSED_OFFSET_F
#define Gimbal_Turn_Head_Speed 0.005f

#define GIMBAL_FIRST_MODE GIMBAL_ZERO
#define GIMBAL_SECOND_MODE GIMBAL_MOTOR_CONTROL
#define GIMBAL_THIRD_MODE GIMBAL_SPIN

#define GIMBAL_YAW_VISION_ANGLE_KP 1000
#define GIMBAL_YAW_VISION_ANGLE_KI 0
#define GIMBAL_YAW_VISION_ANGLE_KD 0
#define GIMBAL_YAW_VISION_ANGLE_MAXOUT 10
#define GIMBAL_YAW_VISION_ANGLE_IMAXOUT 10

#define GIMBAL_YAW_MOTOR_ANGLE_KP 10
#define GIMBAL_YAW_MOTOR_ANGLE_KI 0
#define GIMBAL_YAW_MOTOR_ANGLE_KD 60
#define GIMBAL_YAW_MOTOR_ANGLE_MAXOUT 10
#define GIMBAL_YAW_MOTOR_ANGLE_IMAXOUT 10

#define GIMBAL_YAW_IMU_ANGLE_KP 20
#define GIMBAL_YAW_IMU_ANGLE_KI 0
#define GIMBAL_YAW_IMU_ANGLE_KD 400
#define GIMBAL_YAW_IMU_ANGLE_MAXOUT 9
#define GIMBAL_YAW_IMU_ANGLE_IMAXOUT 0

#define GIMBAL_YAW_SPEED_KP 5000
#define GIMBAL_YAW_SPEED_KI 40
#define GIMBAL_YAW_SPEED_KD 0
#define GIMBAL_YAW_SPEED_MAXOUT 25000
#define GIMBAL_YAW_SPEED_IMAXOUT 5000

#define GIMBAL_PITCH_VISION_ANGLE_KP 800
#define GIMBAL_PITCH_VISION_ANGLE_KI 0
#define GIMBAL_PITCH_VISION_ANGLE_KD 0
#define GIMBAL_PITCH_VISION_ANGLE_MAXOUT 10
#define GIMBAL_PITCH_VISION_ANGLE_IMAXOUT 10

#define GIMBAL_PITCH_MOTOR_ANGLE_KP 18
#define GIMBAL_PITCH_MOTOR_ANGLE_KI 0
#define GIMBAL_PITCH_MOTOR_ANGLE_KD 20
#define GIMBAL_PITCH_MOTOR_ANGLE_MAXOUT 10
#define GIMBAL_PITCH_MOTOR_ANGLE_IMAXOUT 10

#define GIMBAL_PITCH_IMU_ANGLE_KP 10
#define GIMBAL_PITCH_IMU_ANGLE_KI 0
#define GIMBAL_PITCH_IMU_ANGLE_KD 0
#define GIMBAL_PITCH_IMU_ANGLE_MAXOUT 10
#define GIMBAL_PITCH_IMU_ANGLE_IMAXOUT 0

#define GIMBAL_PITCH_SPEED_KP 5000
#define GIMBAL_PITCH_SPEED_KI 1
#define GIMBAL_PITCH_SPEED_KD 0
#define GIMBAL_PITCH_SPEED_MAXOUT 25000
#define GIMBAL_PITCH_SPEED_IMAXOUT 5000

#define GIMBAL_YAW_IMU_ANGLE_ADDRESS_OFFSET  0
#define GIMBAL_PITCH_IMU_ANGLE_ADDRESS_OFFSET  2
#define GIMBAL_YAW_IMU_ASPEED_ADDRESS_OFFSET  0
#define GIMBAL_PITCH_IMU_ASPEED_ADDRESS_OFFSET  2

#define GIMBAL_YAW_MAX_ANGLE 1.59
#define GIMBAL_YAW_MIN_ANGLE -1.59
#define GIMBAL_PITCH_MAX_ANGLE 0.46
#define GIMBAL_PITCH_MIN_ANGLE -0.34

//�е�����
#define GIMBAL_YAW_MID_ANGLE  2.10          //  -0.0069029131    // 3.09
#define GIMBAL_PITCH_MID_ANGLE  0.53         //    0.0383495167  //  0.53
//�����Ƶı���
#define GIMBAL_YAW_RC_SEN 278000.0f
#define GIMBAL_YAW_MOUSE_SEN 20000.0f
#define GIMBAL_PITCH_RC_SEN 756000.0f
#define GIMBAL_PITCH_MOUSE_SEN 20000.0f

#define GIMBAL_MOTOR_ENCODER_MID_VAL 4096

#define YAW_MOTOR_ID 0x205
#define PITCH_MOTOR_ID 0x206
#define GIMBAL_MOTOR_ALL_ID 0x1FF
#define GIMBAL_CAN CAN2_RX 

#define GIMBAL_INIT_ANGLE_ERROR  0.1f
#define GIMBAL_INIT_PITCH_SPEED  0.004f
#define GIMBAL_INIT_YAW_SPEED    0.001f
#define GIMBAL_MAX_INIT_TIME 		 5000

#define GIMBAL_MAX_NO_OPT_TIME 1000

typedef enum
{
	GIMBAL_ZERO = 0,//�޿��ƣ�����0
	GIMBAL_INIT,//��ʼ��ģʽ������ת�����趨�Ƕȣ���ֹת�����͵��½ṹ��
	GIMBAL_MOTIONLESS,//�޿��ƣ��õ���Ƕȱ��ֵ�ǰλ��
	GIMBAL_MOTOR_CONTROL,//����Ƕȿ���
	GIMBAL_IMU_CONTROL,//�����ǽǶȿ���
	GIMBAL_VISION_CONTROL,//�Ӿ����ƽӿ�
	GIMBAL_FOLLOW_CHASSIS,//��̨������̣�yaw��̶�����
	GIMBAL_SPIN//С����ģʽ
}Gimbal_Mode_t;

typedef enum
{
		GIMBAL_MOTOR_NO_FORCE,//�������ģʽ������Ϊ0
		GIMBAL_MOTOR_ENCODER_ANGLE,//����������Ƕȿ���ģʽ
		GIMBAL_MOTOR_IMU_ANGLE,//���IMU�Ƕȿ���ģʽ
		GIMBAL_MOTOR_SPEED,//����ٶȿ���ģʽ
}Gimbal_Motor_Mode_t;

typedef struct
{
	Motor_Msg_t* Shoot_Motor_Msg_Get;
	int Motoe_Now_Angle;
	int Motoe_Now_Speed;
	int Motor_Now_Current;
	int Motor_Last_Speed;	
	int Motor_Last_Angle;
}Gimbal_Motor_Msg_t;


typedef struct
{
	//��̨���ģʽ
	Gimbal_Motor_Mode_t Gimbal_Motor_Mode;
	
	//��̨����������
	Motor_Msg_t* Gimbal_Motor_Msg_Get;	
	//�����ǽǶȻ�ȡ��ַ
	const float* Gimbal_IMU_Angle_Msg_Get;
	//�����ǽ��ٶȻ�ȡ��ַ
	const float* Gimbal_IMU_Aspeed_Msg_Get;
	
	//��̨��е������ֵ
	int16_t Gimbal_Motor_Encoder_Get;
	//��̨��е�Ƕ�ֵ
	float Gimbal_Motor_Angle_Get;
	//��̨�����ǽǶ�ֵ
	float Gimbal_IMU_Angle_Get;
	//��̨����Ƕ��趨ֵ
	float Gimbal_Motor_Angle_Set;
	
	//��̨��е���ٶ�ֵ
	float Gimbal_Motor_Aspeed_Get;	
	//��̨����Ƕ��趨ֵ
	float Gimbal_Motor_ASpeed_Set;
	
	//��̨����Ƕ��е���Ϣ
	float Gimbal_Motor_Mid_Angle;	
	//��̨������Ƕ���Ϣ
	float Gimbal_Motor_Max_Angle;
	//��̨�����С�Ƕ���Ϣ
	float Gimbal_Motor_Min_Angle;
	
	//��̨����ǶȻ�PID
	PID* Gimbal_Motor_Angle_Pid;
	PID* Gimbal_Motor_Speed_Pid;
	//��̨����ٶȻ�PID
	
	//��̨��������趨ֵ
	float Gimbal_Motor_Current_Set;
}Gimbal_Motor_t;

typedef struct
{
	//��̨ģʽ
	Gimbal_Mode_t Gimbal_Mode;
	Gimbal_Mode_t Last_Gimbal_Mode;
	
/**********��̨���PID�ṹ��**********/
	//��̨���ԽǶȼ���PID-��е�Ƕ�
	PID Gimbal_Yaw_Motor_Angle_Pid;
	PID Gimbal_Pitch_Motor_Angle_Pid;
	//��̨���ԽǶȼ���PID-�����ǽǶ�
	PID Gimbal_Yaw_IMU_Angle_Pid;
	PID Gimbal_Pitch_IMU_Angle_Pid;
	//��̨�Ӿ�����PID-�����ǽǶ�
	PID Gimbal_Yaw_Vision_Angle_Pid;
	PID Gimbal_Pitch_Vision_Angle_Pid;
	
	//��̨���ٶȼ���PID
	PID Gimbal_Yaw_Speed_Pid;	
	PID Gimbal_Pitch_Speed_Pid;

	//��̨����PID
	//PID Gimbal_Yaw_Motor_Pid;	
	//PID Gimbal_Pitch_Motor_Pid;	
	
/**********��̨�����Ϣ�ṹ��**********/
	Gimbal_Motor_t Gimbal_Yaw_Motor;
	Gimbal_Motor_t Gimbal_Pitch_Motor;
	
	//������������ݻ�ȡ
	//const float* Gimbal_IMU_Angle;
	//const float* Gimbal_IMU_Aspeed;
	//ң��������ֵ��ȡ
	const RC_Ctl_t* Gimbal_Rc_Ctrl_Data;	
	
	float Gimbal_Init_Time;
	float Gimbal_Yaw_Init_Finish_Flag;
	float Gimbal_Pitch_Init_Finish_Flag;
	
	float Gimbal_Yaw_Init_Start_Angle;
	float Gimbal_Pitch_Init_Start_Angle;
	
	float Gimbal_Yaw_Rc_Sen;
	float Gimbal_Pitch_Rc_Sen;
	float Gimbal_Yaw_Mouse_Sen;
	float Gimbal_Pitch_Mouse_Sen;
	
	uint16_t Gimbal_Yaw_No_Opt_Time;
	uint16_t Gimbal_Pitch_No_Opt_Time;
	//��̨���͵���ֵ
	//float Gimbal_Motor_Current_Send[2];

}Gimbal_t;


void Gimbal_Task(void *pvParameters);
float* Get_Gimbal_Yaw_Angle(void);

Gimbal_Mode_t* Return_Gimbal_Mode_Add(void);

#endif 
