#include "main.h"

#include "oled.h"
#include "oled_mode.h"

#include "usart1_dbus.h"
#include "adc.h"


int key_read;


void motor_angle_limit(float *a)
{
	if(*a>=8190)
		*a=5;
	if(*a<=0)
		*a=8186;
}
float data_get(float *a)
{
	int b;
	if(*a>50000)
	{
	b=*a-65536;
	return b;
	}
	else
	return *a;
}



/****************************************************
	获取电机反馈速度
****************************************************/
float motor_speed_get(int *a)
{
	int b;
	if(*a>50000)
	{
	b=*a-65536;
	return b;
	}
	else
	return *a;
}

/****************************************************
	oled显示kp，ki，kd数值
	**没法用数组储存再显示**test1**失败**
	**拆成整数和小数显示**test2**
	**一行能显示20个数字**
	********************
	kp 		 ki	    kd
	**.*** **.*** **.***
	kp(3,1)数字(4,1,6)--整数(4,1,0,2)小数(4,4,1,3)
	标点--oled_printf(4,3,".");
	ki(3,8)数字(4,8,6)--整数(4,8,0,2)小数(4,11,1,3)
	标点--oled_printf(4,10,".");
	kd(3,15数字)(4,15,6)--整数(4,15,0,2)小数(4,18,1,3)
	标点--oled_printf(4,17,".");
****************************************************/			

void motor_speed_PID_test(void)
{

/****************************************************
	oled显示kp，ki，kd数值
	**没法用数组储存再显示**demot1**失败**
	**拆成整数和小数显示**demo2**成功**
	**有俩种模式**test1**test2**
	**一行能显示20个数字**
	********************
	kp 		 ki	    kd
	**.*** **.*** **.***
	kp(3,1)数字(4,1,6)--整数(4,1,0,2)小数(4,4,1,3)
	标点--oled_printf(4,3,".");
	ki(3,8)数字(4,8,6)--整数(4,8,0,2)小数(4,11,1,3)
	标点--oled_printf(4,10,".");
	kd(3,15数字)(4,15,6)--整数(4,15,0,2)小数(4,18,1,3)
	标点--oled_printf(4,17,".");
****************************************************/		
	
#ifdef motot_3510_PID_change_test1
	
	oled_clear(Pen_Clear);
//		printf("motor3510_speed_now_kd=%f    \r\n",motor3510_speed_now_kd);
	PID_minlimit(&motor3510_speed_now_kp);
	PID_minlimit(&motor3510_speed_now_ki);
	PID_minlimit(&motor3510_speed_now_kd);
		
/****************************************************
	oled显示电机速度
****************************************************/	
	motor_speed_now = motor_speed_get(&dipan1.speed);
	oled_number(2,8,&motor_speed_now,0,4);
	
	if (RC_Ctl.rc.s1==0x01&&RC_Ctl.rc.s2==0x02)
	{

			oled_printf(1,1,"motor_speed_cotrol");
			oled_number(2,1,&motor_speed_test_error,0,4);
			
			motor_speed_test_error=motor_speed_control-motor_speed_now;
			oled_number(2,14,&motor_speed_test_error,1,4);

			
	}
	else
	{
			oled_printf(1,1,"PID_change_test1");	
			oled_number(2,1,&PID_test_error,0,4);
			
			PID_test_error=motor_speed_change-motor_speed_now;
			oled_number(2,14,&PID_test_error,1,4);
	}
	
	oled_printf(3,1,"kp");	
	oled_small_number(4,1,&motor3510_speed_now_kp,&motor3510_speed_now_kp_integer,&motor3510_speed_now_kp_decimal,2,3);			
	oled_printf(3,8,"ki");
	oled_small_number(4,8,&motor3510_speed_now_ki,&motor3510_speed_now_ki_integer,&motor3510_speed_now_ki_decimal,2,3);	
	oled_printf(3,15,"kd");	
	oled_small_number(4,15,&motor3510_speed_now_kd,&motor3510_speed_now_kd_integer,&motor3510_speed_now_kd_decimal,2,3);
	
#endif	

#ifdef motot_3510_PID_change_test2
	
	oled_clear(Pen_Clear);
//		printf("motor3510_speed_now_kd=%f    \r\n",motor3510_speed_now_kd);
	PID_minlimit(&motor3510_speed_now_kp);
	PID_minlimit(&motor3510_speed_now_ki);
	PID_minlimit(&motor3510_speed_now_kd);
		
/****************************************************
	oled显示电机速度
****************************************************/	
	motor_speed_now = motor_speed_get(&dipan1.speed);
	oled_number(2,8,&motor_speed_now,0,4);
	
		if (RC_Ctl.rc.s1==0x02)
		{
			oled_printf(1,1,"motor_speed_cotrol");				
			oled_number(2,1,&motor_speed_control,0,4);			
			motor_speed_test_error=motor_speed_control-motor_speed_now;		
//			printf("dipan1.speed=%d      \r\n",dipan1.speed);
			oled_number(2,14,&motor_speed_test_error,1,4);		
//			printf("motor_speed_test_error=%d      \r\n",motor_speed_test_error);
			
		}
		else
		{
			oled_printf(1,1,"PID_change_test2");	
			oled_number(2,1,&motor_speed_change,0,4);			
			
			PID_test_error=motor_speed_change-motor_speed_now;
			oled_number(2,14,&PID_test_error,1,4);
		}
			
	oled_printf(3,1,"kp");	
	oled_small_number(4,1,&motor3510_speed_now_kp,&motor3510_speed_now_kp_integer,&motor3510_speed_now_kp_decimal,2,3);			
	oled_printf(3,8,"ki");
	oled_small_number(4,8,&motor3510_speed_now_ki,&motor3510_speed_now_ki_integer,&motor3510_speed_now_ki_decimal,2,3);	
	oled_printf(3,15,"kd");	
	oled_small_number(4,15,&motor3510_speed_now_kd,&motor3510_speed_now_kd_integer,&motor3510_speed_now_kd_decimal,2,3);
		
#endif		
	
#ifdef oled_test
	oled_LOGO();
#endif		

	oled_refresh_gram();
	
}

void motor_angle_PID_test(void)
{
#ifdef oled_test
	oled_LOGO();
#endif

#ifdef motor_GM3510_PID_change_test1
		oled_clear(Pen_Clear);
//		printf("motor3510_speed_now_kd=%f    \r\n",motor3510_speed_now_kd);
	PID_minlimit(&motorGM3510_angle_now_kp);
	PID_minlimit(&motorGM3510_angle_now_ki);
	PID_minlimit(&motorGM3510_angle_now_kd);
//	motor_angle_limit(&motor_angle_control);
	motor_angle_limit(&motor_angle_change);
		
/****************************************************
	oled显示电机角度
****************************************************/	
	motor_angle_now = yuntai1.angle;
	oled_number(2,8,&motor_angle_now,0,4);
	
	
		if (RC_Ctl.rc.s1==0x02)
		{
			oled_printf(1,1,"motor_angle_control");				
			oled_number(2,1,&motor_angle_control,0,4);			
			motor_angle_test_error=motor_angle_control-motor_angle_now;		
//			printf("dipan1.speed=%d      \r\n",dipan1.speed);
			oled_number(2,14,&motor_angle_test_error,1,4);		
//			printf("motor_speed_test_error=%d      \r\n",motor_speed_test_error);
			
		}
		else
		{
			oled_printf(1,1,"PID_change_test2");	
			oled_number(2,1,&motor_angle_change,0,4);			
			
			PID_test_error=motor_angle_change-motor_angle_now;
			oled_number(2,14,&PID_test_error,1,4);
		}
			
	
	oled_printf(3,1,"kp");	
	oled_small_number(4,1,&motorGM3510_angle_now_kp,&motorGM3510_angle_now_kp_integer,&motorGM3510_angle_now_kp_decimal,2,3);			
	oled_printf(3,8,"ki");
	oled_small_number(4,8,&motorGM3510_angle_now_ki,&motorGM3510_angle_now_ki_integer,&motorGM3510_angle_now_ki_decimal,2,3);	
	oled_printf(3,15,"kd");	
	oled_small_number(4,15,&motorGM3510_angle_now_kd,&motorGM3510_angle_now_kd_integer,&motorGM3510_angle_now_kd_decimal,2,3);

	
#endif
oled_refresh_gram();
}

void oled_test(void)
{
	oled_clear(Pen_Clear);
	oled_LOGO();
	oled_refresh_gram();
}

void oled_key(void)
{
			oled_clear(Pen_Clear);
	key_read=Get_KEY_ADC();
	if(key_read<10&&key_read>=0)
		oled_printf(3,1,"left");
	else if(key_read>2900&&key_read<3200)
		oled_printf(3,1,"in");
	else if(key_read>900&&key_read<1200)
		oled_printf(3,1,"up");
	else if(key_read>1800&&key_read<2100)
		oled_printf(3,1,"right");
	else if(key_read>3400&&key_read<3700)
		oled_printf(3,1,"down");
	else
		oled_printf(3,1,"no move");
	oled_printf(1,1,"key_read");	
	oled_shownum(2,1,key_read,0,4);
	oled_refresh_gram();
}
extern Osker_Remote_Control_Struct Osker_Remote_Control_Data;

extern Osker_Remote_Control_Data_Key_t Osker_Remote_Control_Data_Key;
extern Osker_Remote_Control_Data_Key_t Osker_Remote_Control_Data_Key_Last;

extern float Osker_Motor_control_Data[4];

	int pwm_show;

void oled_Osker_show(void)
{

			oled_clear(Pen_Clear);
			
			oled_printf(2,9,"Key");
			pwm_show = Osker_Motor_control_Data[0];
	
			oled_printf(4,6,"PWM:");
			oled_shownum(4,10,pwm_show,0,5);
			
			if(Osker_Remote_Control_Data_Key == L_KEY)
			{
				oled_printf(3,9,"L_KEY");
			}
			else if(Osker_Remote_Control_Data_Key == R_KEY)
			{
				oled_printf(3,9,"R_KEY");
			}
			else if(Osker_Remote_Control_Data_Key == R2)
			{
				oled_printf(3,9,"R2");
			}
			else if(Osker_Remote_Control_Data_Key == R1)
			{
				oled_printf(3,9,"R1");
			}
			else if(Osker_Remote_Control_Data_Key == RU)
			{
				oled_printf(3,9,"RU");
			}
			else if(Osker_Remote_Control_Data_Key == RR)
			{
				oled_printf(3,9,"RR");
			}
			else if(Osker_Remote_Control_Data_Key == RD)
			{
				oled_printf(3,9,"RD");
			}
			else if(Osker_Remote_Control_Data_Key == RL)
			{
				oled_printf(3,9,"RL");
			}
			else if(Osker_Remote_Control_Data_Key == ST)
			{
				oled_printf(3,9,"ST");
			}
			else if(Osker_Remote_Control_Data_Key == SE)
			{
				oled_printf(3,9,"SE");
			}
			else if(Osker_Remote_Control_Data_Key == LR)
			{
				oled_printf(3,9,"LR");
			}
			else if(Osker_Remote_Control_Data_Key == LD)
			{
				oled_printf(3,9,"LD");
			}
			else if(Osker_Remote_Control_Data_Key == LL)
			{
				oled_printf(3,9,"LL");
			}
			else if(Osker_Remote_Control_Data_Key == LU)
			{
				oled_printf(3,9,"LU");
			}
			else if(Osker_Remote_Control_Data_Key == L2)
			{
				oled_printf(3,9,"L2");
			}
			else if(Osker_Remote_Control_Data_Key == L1)
			{
				oled_printf(3,9,"L1");
			}
	
//	if(key_read<10&&key_read>=0)
//		oled_printf(3,1,"left");
//	else if(key_read>2700&&key_read<3000)
//		oled_printf(3,1,"in");
//	else if(key_read>900&&key_read<1200)
//		oled_printf(3,1,"up");
//	else if(key_read>1800&&key_read<2100)
//		oled_printf(3,1,"right");
//	else if(key_read>3000&&key_read<3200)
//		oled_printf(3,1,"down");
//	else
//		oled_printf(3,1,"no move");

	if(Osker_Remote_Control_Data.rc.ch3 < 0)
	{
		oled_printf(1,17,"RU");
	}
	else if(Osker_Remote_Control_Data.rc.ch3 > 0)
	{
		oled_printf(3,17,"RD");
	}
	
	if(Osker_Remote_Control_Data.rc.ch2 < 0)
	{
		oled_printf(2,19,"RR");
	}
	else if(Osker_Remote_Control_Data.rc.ch2 > 0)
	{
		oled_printf(2,15,"RL");
	}
	
	if((Osker_Remote_Control_Data.rc.ch3 == 0) && ((Osker_Remote_Control_Data.rc.ch2 == 0)))
	{
		oled_printf(2,17,"NO");
	}

	
	if((Osker_Remote_Control_Data.rc.ch0 == 0) && ((Osker_Remote_Control_Data.rc.ch1 == 0)))
	{
			oled_printf(2,3,"NO");
	}
	
	if(Osker_Remote_Control_Data.rc.ch1 < 0)
	{
		oled_printf(1,3,"LU");	
	}
	else if(Osker_Remote_Control_Data.rc.ch1 > 0)
	{
		oled_printf(3,3,"LD");	
	}
	
	if(Osker_Remote_Control_Data.rc.ch0 < 0)
	{
		oled_printf(2,5,"LR");	
	}
	else if(Osker_Remote_Control_Data.rc.ch0 > 0)
	{
		oled_printf(2,1,"LL");
	}
	

//	oled_printf(1,3,"LU");	
//	oled_printf(2,5,"LR");	
//	oled_printf(2,1,"LL");	
//	oled_printf(3,3,"LD");
//	
//	oled_printf(1,17,"RU");	
//	oled_printf(2,19,"RR");	
//	oled_printf(2,15,"RL");	
//	oled_printf(3,17,"RD");	
//	oled_shownum(0,1,key_read,0,4);
	oled_refresh_gram();
}

float Motor_Power;
float Board_Power;
float Cpu_Power;

void Oled_Show_Tem(void)
{
	oled_clear(Pen_Clear);
	
	Motor_Power = Motor_Power_Read(10);
	Board_Power = Board_Power_Read(10);
	Cpu_Power = Cpu_Temperature_Read(10);
//	oled_LOGO();
	oled_printf(1,2,"Adc_Show");
//	oled_printf(2,2,"Motor_Power:");
//	oled_printf(3,2,"Board_Power:");
//	oled_printf(4,2,"Cpu_Power:");
	oled_printf(2,2,"Motor_Power:%5.2f",Motor_Power);
	oled_printf(3,2,"Board_Power:%5.2f",Board_Power);
	oled_printf(4,2,"Cpu_Power:%5.2f",Cpu_Power);
	
	oled_refresh_gram();
}
