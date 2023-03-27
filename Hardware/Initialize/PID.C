/**
  ******************************************************************************
  * @file    pid.c
  * @author  JackyJuu,  site:www.robomentor.cn
	* @Note 	 pid��ؿ��ƺ���
  * @version V1.9.7
  ******************************************************************************
	* @describe PID��غ���
*/
#include "pid.h"

//PID�������ֵ����
void PID_limit(float *a, float PID_MAX)
{
    if(*a > PID_MAX)
        *a = PID_MAX;
    if(*a < -PID_MAX)
        *a = -PID_MAX;
}

//PID������Сֵ����
void PID_limitmin(float *a, float PID_MIN)
{
		if(*a < PID_MIN && *a > 0)
        *a = PID_MIN;
    if(*a > -PID_MIN && *a < 0)
        *a = -PID_MIN;
}

float PID_Fabs(float ffabs)
{
		if(ffabs >= 0)
        return ffabs;
    if(ffabs <= 0)
        return -ffabs;
		return ffabs;
}

//PID��ʼ��
void pid_init(PID*pid,float p,float i,float d,int maxout,int imaxout,pid_type_t mode)
{
	pid->kp=p;
	pid->ki=i;
	pid->kd =d;
	pid->pout =0;
	pid->iout =0;
	pid->dout =0;
	pid->last_error =0;
	pid->last_last_error =0;
	pid->now_error =0;
	pid->sum_of_error =0;
	pid->pid_mode =mode;//1Ϊλ�û�PID��2Ϊ����ʽPID
	pid->maxout =maxout;//PID�޷�
	pid->imaxout=imaxout;	
	pid->delta =0;
	pid->delta_out =0;
	pid->last_delta_out =0;
}

//PID��ʼ��
void pid_math_init(PID*pid,float p,float i,float d,int maxout,int imaxout,float pid_Aa,float pid_Bb,float pid_Alpha,float Limit_Data)
{
	pid->kp=p;
	pid->ki=i;
	pid->kd =d;
	pid->pout =0;
	pid->iout =0;
	pid->dout =0;
	pid->last_error =0;
	pid->last_last_error =0;
	pid->now_error =0;
	pid->sum_of_error =0;
	pid->pid_mode =PID_ADVANCED;//1Ϊλ�û�PID��2Ϊ����ʽPID
	pid->maxout =maxout;//PID�޷�
	pid->imaxout=imaxout;	
	pid->delta =0;
	pid->delta_out =0;
	pid->last_delta_out =0;
	
	pid->Small_Error_Limit = Limit_Data;
	
	
	pid->Set_A = pid_Aa;
	pid->Set_B = pid_Bb;
	pid->Set_alpha = pid_Alpha;
	
}

//pid��������
void pid_update(PID*pid,float p,float i,float d,int maxout,int imaxout,pid_type_t mode)
{
	pid->kp=p;
	pid->ki=i;
	pid->kd =d;
	pid->pid_mode =mode;//1Ϊλ�û�PID��2Ϊ����ʽPID
	pid->maxout =maxout;//PID�޷�
	pid->imaxout=imaxout;	
}

//PID����
static float pid_calc(PID*pid, float error)
{
    

		pid->now_error = error;	//set - measure

    if(pid->pid_mode == PID_POSITON) //λ�û�PID
    {
	      pid->pout = pid->kp * pid->now_error;
        pid->iout = pid->ki * pid->sum_of_error;
        pid->dout = pid->kd * (pid->now_error - pid->last_error );
				pid->sum_of_error+=pid->now_error;	
				PID_limit(&(pid->sum_of_error), 10000);
				PID_limit(&(pid->iout), pid->imaxout);
        pid->out = pid->pout + pid->iout + pid->dout;
        PID_limit(&(pid->out), pid->maxout);
    }	
		
    else if(pid->pid_mode == PID_DELTA)//����ʽPID
    {
        pid->pout = pid->kp * (pid->now_error - pid->last_error);
        pid->iout = pid->ki * pid->now_error;
        pid->dout = pid->kd * (pid->now_error - 2*pid->last_error + pid->last_last_error);        
				PID_limit(&(pid->iout), pid->imaxout);
        pid->delta = pid->pout + pid->iout + pid->dout;
        pid->delta_out = pid->last_delta_out + pid->delta;
			  pid->out = pid->delta_out; 
				PID_limit(&(pid->out), pid->maxout);
        pid->last_delta_out = pid->delta_out;	//update last time
    }
		else if(pid->pid_mode == PID_ADVANCED)//���ٻ���/����ȫ΢��
    {
	      pid->pout = pid->kp * pid->now_error;
				if(((pid->Set_Out_Mode == 1) && (pid->now_error > 0)) || ((pid->Set_Out_Mode == -1) && (pid->now_error < 0)));
				else
				{
					if(PID_Fabs(pid->now_error) <= pid->Set_B)
					{
						pid->sum_of_error += pid->now_error;
					}
					else if(PID_Fabs(pid->now_error) >= (pid->Set_B + pid->Set_A))
					{
						pid->sum_of_error = 0;
					}
					else
					{
						pid->Set_ratio = (pid->Set_A + pid->Set_B - PID_Fabs(pid->now_error)) / pid->Set_A;
						pid->sum_of_error += pid->Set_ratio * pid->now_error;
					}
				}
			//���ٻ���
      pid->iout = pid->ki * pid->sum_of_error;			
			//����ȫ΢��	
			pid->dout = pid->kd * (pid->now_error - pid->last_error) * (1 - pid->Set_alpha) + pid->Set_alpha * pid->Last_Ud;

        pid->out = pid->pout + pid->iout + pid->dout;
				if(pid->out > pid->maxout)
				{
					pid->out = pid->maxout;
					pid->Set_Out_Mode = 1;
				}
				else
				{
					pid->Set_Out_Mode = 0;
				}
				
				
				if(pid->out < -pid->maxout)
				{
					pid->out = -pid->maxout;
					pid->Set_Out_Mode = -1;
				}
				else
				{
					pid->Set_Out_Mode = 0;
				}
			pid->Last_Ud = pid->dout;

    }
		
    pid->last_last_error= pid->last_error;
    pid->last_error = pid->now_error;

    return pid->out;
}
//��������������PID����
float pid_calc_linear(PID* pid,float now,float set)
{
		pid->now = now;
    pid->set = set;
		float error = set - now;
		return pid_calc(pid,error);
}
//����ǻ��������PID����
float pid_calc_circular(PID*pid,float now,float set,float max,float min)
{
		if(min > max)
		{
				float temp = max;
				max = min;
				min = temp;
		}
		
		pid->now = now;
    pid->set = set;
		float error = set - now;
		float range = max - min;
		if(error > range/2)
				error -= range;
		else if(error < -range/2)
				error += range;
		else if((error == range/2&&pid->last_error < 0)||(error == -range/2&&pid->last_error > 0))
				error = -error;
		return pid_calc(pid,error);
}

	