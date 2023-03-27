#include "user_tool.h"

void Num_Limit(float* Num,int Big_Num,int Sml_Num)
{
	if(*Num < Sml_Num)
	{
		*Num = Sml_Num;
	}
	else if(*Num > Big_Num)
	{
		*Num = Big_Num;
	}
		
}
