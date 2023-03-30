#include "Judge_Graphic.h"

DJI_Judge_Graphic_t DJI_Judge_Graphic;

#define JUDGE_GRAPHIC_STK_SIZE 256
#define JUDGE_GRAPHIC_TASK_PRIO 21
TaskHandle_t JudgeGraphicTask_Handler;

#define JUDGE_PTR_ASSERT(Ptr) if(!Ptr) return
#define JUDGE_PTR_ASSERT_EX(Ptr,Val) if(!Ptr) return Val
#define JUDGE_GRAPHIC_DATA_LEN 15
#define JUDGE_GRAPHIC_CHARACTER_LEN 30

#define Judge_Graphic_CommonObj_Opt_Apply(Obj) xQueueSend(DJI_Judge_Graphic.Judge_Graphic_CommonObj_Queue,&(Obj),portMAX_DELAY)
#define Judge_Graphic_Character_Opt_Apply(Obj) xQueueSend(DJI_Judge_Graphic.Judge_Graphic_Character_Queue,&(Obj),portMAX_DELAY)

/********基础操作函数**********/
static Judge_Graphic_Obj_t* Judge_Graphic_Obj_Init()
{
		if(DJI_Judge_Graphic.Judge_Obj_Counter > 0xFFF)
			return NULL;
			
		Judge_Graphic_Obj_t* Obj = pvPortMalloc(sizeof(Judge_Graphic_Obj_t));
		JUDGE_PTR_ASSERT_EX(Obj,NULL);
		Obj->Graphic_Data.graphic_name = DJI_Judge_Graphic.Judge_Obj_Counter++;
		return Obj;
}

void Judge_Graphic_Obj_Set_Color(Judge_Graphic_Obj_t* Obj,Judge_Graphic_Color_t Color)
{
		JUDGE_PTR_ASSERT(Obj);
		Obj->Graphic_Data.color = Color;
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

uint32_t Judge_Graphic_Obj_Get_Width(Judge_Graphic_Obj_t* Obj)
{
		JUDGE_PTR_ASSERT_EX(Obj,0);
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				return Obj->Graphic_Data.graphic_union.graphic_data.end_x;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				return Obj->Graphic_Data.graphic_union.graphic_data.radius;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				return Obj->Graphic_Data.graphic_union.graphic_data.end_x - Obj->Graphic_Data.start_x;
		}
		else
		{
				return 0;//todo
		}
}

uint32_t Judge_Graphic_Obj_Get_Height(Judge_Graphic_Obj_t* Obj)
{
		JUDGE_PTR_ASSERT_EX(Obj,0);
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				return Obj->Graphic_Data.graphic_union.graphic_data.end_y;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				return Obj->Graphic_Data.graphic_union.graphic_data.radius;
		}
		else if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				return Obj->Graphic_Data.graphic_union.graphic_data.end_y - Obj->Graphic_Data.start_y;
		}
		else
		{
				return 0;//todo
		}
}

void Judge_Graphic_Obj_Set_Width(Judge_Graphic_Obj_t* Obj,uint32_t W)
{
		JUDGE_PTR_ASSERT(Obj);
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
		else
		{
				//todo
		}
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Set_Height(Judge_Graphic_Obj_t* Obj,uint32_t H)
{
		JUDGE_PTR_ASSERT(Obj);
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
		else
		{
				//todo
		}
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Set_Size(Judge_Graphic_Obj_t* Obj,uint32_t W,uint32_t H)
{
		Judge_Graphic_Obj_Set_Width(Obj,W);
		Judge_Graphic_Obj_Set_Height(Obj,H);
}

uint32_t Judge_Graphic_Obj_Get_X(Judge_Graphic_Obj_t* Obj)
{
		JUDGE_PTR_ASSERT_EX(Obj,0);
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				uint32_t Ra = Obj->Graphic_Data.graphic_union.graphic_data.end_x;
				return Obj->Graphic_Data.start_x - Ra/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				uint32_t R = Obj->Graphic_Data.graphic_union.graphic_data.radius;
				return Obj->Graphic_Data.start_x - R/2;
		}
		else
		{
				return Obj->Graphic_Data.start_x;
		}
}

uint32_t Judge_Graphic_Obj_Get_Y(Judge_Graphic_Obj_t* Obj)
{
		JUDGE_PTR_ASSERT_EX(Obj,0);
		if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC)
		{
				uint32_t Rb = Obj->Graphic_Data.graphic_union.graphic_data.end_y;
				return Obj->Graphic_Data.start_y - Rb/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				uint32_t R = Obj->Graphic_Data.graphic_union.graphic_data.radius;
				return Obj->Graphic_Data.start_y - R/2;
		}
		else
		{
				return Obj->Graphic_Data.start_y;
		}
}


void Judge_Graphic_Obj_Set_X(Judge_Graphic_Obj_t* Obj,uint32_t X)
{
		JUDGE_PTR_ASSERT(Obj);
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
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Set_Y(Judge_Graphic_Obj_t* Obj,uint32_t Y)
{
		JUDGE_PTR_ASSERT(Obj);
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
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Set_Pos(Judge_Graphic_Obj_t* Obj,uint32_t X,uint32_t Y)
{
		Judge_Graphic_Obj_Set_X(Obj,X);
		Judge_Graphic_Obj_Set_Y(Obj,Y);
}

uint32_t Judge_Graphic_Obj_Get_Center_X(Judge_Graphic_Obj_t* Obj)
{
		JUDGE_PTR_ASSERT_EX(Obj,0);
		if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				return (Obj->Graphic_Data.start_x+Obj->Graphic_Data.graphic_union.graphic_data.end_x)/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC||
						Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				return Obj->Graphic_Data.start_x;
		}
		else
		{
				return 0;//todo
		}
}

uint32_t Judge_Graphic_Obj_Get_Center_Y(Judge_Graphic_Obj_t* Obj)
{
		JUDGE_PTR_ASSERT_EX(Obj,0);
		if(Obj->Graphic_Data.graphic_tpye == RECTANGLE||Obj->Graphic_Data.graphic_tpye == LINE)
		{
				return (Obj->Graphic_Data.start_y+Obj->Graphic_Data.graphic_union.graphic_data.end_y)/2;
		}
		else if(Obj->Graphic_Data.graphic_tpye == ELLIPSE||Obj->Graphic_Data.graphic_tpye == ARC||
						Obj->Graphic_Data.graphic_tpye == CIRCLE)
		{
				return Obj->Graphic_Data.start_y;
		}
		else
		{
				return 0;
		}
}

void Judge_Graphic_Obj_Set_Center_X(Judge_Graphic_Obj_t* Obj,uint32_t Cx)
{
		JUDGE_PTR_ASSERT(Obj);
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
		else
		{
				//todo
		}
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Set_Center_Y(Judge_Graphic_Obj_t* Obj,uint32_t Cy)
{
		JUDGE_PTR_ASSERT(Obj);
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
		else
		{
				//todo
		}
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Set_Center_Pos(Judge_Graphic_Obj_t* Obj,uint32_t Cx,uint32_t Cy)
{
		Judge_Graphic_Obj_Set_Center_X(Obj,Cx);
		Judge_Graphic_Obj_Set_Center_Y(Obj,Cy);
}

void Judge_Graphic_Obj_Set_Radius_A(Judge_Graphic_Obj_t* Obj,uint32_t Ra)
{
		JUDGE_PTR_ASSERT(Obj);
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
		else
		{
			 //todo
		}
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Set_Radius_B(Judge_Graphic_Obj_t* Obj,uint32_t Rb)
{
		JUDGE_PTR_ASSERT(Obj);
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
		else
		{
			 //todo
		}
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Set_Radius(Judge_Graphic_Obj_t* Obj,uint32_t R)
{
		Judge_Graphic_Obj_Set_Radius_B(Obj,R);
		Judge_Graphic_Obj_Set_Radius_A(Obj,R);
}

void Judge_Graphic_Obj_Set_Line_Width(Judge_Graphic_Obj_t* Obj,uint32_t W)
{
		Obj->Graphic_Data.width = W;
		Obj->Graphic_Data.operate_tpye = OPT_CHANGE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}

void Judge_Graphic_Obj_Del(Judge_Graphic_Obj_t* Obj)
{
		JUDGE_PTR_ASSERT(Obj);
		Obj->Graphic_Data.operate_tpye = OPT_DELETE;
		Judge_Graphic_CommonObj_Opt_Apply(Obj);
}
/********图形创建函数**********/

Judge_Graphic_Obj_t* Judge_Graphic_Ellipse_Create(uint32_t Cx,uint32_t Cy,uint32_t Ra,uint32_t Rb,uint32_t W)
{
		Judge_Graphic_Obj_t* Ellipse = Judge_Graphic_Obj_Init();
		JUDGE_PTR_ASSERT_EX(Ellipse,NULL);
		Ellipse->Graphic_Data.graphic_tpye = ELLIPSE;
		Ellipse->Graphic_Data.start_x = Cx - Ra/2;
		Ellipse->Graphic_Data.start_y = Cy - Ra/2;
		Ellipse->Graphic_Data.graphic_union.graphic_data.end_x = (uint32_t)Cx + Ra/2;
		Ellipse->Graphic_Data.graphic_union.graphic_data.end_y = (uint32_t)Cy + Rb/2;	
		Ellipse->Graphic_Data.width = W;
		Ellipse->Graphic_Data.operate_tpye = OPT_ADD;
		Judge_Graphic_CommonObj_Opt_Apply(Ellipse);
		return  Ellipse;
}

Judge_Graphic_Obj_t* Judge_Graphic_Circle_Create(uint32_t Cx,uint32_t Cy,uint32_t R,uint32_t W)
{
		Judge_Graphic_Obj_t* Circle = Judge_Graphic_Ellipse_Create(Cx,Cy,R,R,W);
		JUDGE_PTR_ASSERT_EX(Circle,NULL);
		Circle->Graphic_Data.graphic_tpye = CIRCLE;
		Judge_Graphic_CommonObj_Opt_Apply(Circle);
		return Circle;
}

Judge_Graphic_Obj_t* Judge_Graphic_Line_Create(uint32_t X1,uint32_t Y1,uint32_t X2,uint32_t Y2,uint32_t W)
{
		Judge_Graphic_Obj_t* Line = Judge_Graphic_Obj_Init();
		JUDGE_PTR_ASSERT_EX(Line,NULL);
		Line->Graphic_Data.graphic_tpye = LINE;
		Line->Graphic_Data.start_x = X1;
		Line->Graphic_Data.start_y = Y1;
		Line->Graphic_Data.graphic_union.graphic_data.end_x = X2;
		Line->Graphic_Data.graphic_union.graphic_data.end_y = Y2;	
		Line->Graphic_Data.width = W;
		Line->Graphic_Data.operate_tpye = OPT_ADD;
		Judge_Graphic_CommonObj_Opt_Apply(Line);
		return Line;
}

Judge_Graphic_Obj_t* Judge_Graphic_Rect_Create(uint32_t X1,uint32_t Y1,uint32_t X2,uint32_t Y2,uint32_t W)
{
		Judge_Graphic_Obj_t* Rect = Judge_Graphic_Obj_Init();
		JUDGE_PTR_ASSERT_EX(Rect,NULL);
		Rect->Graphic_Data.graphic_tpye = RECTANGLE;
		Rect->Graphic_Data.start_x = X1;
		Rect->Graphic_Data.start_y = Y1;
		Rect->Graphic_Data.graphic_union.graphic_data.end_x = X2;
		Rect->Graphic_Data.graphic_union.graphic_data.end_y = Y2;	
		Rect->Graphic_Data.width = W;
		Rect->Graphic_Data.operate_tpye = OPT_ADD;
		Judge_Graphic_CommonObj_Opt_Apply(Rect);
		return Rect;
}

Judge_Graphic_Obj_t* Judge_Graphic_Arc_Create(uint32_t A1,uint32_t A2,uint32_t Cx, uint32_t Cy,uint32_t R,uint32_t W)
{
		Judge_Graphic_Obj_t* Arc = Judge_Graphic_Obj_Init();
		JUDGE_PTR_ASSERT_EX(Arc,NULL);
		Arc->Graphic_Data.graphic_tpye = ARC;
		Arc->Graphic_Data.start_angle = A1;
		Arc->Graphic_Data.end_angle = A2;
		Arc->Graphic_Data.start_x = Cx - R/2;
		Arc->Graphic_Data.start_y = Cy - R/2;
		Arc->Graphic_Data.graphic_union.graphic_data.end_x = Cx + R/2;
		Arc->Graphic_Data.graphic_union.graphic_data.end_y = Cy + R/2;
		Arc->Graphic_Data.width = W;
		Arc->Graphic_Data.operate_tpye = OPT_ADD;
		Judge_Graphic_CommonObj_Opt_Apply(Arc);
		return Arc;
}

Judge_Graphic_Obj_t* Judge_Graphic_Float_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,float Val)
{
		Judge_Graphic_Obj_t* Float = Judge_Graphic_Obj_Init();
		JUDGE_PTR_ASSERT_EX(Float,NULL);
		Float->Graphic_Data.graphic_tpye = FLOAT;
		Float->Graphic_Data.start_x = X;
		Float->Graphic_Data.start_y = Y;
		Float->Graphic_Data.start_angle = Font_Size;
		Float->Graphic_Data.end_angle = 3;
		Float->Graphic_Data.width = Font_Size/10;
		Float->Graphic_Data.operate_tpye = OPT_ADD;
		Judge_Graphic_CommonObj_Opt_Apply(Float);
		return Float;
}

Judge_Graphic_Obj_t* Judge_Graphic_Integer_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,int32_t Val)
{
		Judge_Graphic_Obj_t* Int = Judge_Graphic_Obj_Init();
		JUDGE_PTR_ASSERT_EX(Int,NULL);
		Int->Graphic_Data.graphic_tpye = INTEGER;
		Int->Graphic_Data.start_x = X;
		Int->Graphic_Data.start_y = Y;
		Int->Graphic_Data.start_angle = Font_Size;
		Int->Graphic_Data.width = Font_Size/10;
		Int->Graphic_Data.operate_tpye = OPT_ADD;
		Judge_Graphic_CommonObj_Opt_Apply(Int);
		return Int;
}

Judge_Graphic_Obj_t* Judge_Graphic_Character_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,char* Str)
{
		uint32_t Character_Len = strlen(Str);
		uint8_t* Character_Buff = pvPortMalloc(Character_Len);
		JUDGE_PTR_ASSERT_EX(Str,NULL);
	
		Judge_Graphic_Obj_t* Character = Judge_Graphic_Obj_Init();
		if(!Character)
		{
				vPortFree(Character_Buff);
				return NULL;
		}
		Character->Graphic_Data.graphic_tpye = CHARACTER;
		Character->Graphic_Data.start_x = X;
		Character->Graphic_Data.start_y = Y;
		Character->Graphic_Data.start_angle = Font_Size;
		Character->Graphic_Data.end_angle = Character_Len;
		Character->Graphic_Data.width = Font_Size/10;
		memcpy(Character_Buff,Str,Character_Len);
		Character->Ext_Data = Character_Buff;
		Character->Graphic_Data.operate_tpye = OPT_ADD;
		Judge_Graphic_Character_Opt_Apply(Character);
		return Character;
}

static void Judge_Graphic_Obj_Del_Check(Judge_Graphic_Obj_t* Obj)
{
		JUDGE_PTR_ASSERT(Obj);
		if(Obj->Graphic_Data.operate_tpye == OPT_DELETE)
		{
			if(Obj->Ext_Data)
				vPortFree(Obj->Ext_Data);
			vPortFree(Obj);
		}
		else
		{
				Obj->Graphic_Data.operate_tpye = OPT_NONE;
		}
}

static uint8_t Judge_Graphic_CommonObj_Send()
{
		uint32_t Items_In_Queue = uxQueueMessagesWaiting(DJI_Judge_Graphic.Judge_Graphic_CommonObj_Queue);
				
		uint32_t Items_To_Send = 0;
		uint16_t CmdID;
		if(Items_In_Queue>=7)
		{
				Items_To_Send = 7;
				CmdID = GRAPHIC_SEVEN;
		}
		else if(Items_In_Queue>=5)
		{
				Items_To_Send = 5;
				CmdID = GRAPHIC_FIVE;
		}
		else if(Items_In_Queue>=2)
		{
				Items_To_Send = 2;
				CmdID = GRAPHIC_TWO;
		}
		else if(Items_In_Queue>=1)
		{
				Items_To_Send = 1;
				CmdID = GRAPHIC_ONE;
		}
		
		if(Items_To_Send>0)
		{
			uint8_t* Data = pvPortMalloc(Items_To_Send*JUDGE_GRAPHIC_DATA_LEN);
			uint8_t* Ptr = Data;
			
			for(uint8_t i = 1; i <= Items_To_Send; i++)
			{
					Judge_Graphic_Obj_t* Obj;
					xQueueReceive(DJI_Judge_Graphic.Judge_Graphic_CommonObj_Queue,&Obj,portMAX_DELAY);
					memcpy(Ptr,Obj,JUDGE_GRAPHIC_DATA_LEN);
					Judge_Graphic_Obj_Del_Check(Obj);
					Ptr += JUDGE_GRAPHIC_DATA_LEN;
			}
			Judge_Student_Data_Send(Data,Items_To_Send*JUDGE_GRAPHIC_DATA_LEN,CmdID,Target_Client);
			vPortFree(Data);
			return 1;
		}	
		return 0;
}

static uint8_t Judge_Graphic_Character_Send()
{
		uint32_t Items_In_Queue = uxQueueMessagesWaiting(DJI_Judge_Graphic.Judge_Graphic_Character_Queue);
	
		if(Items_In_Queue>0)
		{
				uint8_t* Data = pvPortMalloc(JUDGE_GRAPHIC_DATA_LEN+JUDGE_GRAPHIC_CHARACTER_LEN);
				Judge_Graphic_Obj_t* Obj;
				xQueueReceive(DJI_Judge_Graphic.Judge_Graphic_Character_Queue,&Obj,portMAX_DELAY);
				memcpy(Data,Obj,JUDGE_GRAPHIC_DATA_LEN);
				memcpy(Data+JUDGE_GRAPHIC_DATA_LEN,Obj->Ext_Data,strlen((char*)(Obj->Ext_Data)));
				Judge_Graphic_Obj_Del_Check(Obj);
				uint16_t CmdID = GRAPHIC_CHARACTER;
				Judge_Student_Data_Send(Data,JUDGE_GRAPHIC_DATA_LEN+JUDGE_GRAPHIC_CHARACTER_LEN,CmdID,Target_Client);
				vPortFree(Data);
				return 1;
		}
		return 0;
}

void Judge_Graphic_Task(void *pvParameters)
{
		while(1)
		{
				//优先发送图形
				if(!Judge_Graphic_CommonObj_Send())
				{
					 Judge_Graphic_Character_Send();
				}
				vTaskDelay(100);
		}
}

void Judge_Graphic_Init()
{
		 Judge_Data_Init();
		 DJI_Judge_Graphic.Judge_Graphic_CommonObj_Queue= xQueueCreate(10,sizeof(Judge_Graphic_Obj_t*));
		 DJI_Judge_Graphic.Judge_Graphic_Character_Queue= xQueueCreate(10,sizeof(Judge_Graphic_Obj_t*));
		 DJI_Judge_Graphic.Judge_Obj_Counter = 1;
		 xTaskCreate((TaskFunction_t)Judge_Graphic_Task,          //任务函数
                (const char *)"Judge_Graphic_Task",          //任务名称
                (uint16_t)JUDGE_GRAPHIC_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                       //传递给任务函数的参数
                (UBaseType_t)JUDGE_GRAPHIC_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&JudgeGraphicTask_Handler); //任务句柄
}