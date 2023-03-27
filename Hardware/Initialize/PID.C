/**
  ******************************************************************************
  * @file    pid.c
  * @author  JackyJuu,  site:www.robomentor.cn
	* @Note 	 pid相关控制函数
  * @version V1.9.7
  ******************************************************************************
	* @describe PID相关函数
*/
#include "pid.h"

//PID限制最大值函数
void PID_limit(float *a, float PID_MAX)
{
    if(*a > PID_MAX)
        *a = PID_MAX;
    if(*a < -PID_MAX)
        *a = -PID_MAX;
}

//PID限制最小值函数
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

//PID初始化
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
	pid->pid_mode =mode;//1为位置环PID，2为增量式PID
	pid->maxout =maxout;//PID限幅
	pid->imaxout=imaxout;	
	pid->delta =0;
	pid->delta_out =0;
	pid->last_delta_out =0;
}

//PID初始化
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
	pid->pid_mode =PID_ADVANCED;//1为位置环PID，2为增量式PID
	pid->maxout =maxout;//PID限幅
	pid->imaxout=imaxout;	
	pid->delta =0;
	pid->delta_out =0;
	pid->last_delta_out =0;
	
	pid->Small_Error_Limit = Limit_Data;
	
	
	pid->Set_A = pid_Aa;
	pid->Set_B = pid_Bb;
	pid->Set_alpha = pid_Alpha;
	
}

//pid参数更新
void pid_update(PID*pid,float p,float i,float d,int maxout,int imaxout,pid_type_t mode)
{
	pid->kp=p;
	pid->ki=i;
	pid->kd =d;
	pid->pid_mode =mode;//1为位置环PID，2为增量式PID
	pid->maxout =maxout;//PID限幅
	pid->imaxout=imaxout;	
}

//PID函数
static float pid_calc(PID*pid, float error)
{
    

		pid->now_error = error;	//set - measure

    if(pid->pid_mode == PID_POSITON) //位置环PID
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
		
    else if(pid->pid_mode == PID_DELTA)//增量式PID
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
		else if(pid->pid_mode == PID_ADVANCED)//变速积分/不完全微分
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
			//变速积分
      pid->iout = pid->ki * pid->sum_of_error;			
			//不完全微分	
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
//误差是线性区间的PID解算
float pid_calc_linear(PID* pid,float now,float set)
{
		pid->now = now;
    pid->set = set;
		float error = set - now;
		return pid_calc(pid,error);
}
//误差是环形区间的PID解算
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

	
