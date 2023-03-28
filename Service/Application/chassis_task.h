 #ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_

#include "main.h"
#include "DJI_Remote_Control.h"
#include "pid.h"
#include "Judge_Data.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#define CHASSIS_MOTIONLESS_TIME 1500

#define CHASSIS_RC_DEADLINE  RC_DEADLINE

#define CHASSIS_MOTOR_SPEED_KP 12000
#define CHASSIS_MOTOR_SPEED_KI 10
#define CHASSIS_MOTOR_SPEED_KD 0
#define CHASSIS_MOTOR_SPEED_MAXOUT 16000
#define CHASSIS_MOTOR_SPEED_IMAXOUT 2000

#define CHASSIS_ANGLE_KP 10
#define CHASSIS_ANGLE_KI 0
#define CHASSIS_ANGLE_KD 1
#define CHASSIS_ANGLE_MAXOUT 5
#define CHASSIS_ANGLE_IMAXOUT 0


#define CHASSIS_3508_RPM_TO_WHEEL_SPEED 0.0012708333f

#define Chassis_Max_Heat_Normal 60
#define Chassis_Max_Heat_Feipo 250

#define CHASSIS_SPEED_SETUP_TIME 1000

#define CHASSIS_TURN_SPEED_UP_KEY KEY_PRESSED_OFFSET_SHIFT
#define CHASSIS_FORWARD_KEY KEY_PRESSED_OFFSET_W
#define CHASSIS_BACKWARD_KEY KEY_PRESSED_OFFSET_S
#define CHASSIS_MOVE_LEFT_KEY KEY_PRESSED_OFFSET_A
#define CHASSIS_MOVE_RIGHT_KEY KEY_PRESSED_OFFSET_D
#define CHASSIS_TURN_LEFT_KEY KEY_PRESSED_OFFSET_E
#define CHASSIS_TURN_RIGHT_KEY KEY_PRESSED_OFFSET_Q
#define CHASSIS_TRANSLATE_KEY 	KEY_PRESSED_OFFSET_X

#define CHASSIS_X_MAX_SPEED 5.0f
#define CHASSIS_Y_MAX_SPEED 5.0f
#define CHASSIS_WZ_MAX_SPEED 5.0f
#define CHASSIS_WZ_SPIN_MAX_SPEED 10.0f

#define Chassis_Follow_Gimbal_Set 6

//���̶����ƶ�ʱ���ٶ�����������ٶ�
#define Chassis_Translate_Delta_Speed 0.005
#define Chassis_Translate_Max_Speed  1
//���̶����ƶ�ʱ��������ٶȵ�ʱ�䣨ms��
#define Chassis_Translate_Max_Speed_Time 2000

#define CHASSIS_FIRST_MODE CHASSIS_ZERO
#define CHASSIS_SECOND_MODE CHASSIS_RC_CONTROL
#define CHASSIS_THIRD_MODE CHASSIS_FOLLOW_GIMBAL

#define RI_MOTOR_ID 0x201
#define LI_MOTOR_ID 0x202
#define LB_MOTOR_ID 0x203
#define RB_MOTOR_ID 0x204
#define CHASSIS_MOTOR_ALL_ID 0x200
#define CHASSIS_CAN CAN2_RX

typedef enum
{
	CHASSIS_ZERO = 0,//�޿��ƣ�����0
	CHASSIS_MOTIONLESS,//�޿��ƣ��õ���Ƕȱ��ֵ�ǰλ��
	CHASSIS_RC_CONTROL,//ң��������
	CHASSIS_FOLLOW_GIMBAL,//���̸�����̨
	CHASSIS_SPIN_LEFT,//С����ģʽ
	CHASSIS_SPIN_RIGHT,//С����ģʽ
	CHASSIS_TRANSLATE
}Chassis_Mode_t;


typedef struct
{
	//����ģʽ
	Chassis_Mode_t Chassis_Mode;
	
	uint32_t Translate_Time_Count;
	float Chassis_Translate_Speed;
	
	const float* Chassis_IMU_Angle;//������������ݻ�ȡ
	const float* Chassis_IMU_Aspeed;
	const RC_Ctl_t* Chassis_RC_Ctl_Data;//ң��������ֵ��ȡ	
	const Motor_Msg_t* Chassis_Motor_Msg_Get[4];
	const DJI_Judge_Msg_t* Chassis_Judge_Msg_Get;
	const Super_C_Msg_t* Chassis_Cap_Msg_Get;
	
	float Chassis_X_Max_Speed;
	float Chassis_Y_Max_Speed;
	float Chassis_WZ_Max_Speed;
	float Chassis_WZ_Spin_Max_Speed;
	
	float Chassis_X_Speed_Set;	//�����ٶ�����	
	float Chassis_Y_Speed_Set;
	float Chassis_WZ_Speed_Set;
	float* Chassis_Follow_Gimbal_Angle_TM;//���̸�����̨-��̨��е�ǶȲ�
	
	uint16_t Chassis_Forward_Key_Time;
	uint16_t Chassis_Backward_Key_Time;
	uint16_t Chassis_Move_Left_Key_Time;
	uint16_t Chassis_Move_Right_Key_Time;
	uint16_t Chassis_Turn_Left_Key_Time;
	uint16_t Chassis_Turn_Right_Key_Time;
	
	PID Chassis_Motor_Pid[4];//���̵�����pid����
	PID Chassis_Angle_Pid;
	
	float Chassis_Motor_Speed_Set[4];//���̵���ٶ�����
	float Chassis_Motor_Speed_Get[4];//���̵����ǰ�ٶ�
	float Chassis_Motor_Curent_Send[4];//���̷��͵���ֵ
	
	uint8_t Chassis_X_Reverse_Flag;
	uint8_t Chassis_Y_Reverse_Flag;
	uint8_t Chassis_WZ_Reverse_Flag;
	
	uint16_t Chassis_Last_Mode_Key;
	uint8_t Chassis_Last_Mode_Switch;
	
	float Chassis_Power;	//���̹�������
	uint16_t Chassis_Power_Buffer;		//������������
	uint16_t Chassis_Max_Power;	//��ǰ���������

}Chassis_t;

void Chassis_Task(void *pvParameters);

uint8_t Is_Chassis_Spin();

#endif

