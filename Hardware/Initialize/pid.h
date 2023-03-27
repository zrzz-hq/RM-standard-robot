#ifndef _pid_h_
#define _pid_h_

typedef enum
{
		PID_POSITON,
		PID_DELTA,
		PID_ADVANCED
}pid_type_t;

//PID�ṹ��
typedef struct PID
{
	float kp;//kp
	float ki;//ki
	float kd;//kd
	double pout;//k���
	float iout;//i���
	float dout;//d���
	float now_error;			//��ǰ���
	float last_error;			//��һ�����
	float last_last_error;	//���ϴ����
	float sum_of_error;     //��ʷ�����
	float set;		//����
	float now;		//��ǰ
	float out;		//���

	pid_type_t pid_mode;	//PIDģʽ���ã�1Ϊλ�û�PID��2Ϊ����ʽPID,3Ϊ�Ƕ�ר��PID
 
  float maxout;//PID����޷�	
	float imaxout;//I����޷�	
	float delta;		//��������ֵ
  float delta_out;	//����ʽ���ֵplus_out = last_plus_out + plus
  float last_delta_out;//�ϴ�����ʽ���ֵ
	
	float Max_Error_Data;
	float Small_Error_Limit;
	int Set_Out_Mode;
	float Set_A;
	float Set_B;
	float Set_ratio;
	float Set_alpha;
	float Last_Ud;
}PID;		

//PID�������ֵ����
void PID_limit(float *a, float PID_MAX);

//PID������Сֵ����
void PID_limitmin(float *a, float PID_MIN);

float PID_Fabs(float ffabs);

//PID��ʼ��
void pid_init(PID*pid,float p,float i,float d,int maxout,int imaxout,pid_type_t mode);

//PIDȫ����ʼ��
void pid_math_init(PID*pid,float p,float i,float d,int maxout,int imaxout,float pid_Aa,float pid_Bb,float pid_Alpha,float Limit_Data);

//pid��������
void pid_update(PID*pid,float p,float i,float d,int maxout,int imaxout,pid_type_t mode);

//PID����
float pid_calc_linear(PID*pid, float now, float set);
float pid_calc_circular(PID*pid,float now,float set,float max,float min);


#endif
