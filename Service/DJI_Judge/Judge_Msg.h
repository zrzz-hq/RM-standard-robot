#ifndef _JUDGE_MSG_H_
#define _JUDGE_MSG_H_

#include "Judge_Data.h"




typedef __packed struct
{
		DJI_Judge_Info_t DJI_Judge_Info;
		
}DJI_Judge_Msg_t;

DJI_Judge_Info_t* Get_Judge_Info(void);

#endif