#include "Judge_Graphic.h"

DJI_Judge_Graphic_t DJI_Judge_Graphic;

#define JUDGE_GRAPHIC_STK_SIZE 256
#define JUDGE_GRAPHIC_TASK_PRIO 21
TaskHandle_t JudgeGraphicTask_Handler;

#define JUDGE_GRAPHIC_DATA_LEN 15
#define JUDGE_GRAPHIC_CHARACTER_LEN 30

#define Judge_Assert(Ptr) if(!(Ptr)) Judge_Assert_Fault_Handler()
#define Judge_Graphic_Obj_Lock(Obj) xSemaphoreTake((Obj)->Graphic_Obj_Mutex,portMAX_DELAY)
#define Judge_Graphic_Obj_Poll(Obj) xSemaphoreTake((Obj)->Graphic_Obj_Mutex,0)
#define Judge_Graphic_Obj_Unlock(Obj)  			xSemaphoreGive((Obj)->Graphic_Obj_Mutex)

//图形操作优先级查找表,由高到低依次是：删除操作、添加操作、修改操作、空操作
uint8_t Judge_Graphic_Opt_Prio[4] = {0,2,1,3};
//要求操作的优先级大于当前操作的优先级，那么覆盖当前操作
#define Judge_Graphic_Obj_Apply_Opt(Obj,Opt) \
	if(Judge_Graphic_Opt_Prio[(Opt)]>Judge_Graphic_Opt_Prio[(Obj)->Graphic_Data.operate_tpye])		\
			(Obj)->Graphic_Data.operate_tpye = (Opt);	

/********基础操作函数**********/

static Judge_Graphic_Obj_t* Judge_Graphic_Obj_Init()
{
		Judge_Assert(DJI_Judge_Graphic.Judge_Obj_Counter < 0xFFF);
	
		Judge_Graphic_Obj_t* Obj = pvPortMalloc(sizeof(Judge_Graphic_Obj_t));
		Judge_Assert(Obj);
		memset(Obj,0,sizeof(Judge_Graphic_Obj_t));
		vListInitialiseItem((ListItem_t*)Obj);
	
		Obj->Graphic_Data.graphic_name = DJI_Judge_Graphic.Judge_Obj_Counter++;
		//创建信号量
		Obj->Graphic_Obj_Mutex = xSemaphoreCreateBinary();
		Judge_Assert(Obj->Graphic_Obj_Mutex);
	
		List_Insert_End(&DJI_Judge_Graphic.Graphic_Obj_List,(ListItem_t*)Obj,portMAX_DELAY);
		return Obj;
}

void Judge_Graphic_Obj_Set_Color(Judge_Graphic_Obj_t* Obj,Judge_Graphic_Color_t Color)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		Obj->Graphic_Data.color = Color;

		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

uint32_t Judge_Graphic_Obj_Get_Width(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		uint32_t W = 0;
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				W = Obj->Graphic_Data.graphic_union.graphic_data.end_x;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				W = Obj->Graphic_Data.graphic_union.graphic_data.radius;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				W = Obj->Graphic_Data.graphic_union.graphic_data.end_x - Obj->Graphic_Data.start_x;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return W;
}

uint32_t Judge_Graphic_Obj_Get_Height(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		uint32_t H = 0;
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				H = Obj->Graphic_Data.graphic_union.graphic_data.end_y;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				H = Obj->Graphic_Data.graphic_union.graphic_data.radius;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				H = Obj->Graphic_Data.graphic_union.graphic_data.end_y - Obj->Graphic_Data.start_y;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return H;
}

void Judge_Graphic_Obj_Set_Width(Judge_Graphic_Obj_t* Obj,uint32_t W)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				uint32_t Ra = Obj->Graphic_Data.graphic_union.graphic_data.end_x;
				uint32_t X = Obj->Graphic_Data.start_x - Ra/2;
				Obj->Graphic_Data.start_x = X + W/2;
				Obj->Graphic_Data.graphic_union.graphic_data.end_x = W;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				uint32_t R = Obj->Graphic_Data.graphic_union.graphic_data.radius;
				uint32_t X = Obj->Graphic_Data.start_x - R/2;
				Obj->Graphic_Data.start_x = X + W/2;
				Obj->Graphic_Data.graphic_union.graphic_data.radius = W;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				Obj->Graphic_Data.graphic_union.graphic_data.end_x = Obj->Graphic_Data.start_x + W;
		}
		
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

void Judge_Graphic_Obj_Set_Height(Judge_Graphic_Obj_t* Obj,uint32_t H)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				uint32_t Rb = Obj->Graphic_Data.graphic_union.graphic_data.end_y;
				uint32_t Y = Obj->Graphic_Data.start_y - Rb/2;
				Obj->Graphic_Data.start_y = Y + H/2;
				Obj->Graphic_Data.graphic_union.graphic_data.end_y = H;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				uint32_t R = Obj->Graphic_Data.graphic_union.graphic_data.radius;
				uint32_t Y = Obj->Graphic_Data.start_y - R/2;
				Obj->Graphic_Data.start_x = Y + H/2;
				Obj->Graphic_Data.graphic_union.graphic_data.radius = H;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				Obj->Graphic_Data.graphic_union.graphic_data.end_y = Obj->Graphic_Data.start_y + H;
		}
		
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

void Judge_Graphic_Obj_Set_Size(Judge_Graphic_Obj_t* Obj,uint32_t W,uint32_t H)
{
		Judge_Graphic_Obj_Set_Width(Obj,W);
		Judge_Graphic_Obj_Set_Height(Obj,H);
}

uint32_t Judge_Graphic_Obj_Get_X(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		uint32_t X = 0;
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				uint32_t Ra = Obj->Graphic_Data.graphic_union.graphic_data.end_x;
				X = Obj->Graphic_Data.start_x - Ra/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				uint32_t R = Obj->Graphic_Data.graphic_union.graphic_data.radius;
				X = Obj->Graphic_Data.start_x - R/2;
		}
		else
		{
				X = Obj->Graphic_Data.start_x;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return X;
}

uint32_t Judge_Graphic_Obj_Get_Y(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		uint32_t Y = 0;
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				uint32_t Rb = Obj->Graphic_Data.graphic_union.graphic_data.end_y;
				Y = Obj->Graphic_Data.start_y - Rb/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				uint32_t R = Obj->Graphic_Data.graphic_union.graphic_data.radius;
				Y = Obj->Graphic_Data.start_y - R/2;
		}
		else
		{
				Y = Obj->Graphic_Data.start_y;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return Y;
}


void Judge_Graphic_Obj_Set_X(Judge_Graphic_Obj_t* Obj,uint32_t X)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				uint32_t Ra = Obj->Graphic_Data.graphic_union.graphic_data.end_x;
				Obj->Graphic_Data.start_x = X + Ra/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				uint32_t R = Obj->Graphic_Data.graphic_union.graphic_data.radius;
				Obj->Graphic_Data.start_x = X + R/2;
		}
		else
		{
				uint32_t W = Obj->Graphic_Data.graphic_union.graphic_data.end_x - Obj->Graphic_Data.start_x;
				Obj->Graphic_Data.start_x = X;
				Obj->Graphic_Data.graphic_union.graphic_data.end_x = X+W;
		}
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

void Judge_Graphic_Obj_Set_Y(Judge_Graphic_Obj_t* Obj,uint32_t Y)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				uint32_t Ra = Obj->Graphic_Data.graphic_union.graphic_data.end_y;
				Obj->Graphic_Data.start_y = Y + Ra/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				uint32_t R = Obj->Graphic_Data.graphic_union.graphic_data.radius;
				Obj->Graphic_Data.start_y = Y + R/2;
		}
		else
		{
				uint32_t H = Obj->Graphic_Data.graphic_union.graphic_data.end_y - Obj->Graphic_Data.start_y;
				Obj->Graphic_Data.start_y = Y;
				Obj->Graphic_Data.graphic_union.graphic_data.end_y = Y+H;
		}
		
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

void Judge_Graphic_Obj_Set_Pos(Judge_Graphic_Obj_t* Obj,uint32_t X,uint32_t Y)
{
		Judge_Graphic_Obj_Set_X(Obj,X);
		Judge_Graphic_Obj_Set_Y(Obj,Y);
}

uint32_t Judge_Graphic_Obj_Get_Center_X(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		uint32_t X = 0;
		
		if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				X = (Obj->Graphic_Data.start_x+Obj->Graphic_Data.graphic_union.graphic_data.end_x)/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC||
						Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				X = Obj->Graphic_Data.start_x;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return X;
}

uint32_t Judge_Graphic_Obj_Get_Center_Y(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		uint32_t Y;
	
		if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				Y = (Obj->Graphic_Data.start_y+Obj->Graphic_Data.graphic_union.graphic_data.end_y)/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC||
						Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				Y = Obj->Graphic_Data.start_y;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return Y;
}

void Judge_Graphic_Obj_Set_Center_X(Judge_Graphic_Obj_t* Obj,uint32_t Cx)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				uint32_t W = Obj->Graphic_Data.graphic_union.graphic_data.end_x - Obj->Graphic_Data.start_x;
				Obj->Graphic_Data.start_x = Cx - W/2;
				Obj->Graphic_Data.graphic_union.graphic_data.end_x = Cx + W/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC||
						Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				Obj->Graphic_Data.start_x = Cx;
		}
		
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

void Judge_Graphic_Obj_Set_Center_Y(Judge_Graphic_Obj_t* Obj,uint32_t Cy)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				uint32_t H = Obj->Graphic_Data.graphic_union.graphic_data.end_y - Obj->Graphic_Data.start_y;
				Obj->Graphic_Data.start_y = Cy - H/2;
				Obj->Graphic_Data.graphic_union.graphic_data.end_y = Cy + H/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC||
						Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				Obj->Graphic_Data.start_y = Cy;
		}
		
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

void Judge_Graphic_Obj_Set_Center_Pos(Judge_Graphic_Obj_t* Obj,uint32_t Cx,uint32_t Cy)
{
		Judge_Graphic_Obj_Set_Center_X(Obj,Cx);
		Judge_Graphic_Obj_Set_Center_Y(Obj,Cy);
}

uint32_t Judge_Graphic_Obj_Get_Radius_A(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		uint32_t Ra = 0;
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				Ra = Obj->Graphic_Data.graphic_union.graphic_data.end_x;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				Ra = Obj->Graphic_Data.graphic_union.graphic_data.radius;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				Ra = (Obj->Graphic_Data.graphic_union.graphic_data.end_x-Obj->Graphic_Data.start_x)/2;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return Ra;
}

uint32_t Judge_Graphic_Obj_Get_Radius_B(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		uint32_t Rb;
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				Rb = Obj->Graphic_Data.graphic_union.graphic_data.end_y;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				Rb = Obj->Graphic_Data.graphic_union.graphic_data.radius;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				Rb = (Obj->Graphic_Data.graphic_union.graphic_data.end_y-Obj->Graphic_Data.start_y)/2;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return Rb;
}

void Judge_Graphic_Obj_Set_Radius_A(Judge_Graphic_Obj_t* Obj,uint32_t Ra)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				Obj->Graphic_Data.graphic_union.graphic_data.end_x = Ra;
				
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
			 Obj->Graphic_Data.graphic_union.graphic_data.radius = Ra;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				uint32_t Cx = (Obj->Graphic_Data.start_x+Obj->Graphic_Data.graphic_union.graphic_data.end_x)/2;
				Obj->Graphic_Data.start_x = Cx - Ra/2;
				Obj->Graphic_Data.graphic_union.graphic_data.end_x = Cx + Ra/2;
		}
		
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

void Judge_Graphic_Obj_Set_Radius_B(Judge_Graphic_Obj_t* Obj,uint32_t Rb)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				Obj->Graphic_Data.graphic_union.graphic_data.end_y = Rb;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				Obj->Graphic_Data.graphic_union.graphic_data.radius = Rb;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				uint32_t Cy = (Obj->Graphic_Data.start_y+Obj->Graphic_Data.graphic_union.graphic_data.end_y)/2;
				Obj->Graphic_Data.start_x = Cy - Rb/2;
				Obj->Graphic_Data.graphic_union.graphic_data.end_x = Cy + Rb/2;
		}
		
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
		Judge_Graphic_Obj_Unlock(Obj);
}

uint32_t Judge_Graphic_Obj_Get_Start_Angle(Judge_Graphic_Obj_t* Obj,uint32_t Start_Angle)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		
		uint32_t As = 0;
		if(Obj->Graphic_Data.graphic_tpye == ARC)
		{
				As = Obj->Graphic_Data.start_angle;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return As;
}

uint32_t Judge_Graphic_Obj_Get_End_Angle(Judge_Graphic_Obj_t* Obj,uint32_t End_Angle)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		uint32_t Ae = 0;
		if(Obj->Graphic_Data.graphic_tpye == ARC)
		{
				Ae = Obj->Graphic_Data.end_angle;
		}
		
		Judge_Graphic_Obj_Unlock(Obj);
		return Ae;
} 


void Judge_Graphic_Obj_Set_Start_Angle(Judge_Graphic_Obj_t* Obj,uint32_t Start_Angle)
{
		Judge_Assert(Obj);
		if(Obj->Graphic_Data.graphic_tpye == ARC)
		{
				Judge_Graphic_Obj_Lock(Obj);
			
				Obj->Graphic_Data.start_angle = Start_Angle;
			
				Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
				Judge_Graphic_Obj_Unlock(Obj);
		}
}

void Judge_Graphic_Obj_Set_End_Angle(Judge_Graphic_Obj_t* Obj,uint32_t End_Angle)
{
		Judge_Assert(Obj);
		if(Obj->Graphic_Data.graphic_tpye == ARC)
		{
				Judge_Graphic_Obj_Lock(Obj);
			
				Obj->Graphic_Data.end_angle = End_Angle;
			
				Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
				Judge_Graphic_Obj_Unlock(Obj);
		}
} 

void Judge_Graphic_Obj_Set_Radius(Judge_Graphic_Obj_t* Obj,uint32_t R)
{
		Judge_Assert(Obj);
	
		Judge_Graphic_Obj_Set_Radius_B(Obj,R);
		Judge_Graphic_Obj_Set_Radius_A(Obj,R);
	
}

void Judge_Graphic_Obj_Set_Line_Width(Judge_Graphic_Obj_t* Obj,uint32_t W)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
	
		Obj->Graphic_Data.width = W;
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_CHANGE);
	
		Judge_Graphic_Obj_Unlock(Obj);
}

void Judge_Graphic_Obj_Del(Judge_Graphic_Obj_t* Obj)
{
		Judge_Assert(Obj);
		Judge_Graphic_Obj_Lock(Obj);
		Judge_Graphic_Obj_Apply_Opt(Obj,OPT_DELETE);
		Judge_Graphic_Obj_Unlock(Obj);
}
/********图形创建函数**********/

Judge_Graphic_Obj_t* Judge_Graphic_Ellipse_Create(uint32_t Cx,uint32_t Cy,uint32_t Ra,uint32_t Rb,uint32_t W)
{
		Judge_Graphic_Obj_t* Ellipse = Judge_Graphic_Obj_Init();
		Ellipse->Graphic_Data.graphic_tpye = ELLIPSE;
		Ellipse->Graphic_Data.start_x = Cx - Ra/2;
		Ellipse->Graphic_Data.start_y = Cy - Ra/2;
		Ellipse->Graphic_Data.graphic_union.graphic_data.end_x = (uint32_t)Cx + Ra/2;
		Ellipse->Graphic_Data.graphic_union.graphic_data.end_y = (uint32_t)Cy + Rb/2;	
		Ellipse->Graphic_Data.width = W;
	
		Judge_Graphic_Obj_Apply_Opt(Ellipse,OPT_ADD);
		Judge_Graphic_Obj_Unlock(Ellipse);
		return  Ellipse;
}

Judge_Graphic_Obj_t* Judge_Graphic_Circle_Create(uint32_t Cx,uint32_t Cy,uint32_t R,uint32_t W)
{
		Judge_Graphic_Obj_t* Circle = Judge_Graphic_Obj_Init();
		Circle->Graphic_Data.graphic_tpye = CIRCLE;
		Circle->Graphic_Data.start_x = Cx;
		Circle->Graphic_Data.start_y = Cy;
		Circle->Graphic_Data.graphic_union.graphic_data.radius = R;
		Circle->Graphic_Data.width = W;
	
		Judge_Graphic_Obj_Apply_Opt(Circle,OPT_ADD);
		Judge_Graphic_Obj_Unlock(Circle);
		return Circle;
}

Judge_Graphic_Obj_t* Judge_Graphic_Line_Create(uint32_t X1,uint32_t Y1,uint32_t X2,uint32_t Y2,uint32_t W)
{
		Judge_Graphic_Obj_t* Line = Judge_Graphic_Obj_Init();
		Line->Graphic_Data.graphic_tpye = LINE;
		Line->Graphic_Data.start_x = X1;
		Line->Graphic_Data.start_y = Y1;
		Line->Graphic_Data.graphic_union.graphic_data.end_x = X2;
		Line->Graphic_Data.graphic_union.graphic_data.end_y = Y2;	
		Line->Graphic_Data.width = W;
	
		Judge_Graphic_Obj_Apply_Opt(Line,OPT_ADD);
		Judge_Graphic_Obj_Unlock(Line);
		return Line;
}

Judge_Graphic_Obj_t* Judge_Graphic_Rect_Create(uint32_t X1,uint32_t Y1,uint32_t X2,uint32_t Y2,uint32_t W)
{
		Judge_Graphic_Obj_t* Rect = Judge_Graphic_Obj_Init();\
		Rect->Graphic_Data.graphic_tpye = RECTANGLE;
		Rect->Graphic_Data.start_x = X1;
		Rect->Graphic_Data.start_y = Y1;
		Rect->Graphic_Data.graphic_union.graphic_data.end_x = X2;
		Rect->Graphic_Data.graphic_union.graphic_data.end_y = Y2;	
		Rect->Graphic_Data.width = W;
	
		Judge_Graphic_Obj_Apply_Opt(Rect,OPT_ADD);
		Judge_Graphic_Obj_Unlock(Rect);
		return Rect;
}

Judge_Graphic_Obj_t* Judge_Graphic_Arc_Create(uint32_t A1,uint32_t A2,uint32_t Cx, uint32_t Cy,uint32_t R,uint32_t W)
{
		Judge_Graphic_Obj_t* Arc = Judge_Graphic_Obj_Init();
		Arc->Graphic_Data.graphic_tpye = ARC;
		Arc->Graphic_Data.start_angle = A1;
		Arc->Graphic_Data.end_angle = A2;
		Arc->Graphic_Data.start_x = Cx - R/2;
		Arc->Graphic_Data.start_y = Cy - R/2;
		Arc->Graphic_Data.graphic_union.graphic_data.end_x = Cx + R/2;
		Arc->Graphic_Data.graphic_union.graphic_data.end_y = Cy + R/2;
		Arc->Graphic_Data.width = W;
	
		Judge_Graphic_Obj_Apply_Opt(Arc,OPT_ADD);
		Judge_Graphic_Obj_Unlock(Arc);
		return Arc;
}

Judge_Graphic_Obj_t* Judge_Graphic_Float_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,float Val)
{
		Judge_Graphic_Obj_t* Float = Judge_Graphic_Obj_Init();
		Float->Graphic_Data.graphic_tpye = FLOAT;
		Float->Graphic_Data.start_x = X;
		Float->Graphic_Data.start_y = Y;
		Float->Graphic_Data.start_angle = Font_Size;
		Float->Graphic_Data.end_angle = 3;
		Float->Graphic_Data.width = Font_Size/10;
	
		Judge_Graphic_Obj_Apply_Opt(Float,OPT_ADD);
		Judge_Graphic_Obj_Unlock(Float);
		return Float;
}

Judge_Graphic_Obj_t* Judge_Graphic_Integer_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,int32_t Val)
{
		Judge_Graphic_Obj_t* Int = Judge_Graphic_Obj_Init();
		Int->Graphic_Data.graphic_tpye = INTEGER;
		Int->Graphic_Data.start_x = X;
		Int->Graphic_Data.start_y = Y;
		Int->Graphic_Data.start_angle = Font_Size;
		Int->Graphic_Data.width = Font_Size/10;
	
		Judge_Graphic_Obj_Apply_Opt(Int,OPT_ADD);
		Judge_Graphic_Obj_Unlock(Int);
		return Int;
}

Judge_Graphic_Obj_t* Judge_Graphic_Character_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,char* Str)
{
		Judge_Assert(Str);
		uint32_t Character_Len = strlen(Str);
		uint8_t* Character_Buff = pvPortMalloc(Character_Len);
		Judge_Assert(Character_Buff);
	
		Judge_Graphic_Obj_t* Character = Judge_Graphic_Obj_Init();
		
		Character->Graphic_Data.graphic_tpye = CHARACTER;
		Character->Graphic_Data.start_x = X;
		Character->Graphic_Data.start_y = Y;
		Character->Graphic_Data.start_angle = Font_Size;
		Character->Graphic_Data.end_angle = Character_Len;
		Character->Graphic_Data.width = Font_Size/10;
		memcpy(Character_Buff,Str,Character_Len);
		Character->Ext_Data = Character_Buff;
	
		Judge_Graphic_Obj_Apply_Opt(Character,OPT_ADD);
		Judge_Graphic_Obj_Unlock(Character);
		return Character;
}

//static void Judge_Graphic_Obj_Del_Check(Judge_Graphic_Obj_t* Obj)
//{
//		Judge_Assert(Obj);
//		if(Obj->Graphic_Data.operate_tpye == OPT_DELETE)
//		{
//			if(Obj->Ext_Data)
//				vPortFree(Obj->Ext_Data);
//			vPortFree(Obj);
//		}
//		else
//		{
//				Obj->Graphic_Data.operate_tpye = OPT_NONE;
//		}
//}


void Judge_Graphic_Init()
{
		DJI_Judge_Graphic.Judge_Obj_Counter = 1;
		List_Init(&DJI_Judge_Graphic.Graphic_Obj_List);
}

typedef __packed struct
{
		Judge_Graphic_Obj_t* Character_To_Send;
		Judge_Graphic_Obj_t* CommonObj_To_Send[7];
		uint8_t Num_Of_Character_To_Send;
		uint8_t Num_Of_CommonObj_To_Send;
		
}Judge_Graphic_Obj_To_Send_t;

int8_t Judge_Graphic_Obj_Iterator(ListItem_t* ListItem,void* User_Data1,void* User_Data2)
{
		Judge_Graphic_Obj_t* Obj = (Judge_Graphic_Obj_t*)(ListItem);
		Judge_Graphic_Obj_To_Send_t* Graphic_Obj_To_Send = (Judge_Graphic_Obj_To_Send_t*)User_Data1;
		//锁住图形对象
		Judge_Graphic_Obj_Lock(Obj);
		//如果操作不是空操作，那么加入到发送缓冲区内，不解锁图形对象
		if(Obj->Graphic_Data.operate_tpye!=OPT_NONE)
		{
				if(Obj->Graphic_Data.graphic_tpye==CHARACTER&&Graphic_Obj_To_Send->Num_Of_Character_To_Send==0)
				{
						Graphic_Obj_To_Send->Character_To_Send = Obj;
						Graphic_Obj_To_Send->Num_Of_Character_To_Send = 1;
				}
				else if(Obj->Graphic_Data.graphic_tpye!=CHARACTER&&Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send<7)
				{
						Graphic_Obj_To_Send->CommonObj_To_Send[Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send++]=Obj;
				}
		}
		//否则解锁图形对象
		else
		{
				Judge_Graphic_Obj_Unlock(Obj);
		}
		
		//如果两个发送缓冲区都满了，那么停止遍历，剩下的下次遍历
		if(Graphic_Obj_To_Send->Num_Of_Character_To_Send==1&&Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send==7)
			return 0;
		return 1;
}

static void Judge_Graphic_Obj_Del_Asyn(Judge_Graphic_Obj_t* Judge_Graphic_Obj)
{
		//先把对象移出链表
		List_Remove(&DJI_Judge_Graphic.Graphic_Obj_List,(ListItem_t*)Judge_Graphic_Obj,portMAX_DELAY);
		//删除互斥量
		vSemaphoreDelete(Judge_Graphic_Obj->Graphic_Obj_Mutex);
		//释放字符串占用的内存，如果存在的话
		if(Judge_Graphic_Obj->Ext_Data)
				vPortFree(Judge_Graphic_Obj->Ext_Data);
		//释放对象占用的内存
		vPortFree(Judge_Graphic_Obj);
}


static void Judge_Graphic_Obj_Send(Judge_Graphic_Obj_To_Send_t* Graphic_Obj_To_Send)
{
		if(Graphic_Obj_To_Send->Num_Of_Character_To_Send)
		{
				uint8_t* Send_Buff = pvPortMalloc(JUDGE_GRAPHIC_DATA_LEN+JUDGE_GRAPHIC_CHARACTER_LEN);
				//如果申请内存成功，那么写入数据，否则直接解锁对象
				if(Send_Buff)
				{
						memcpy(Send_Buff,&Graphic_Obj_To_Send->Character_To_Send->Graphic_Data,JUDGE_GRAPHIC_DATA_LEN);
						memcpy(&Send_Buff[JUDGE_GRAPHIC_DATA_LEN],Graphic_Obj_To_Send->Character_To_Send->Ext_Data,
										strlen((char*)(Graphic_Obj_To_Send->Character_To_Send->Ext_Data)));	
						//如果对象需要删除，那么直接删除，否则设置空操作后解锁
						if(Graphic_Obj_To_Send->Character_To_Send->Graphic_Data.operate_tpye==OPT_DELETE)
						{
							Judge_Graphic_Obj_Del_Asyn(Graphic_Obj_To_Send->Character_To_Send);
						}
						else
						{
							Graphic_Obj_To_Send->Character_To_Send->Graphic_Data.operate_tpye=OPT_NONE;
							Judge_Graphic_Obj_Unlock(Graphic_Obj_To_Send->Character_To_Send);
						}
						//最后发送数据
						Judge_Student_Data_Send(Send_Buff,JUDGE_GRAPHIC_DATA_LEN+JUDGE_GRAPHIC_CHARACTER_LEN,GRAPHIC_CMDID_CHARACTER,Target_Client);
				}
				else
				{
						Judge_Graphic_Obj_Unlock(Graphic_Obj_To_Send->Character_To_Send);
				}
		}
		
		if(Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send)
		{
				uint8_t Item_To_Send = 0;
				uint32_t CmdID = 0;
				if(Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send>6)
				{
					Item_To_Send = 7;
					CmdID = GRAPHIC_CMDID_SEVEN;
				}
				else if(Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send>4)
				{
					Item_To_Send = 5;
					CmdID = GRAPHIC_CMDID_FIVE;
				}
				else if(Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send>1)
				{
					Item_To_Send = 2;
					CmdID = GRAPHIC_CMDID_TWO;
				}
				else if(Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send>0)
				{
					Item_To_Send = 1;
					CmdID = GRAPHIC_CMDID_ONE;
				}
				
				uint8_t* Send_Buff = pvPortMalloc(JUDGE_GRAPHIC_DATA_LEN*Item_To_Send);
				//如果申请失败，令需要发送的数量为0
				if(!Send_Buff)
						Item_To_Send = 0;
				
				for(uint8_t i = 0;i < Graphic_Obj_To_Send->Num_Of_CommonObj_To_Send;i++)
				{
						//如果需要发送，那么写入发送缓冲区，否则直接解锁
						if(i<Item_To_Send)
						{
							memcpy(&Send_Buff[i*JUDGE_GRAPHIC_DATA_LEN],&Graphic_Obj_To_Send->CommonObj_To_Send[i]->Graphic_Data,JUDGE_GRAPHIC_DATA_LEN);
							//看是否是删除操作，如果是，那么删除这个对象，否则直接设为空操作并解锁
							if(Graphic_Obj_To_Send->CommonObj_To_Send[i]->Graphic_Data.operate_tpye==OPT_DELETE)
							{
								Judge_Graphic_Obj_Del_Asyn(Graphic_Obj_To_Send->CommonObj_To_Send[i]);
							}
							else
							{
								Graphic_Obj_To_Send->CommonObj_To_Send[i]->Graphic_Data.operate_tpye = OPT_NONE;
								Judge_Graphic_Obj_Unlock(Graphic_Obj_To_Send->CommonObj_To_Send[i]);
							}
							
						}
						else
						{
								Judge_Graphic_Obj_Unlock(Graphic_Obj_To_Send->CommonObj_To_Send[i]);
						}
				}
				
				if(Item_To_Send)
						Judge_Student_Data_Send(Send_Buff,Item_To_Send*JUDGE_GRAPHIC_DATA_LEN,CmdID,Target_Client);
		}
		
}

void Judge_Graphic_Handler(void)
{
		Judge_Graphic_Obj_To_Send_t Graphic_Obj_To_Send; 
		memset(&Graphic_Obj_To_Send,0,sizeof(Judge_Graphic_Obj_To_Send_t));
		Iterate_All_ListItem(&DJI_Judge_Graphic.Graphic_Obj_List,&Graphic_Obj_To_Send,NULL,Judge_Graphic_Obj_Iterator);	
		Judge_Graphic_Obj_Send(&Graphic_Obj_To_Send);
}