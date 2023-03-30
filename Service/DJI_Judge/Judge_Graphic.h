#ifndef _JUDGE_GRAPHIC_H_
#define _JUDGE_GRAPHIC_H_

#include "Judge_Data.h"
#include "freertos_usr_lib.h"

//发送数据长度
typedef enum
{
	Graphic_Delete_Long = 2,
	Graphic_Single_Long = 15,
	Graphic_double_Long = 30,
	Graphic_five_Long = 75,
	Graphic_seven_Long = 105,
	Graphic_character_Long = 45
}Judge_Graphic_Data_Long_t;

typedef enum
{
		GRAPHIC_SEVEN = 0x0104,
		GRAPHIC_FIVE = 0x0103,
		GRAPHIC_TWO = 0x0102,
		GRAPHIC_ONE = 0x0101,
		GRAPHIC_CHARACTER = 0x0110
}Judge_Graphic_CmdID_t;

//图形操作
typedef enum
{
	OPT_NONE,
	OPT_ADD,
	OPT_CHANGE,
	OPT_DELETE
}Judge_Graphic_Operation_t;

typedef enum
{
		COLOR_MAIN,
		COLOR_YELLOW,
		COLOR_GREEN,
		COLOR_ORANGE,
		COLOR_PURPLE,
		COLOR_PINK,
		COLOR_CYAN,
		COLOR_BLACK,
		COLOR_WHITE
}Judge_Graphic_Color_t;

//图形类型
typedef enum
{
	LINE,			//直线
	RECTANGLE,//矩形
	CIRCLE,		//正圆
	ELLIPSE,	//椭圆
	ARC,			//圆弧
	FLOAT,		//浮点数
	INTEGER,	//整型数
	CHARACTER //字符
}Judge_Graphic_Type_t;

//图形数据
typedef __packed struct
{
	uint32_t graphic_name:24;
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10;
	uint32_t start_x:11;
	uint32_t start_y:11; 
	__packed union
	{
		int32_t number;
		__packed struct
		{
			uint32_t radius:10;
			uint32_t end_x:11;
			uint32_t end_y:11;
		}graphic_data;
	}graphic_union;
}graphic_data_struct_t;

typedef __packed struct
{
		graphic_data_struct_t Graphic_Data;
		uint8_t* Ext_Data;
}Judge_Graphic_Obj_t;

typedef struct
{
	QueueHandle_t Judge_Graphic_CommonObj_Queue;
	QueueHandle_t Judge_Graphic_Character_Queue;
	uint32_t Judge_Obj_Counter;
}DJI_Judge_Graphic_t;

void Judge_Graphic_Init(void);
void Judge_Graphic_Obj_Set_Color(Judge_Graphic_Obj_t* Obj,Judge_Graphic_Color_t Color);
uint32_t Judge_Graphic_Obj_Get_Width(Judge_Graphic_Obj_t* Obj);
uint32_t Judge_Graphic_Obj_Get_Height(Judge_Graphic_Obj_t* Obj);
void Judge_Graphic_Obj_Set_Width(Judge_Graphic_Obj_t* Obj,uint32_t W);
void Judge_Graphic_Obj_Set_Height(Judge_Graphic_Obj_t* Obj,uint32_t H);
void Judge_Graphic_Obj_Set_Size(Judge_Graphic_Obj_t* Obj,uint32_t W,uint32_t H);
uint32_t Judge_Graphic_Obj_Get_X(Judge_Graphic_Obj_t* Obj);
uint32_t Judge_Graphic_Obj_Get_Y(Judge_Graphic_Obj_t* Obj);
void Judge_Graphic_Obj_Set_X(Judge_Graphic_Obj_t* Obj,uint32_t X);
void Judge_Graphic_Obj_Set_Y(Judge_Graphic_Obj_t* Obj,uint32_t Y);
void Judge_Graphic_Obj_Set_Pos(Judge_Graphic_Obj_t* Obj,uint32_t X,uint32_t Y);
uint32_t Judge_Graphic_Obj_Get_Center_X(Judge_Graphic_Obj_t* Obj);
uint32_t Judge_Graphic_Obj_Get_Center_Y(Judge_Graphic_Obj_t* Obj);
void Judge_Graphic_Obj_Set_Center_X(Judge_Graphic_Obj_t* Obj,uint32_t Cx);
void Judge_Graphic_Obj_Set_Center_Y(Judge_Graphic_Obj_t* Obj,uint32_t Cy);
void Judge_Graphic_Obj_Set_Center_Pos(Judge_Graphic_Obj_t* Obj,uint32_t Cx,uint32_t Cy);


void Judge_Graphic_Obj_Set_Radius_A(Judge_Graphic_Obj_t* Obj,uint32_t Ra);
void Judge_Graphic_Obj_Set_Radius_B(Judge_Graphic_Obj_t* Obj,uint32_t Rb);
void Judge_Graphic_Obj_Set_Radius(Judge_Graphic_Obj_t* Obj,uint32_t R);
void Judge_Graphic_Obj_Set_Line_Width(Judge_Graphic_Obj_t* Obj,uint32_t W);
void Judge_Graphic_Obj_Del(Judge_Graphic_Obj_t* Obj);
Judge_Graphic_Obj_t* Judge_Graphic_Ellipse_Create(uint32_t Cx,uint32_t Cy,uint32_t Ra,uint32_t Rb,uint32_t W);
Judge_Graphic_Obj_t* Judge_Graphic_Circle_Create(uint32_t Cx,uint32_t Cy,uint32_t R,uint32_t W);
Judge_Graphic_Obj_t* Judge_Graphic_Line_Create(uint32_t X1,uint32_t Y1,uint32_t X2,uint32_t Y2,uint32_t W);
Judge_Graphic_Obj_t* Judge_Graphic_Rect_Create(uint32_t X1,uint32_t Y1,uint32_t X2,uint32_t Y2,uint32_t W);
Judge_Graphic_Obj_t* Judge_Graphic_Arc_Create(uint32_t A1,uint32_t A2,uint32_t Cx, uint32_t Cy,uint32_t R,uint32_t W);
Judge_Graphic_Obj_t* Judge_Graphic_Float_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,float Val);
Judge_Graphic_Obj_t* Judge_Graphic_Integer_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,int32_t Val);
Judge_Graphic_Obj_t* Judge_Graphic_Character_Create(uint32_t X,uint32_t Y,uint32_t Font_Size,char* Str);


#endif