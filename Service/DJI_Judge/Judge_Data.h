#ifndef __JUDGE_DATA_H__
#define __JUDGE_DATA_H__

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#define DJI_Judge_Student_CmdID 0x0301

#define DJI_Judge_Buf_Len_Max 200

#define DJI_Judge_Header_SOF 0xA5

#define Judge_Header_LEN 5
#define Judge_Cmd_id_LEN 2
#define Judge_CRC16_LEN 2

typedef enum
{
	Judge_Data_Set_OK = 1,
	Judge_Data_Set_Error = 0
}Judge_Data_SeT_State;

/**
  * @brief  frame header structure definition
  */
typedef __packed struct
{
  uint8_t  sof;
  uint16_t data_length;
  uint8_t  seq;
  uint8_t  crc8;
} frame_header_t;



typedef __packed struct
{
 uint16_t data_cmd_id;
 uint16_t sender_ID;
 uint16_t receiver_ID;
}Judge_student_interactive_header_data_t;

typedef __packed struct
{
	uint8_t* Data;
}Judge_student_data_t;

typedef __packed struct
{
	//头帧
  frame_header_t frame_header;
	//命令码
	uint16_t cmdid;
	//数据
	uint8_t* Data;
	//帧尾校验
	uint16_t crc16;
} Judge_Get_Data_t;

typedef __packed struct
{
	//头帧
  frame_header_t frame_header;
	//命令码
	uint16_t cmdid;
	//学生串口发送内容ID，发送者ID，接收者ID
	Judge_student_interactive_header_data_t Judge_student_interactive_header_data;	
	//数据
//	Judge_student_data_t Judge_student_data;
	//帧尾校验
	uint16_t crc16;
} Judge_Send_Data_t;

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


//图形操作
typedef enum
{
	Graphic_None = 0,
	Graphic_Add = 1,
	Graphic_Change = 2,
	Graphic_Delete = 3
}Judge_Graphic_Control_t;

//图形类型
typedef enum
{
	Straight_Line = 0,//直线
	Rectangle = 1,//矩形
	All_Circle = 2,//正圆
	Oval_Circle = 3,//椭圆
	Arc_Circle = 4,//圆弧
	Floating = 5,//浮点数
	Integer = 6,//整型数
	Character = 7//字符
}Judge_Graphic_Type_t;

//图形数据
typedef __packed struct
{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10;
	uint32_t start_x:11;
	uint32_t start_y:11; 
	uint32_t radius:10;
	uint32_t end_x:11;
	uint32_t end_y:11;
//	union Graphoc_Last_Data_t Graphoc_Last_Data;480
}graphic_data_struct_t;


//图形数据
typedef __packed struct
{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10;
	uint32_t start_x:11;
	uint32_t start_y:11; 
	uint8_t Word_Data[4];
//	uint32_t radius:10;
//	uint32_t end_x:11;
//	uint32_t end_y:11;
//	union Graphoc_Last_Data_t Graphoc_Last_Data;480
}Word_data_struct_t;



//客户端删除图形 机器人间通信：0x0301
typedef __packed struct
{
	uint8_t operate_tpye;
	uint8_t layer;
}Judge_client_custom_graphic_delete_t;







//客户端绘制一个图形
typedef __packed struct
{
 graphic_data_struct_t grapic_data_struct;
}Judge_client_custom_graphic_single_t;

//客户端绘制二个图形
typedef __packed struct
{
graphic_data_struct_t grapic_data_struct[2];
}Judge_client_custom_graphic_double_t;

//客户端绘制五个图形
typedef __packed struct
{
graphic_data_struct_t grapic_data_struct[5];
}Judge_client_custom_graphic_five_t;

//客户端绘制七个图形
typedef __packed struct
{
graphic_data_struct_t grapic_data_struct[7];
}Judge_client_custom_graphic_seven_t;

//客户端绘制字符 
typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct;
	uint8_t data[30];
}Judge_client_custom_character_t;

union Judge_Graphic_Client_Single_t
{
		Word_data_struct_t Word_data_struct;
	graphic_data_struct_t grapic_data_struct;
	uint8_t Judge_client_custom_graphic_single_Data[15];
};

union Judge_Graphic_Client_Double_t
{
	Word_data_struct_t Word_data_struct[2];
	graphic_data_struct_t grapic_data_struct[2];
	uint8_t Judge_client_custom_graphic_double_Data[30];
};

union Judge_Graphic_Client_Five_t
{
	Word_data_struct_t Word_data_struct[5];
	graphic_data_struct_t grapic_data_struct[5];
	uint8_t Judge_client_custom_graphic_five_Data[75];
};

union Judge_Graphic_Client_Seven_t
{
	Word_data_struct_t Word_data_struct[7];
	graphic_data_struct_t grapic_data_struct[7];
	uint8_t Judge_client_custom_graphic_seven_Data[105];
};

//union Judge_Character_Client_t
//{
//	graphic_data_struct_t grapic_data_struct;
//	uint8_t Judge_client_custom_graphic_single_Data[15];
//};


union Judge_Character_Client_t
{
	Judge_client_custom_character_t Judge_client_custom_character;
	uint8_t Judge_Character_Data[45];
};

union Judge_Graphic_Delet_t
{
	Judge_client_custom_graphic_delete_t Judge_client_custom_graphic_delete;
	uint8_t Judge_Character_Delete_Data[2];
};


//客户端发送数据
typedef struct
{
	union Judge_Graphic_Delet_t Judge_Graphic_Delet;
	union Judge_Graphic_Client_Single_t Judge_Graphic_Client_Single;
	union Judge_Graphic_Client_Double_t Judge_Graphic_Client_Double;
	union Judge_Graphic_Client_Five_t Judge_Graphic_Client_Five;
	union Judge_Graphic_Client_Seven_t Judge_Graphic_Client_Seven;
	union	Judge_Character_Client_t Judge_Character_Client;
}DJI_Judge_Send_Msg_t;

/*******************************************************************************
	--------------------------------Cmd_ID----------------------------------------
	0x0001	 11 		比赛状态数据，1Hz 周期发送
	0x0002	 1 		比赛结果数据，比赛结束后发送
	0x0003	 28 	比赛机器人血量数据，1Hz 周期发送
	0x0004	 3 		飞镖发射状态，飞镖发射时发送
	0x0101	 4 		场地事件数据，1Hz 周期发送
	0x0102	 3 		场地补给站动作标识数据，动作发生后发送
	0x0103	 2 		请求补给站补弹数据，由参赛队发送，上限 10Hz。（RM 对抗赛尚未开放）
	0x0104	 2 		裁判警告数据，警告发生后发送
	0x0105	 1 		飞镖发射口倒计时，1Hz 周期发送
	0x0201	 27 	机器人状态数据，10Hz 周期发送
	0x0202	 14 	实时功率热量数据，50Hz 周期发送
	0x0203	 16 	机器人位置数据，10Hz 发送
	0x0204	 1 		机器人增益数据，1Hz 周期发送
	0x0205	 3 		空中机器人能量状态数据，10Hz 周期发送，只有空中机器人主控发送
	0x0206	 1 		伤害状态数据，伤害发生后发送
	0x0207	 6 		实时射击数据，子弹发射后发送
	0x0208	 2 		弹丸剩余发射数，仅空中机器人，哨兵机器人以及 ICRA 机器人发送，1Hz 周期发送
	0x0209	 4 		机器人 RFID 状态，1Hz 周期发送
	0x020A	 12 	飞镖机器人客户端指令数据，10Hz 周期发送
	0x0301 	 n 		机器人间交互数据，发送方触发发送
	0x0302 	 n 		定义控制器交互数据接口，通过客户端触发发送，上限 30Hz
	0x0303 	 15 		客户端小地图交互数据，触发发送
	0x0304 	 12 		键盘、鼠标信息，通过图传串口发送
	-----------------------------------------------------------------------------
******************************************************************************/

//命令码ID,用来判断接收的是什么数据
typedef enum
{ 
	Judge_CmdID_Game_State = 0x0001,
	Judge_CmdID_Game_Result = 0x0002,
	Judge_CmdID_Robot_Blood = 0x0003,
	Judge_CmdID_Missile_State = 0x0004,
	Judge_CmdID_Area_Event = 0x0101,
	Judge_CmdID_Area_Supply_Station_Action_Data = 0x0102,
	Judge_CmdID_Waening_Data = 0x0104,
	Judge_CmdID_Missile_Time_Limit_Count = 0x0105,
	Judge_CmdID_Robot_State = 0x0201,
	Judge_CmdID_Real_Time_Power_Heat = 0x0202,
	Judge_CmdID_Robot_Position_Data = 0x0203,
	Judge_CmdID_Robot_Gain_Data = 0x0204,
	Judge_CmdID_Aerial_Robot_Power_Data = 0x0205,
	Judge_CmdID_Heat_Data = 0x0206,
	Judge_CmdID_Real_Time_Shoot_Data = 0x0207,
	Judge_CmdID_Bullet_Shoot_Limit = 0x0208,
	Judge_CmdID_Robot_RFID_State = 0x0209,
	Judge_CmdID_Missile_Client_Control_Data = 0x020A,
	Judge_CmdID_Robot_Communication_Data = 0x0301
}DJI_Judge_CmdID_t;

typedef enum
{ 
	Judge_Len_Game_State = 11,
	Judge_Len_Game_Result = 1,
	Judge_Len_Robot_Blood = 32,
	Judge_Len_Missile_State = 3,
	Judge_Len_Area_Event = 4,
	Judge_Len_Area_Supply_Station_Action_Data = 4,
	Judge_Len_Waening_Data = 2,
	Judge_Len_Missile_Time_Limit_Count = 1,
	Judge_Len_Robot_State = 27,
	Judge_Len_Real_Time_Power_Heat = 16,
	Judge_Len_Robot_Position_Data = 16,
	Judge_Len_Robot_Gain_Data = 1,
	Judge_Len_Aerial_Robot_Power_Data = 1,
	Judge_Len_Heat_Data = 1,
	Judge_Len_Real_Time_Shoot_Data = 7,
	Judge_Len_Bullet_Shoot_Limit = 2,
	Judge_Len_Robot_RFID_State = 4,
	Judge_Len_Missile_Client_Control_Data = 12,
//	Judge_Len_Robot_Communication_Data = n
}DJI_Judge_Len_t;

/***
比赛状态数据
CMD_ID:	0x0001
发送频率：1Hz
发送范围：所有机器人
****/
typedef __packed struct
{
 uint8_t game_type : 4;
 uint8_t game_progress : 4;
 uint16_t stage_remain_time;
 uint64_t SyncTimeStamp;
}Judge_game_status_t;

/***
比赛结果数据
CMD_ID:	0x0002
发送频率：比赛结束后发送
发送范围：所有机器人
****/
typedef __packed struct
{
 uint8_t winner;
}Judge_game_result_t;

/***
机器人血量数据
CMD_ID:	0x0003
发送频率：1Hz
发送范围：所有机器人。
****/
typedef __packed struct
{
 uint16_t red_1_robot_HP;
 uint16_t red_2_robot_HP;
 uint16_t red_3_robot_HP;
 uint16_t red_4_robot_HP;
 uint16_t red_5_robot_HP;
 uint16_t red_7_robot_HP;
 uint16_t red_outpost_HP;
 uint16_t red_base_HP;
 uint16_t blue_1_robot_HP;
 uint16_t blue_2_robot_HP;
 uint16_t blue_3_robot_HP;
 uint16_t blue_4_robot_HP;
 uint16_t blue_5_robot_HP;
 uint16_t blue_7_robot_HP;
 uint16_t blue_outpost_HP;
 uint16_t blue_base_HP;
}Judge_game_robot_HP_t;

/***
飞镖发射状态
CMD_ID:	0x0004
发送频率：飞镖发射后发送，
发送范围：所有机器人。
****/
typedef __packed struct
{
 uint8_t dart_belong;
 uint16_t stage_remaining_time;
}Judge_dart_status_t;

/***
场地事件数据
CMD_ID:	0x0101。
发送频率：1Hz
发送范围：己方机器人。
****/
typedef __packed struct
{
 uint32_t event_type;
}Judge_event_data_t;

/***
补给站动作标识
CMD_ID:	0x0102
发送频率：动作触发后发送
发送范围：己方机器人。
****/
typedef __packed struct
{
 uint8_t supply_projectile_id;
 uint8_t supply_robot_id;
 uint8_t supply_projectile_step;
uint8_t supply_projectile_num;
}Judge_supply_projectile_action_t;


/***
裁判警告信息
CMD_ID:	0x0104
发送频率：警告发生后发送
发送范围：己方机器人。
****/
typedef __packed struct
{
 uint8_t level;
 uint8_t foul_robot_id;
}Judge_referee_warning_t;

/***
飞镖发射口倒计时
CMD_ID:	0x0105
发送频率：1Hz
发送范围：己方机器人。
****/
typedef __packed struct
{
 uint8_t dart_remaining_time;
}Judge_dart_remaining_time_t;

/***
比赛机器人状态
CMD_ID:	0x0201
发送频率：10Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint8_t robot_id;
 uint8_t robot_level;
 uint16_t remain_HP;
 uint16_t max_HP;
 uint16_t shooter_heat0_cooling_rate;
 uint16_t shooter_heat0_cooling_limit;
 uint16_t shooter_heat0_speed_limit;
 uint16_t shooter_heat1_cooling_rate;
 uint16_t shooter_heat1_cooling_limit;
 uint16_t shooter_heat1_speed_limit;
 uint16_t shooter_42mm_cooling_rate;
 uint16_t shooter_42mm_cooling_limit;
 uint16_t shooter_42mm_speed_limit;	
 uint16_t max_chassis_power;
 uint8_t mains_power_gimbal_output : 1;
 uint8_t mains_power_chassis_output : 1;
 uint8_t mains_power_shooter_output : 1;
}Judge_game_robot_status_t;


/***
实时功率热量数据
CMD_ID:	0x0202。
发送频率：50Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint16_t chassis_volt;
 uint16_t chassis_current;
 float chassis_power;
 uint16_t chassis_power_buffer;
 uint16_t shooter_heat0;
 uint16_t shooter_heat1;
 uint16_t mobile_shooter_heat2;
}Judge_power_heat_data_t;


/***
机器人位置
CMD_ID:	0x0203
发送频率：10Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
 float x;
 float y;
 float z;
 float yaw;
}Judge_game_robot_pos_t;


/***
机器人增益
CMD_ID:	0x0204
发送频率：1Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint8_t power_rune_buff;
}Judge_buff_t;


/***
空中机器人能量状态
CMD_ID:	0x0205
发送频率：10Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint8_t attack_time;
}Judge_aerial_robot_energy_t;


/***
伤害状态
CMD_ID:	0x0206
发送频率：伤害发生后发送
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint8_t armor_id : 4;
 uint8_t hurt_type : 4;
}Judge_robot_hurt_t;

/***
实时射击信息
CMD_ID:	0x0207
发送频率：射击后发送
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint8_t bullet_type;
 uint8_t shoot_ID;
 uint8_t bullet_freq;
 float bullet_speed;
}Judge_shoot_data_t;

/***
子弹剩余发射数
CMD_ID:	0x0208
发送频率：空中机器人，哨兵机器人发送
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint16_t bullet_remaining_num;
}Judge_bullet_remaining_t;


/***
机器人 RFID 状态：
CMD_ID:	0x0209
发送频率：1Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint32_t rfid_status;
}Judge_RFID_status_t;

/***
飞镖机器人客户端指令数据
CMD_ID:	0x020A
发送频率：10Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
 uint8_t dart_launch_opening_status;
 uint8_t dart_attack_target;
 uint16_t target_change_time;
// uint8_t first_dart_speed;
// uint8_t second_dart_speed;
// uint8_t third_dart_speed;
// uint8_t fourth_dart_speed;
// uint16_t last_dart_launch_time;
 uint16_t operate_launch_cmd_time;
}Judge_dart_client_cmd_t;

typedef __packed struct
{
	Judge_student_interactive_header_data_t Judge_student_interactive_header_data;
	uint8_t Judge_Student_Data_Get[113];	
}Judge_Student_Data_t;


/***
兵种名单
****/
typedef enum
{

	Red_Hero = 1,
	Red_Engineer = 2,
	Red_Standard_3 = 3,
	Red_Standard_4 = 4,
	Red_Standard_5 = 5,
	Red_Aerial = 6,
	Red_Sentry = 7,
	Red_DartLaunch = 8,
	Red_Radar = 9,
	Blue_Hero = 101,
	Blue_Engineer = 102,
	Blue_Standard_3 = 103,
	Blue_Standard_4 = 104,
	Blue_Standard_5 = 105,
	Blue_Aerial = 106,
	Blue_Sentry = 107,
	Blue_DartLaunch = 108,
	Blue_Radar = 109,
		
} Judge_Robot_ID_t;

typedef struct
{
	Judge_Robot_ID_t Judge_Robot_ID;
	Judge_game_status_t Judge_game_status;
	Judge_game_result_t Judge_game_result;
	Judge_game_robot_HP_t Judge_game_robot_HP;
	Judge_dart_status_t Judge_dart_status;
	Judge_event_data_t Judge_event_data;
	Judge_supply_projectile_action_t Judge_supply_projectile_action;
	Judge_referee_warning_t Judge_referee_warning;
	Judge_dart_remaining_time_t Judge_dart_remaining_time;
	Judge_game_robot_status_t Judge_game_robot_status;
	Judge_power_heat_data_t Judge_power_heat_data;
	Judge_game_robot_pos_t Judge_game_robot_pos;
	Judge_buff_t Judge_buff;
	Judge_aerial_robot_energy_t Judge_aerial_robot_energy;
	Judge_robot_hurt_t Judge_robot_hurt;
	Judge_shoot_data_t Judge_shoot_data;
	Judge_bullet_remaining_t Judge_bullet_remaining;
	Judge_RFID_status_t Judge_RFID_status;
	Judge_dart_client_cmd_t Judge_dart_client_cmd;
	Judge_Student_Data_t Judge_Student_Data;
	
} DJI_Judge_Msg_t;



typedef struct
{
	
	int Data_len;
	Judge_Get_Data_t Judge_Data;
	Judge_Send_Data_t Judge_Send_Data;
	DJI_Judge_Msg_t DJI_Judge_Msg;
	DJI_Judge_Send_Msg_t DJI_Judge_Send_Msg;
} DJI_Judge_t;


void Judge_Graphic_Data_Set(graphic_data_struct_t* graphic_data,uint8_t* Graphic_Name,Judge_Graphic_Type_t Judge_Graphic_Type,Judge_Graphic_Control_t Judge_Graphic_Control,\
int Grap_Board_Num,int Grap_Colour,int Grap_start_angle,int Grap_end_angle,int Grap_width,int Grap_start_x,int Grap_start_y,int Grap_size,int Grap_end_x,int Grap_end_y);

void Judge_Word_Data_Set(Word_data_struct_t* Word_data_Set,uint8_t* Graphic_Name,Judge_Graphic_Type_t Judge_Graphic_Type,Judge_Graphic_Control_t Judge_Graphic_Control,\
int Grap_Board_Num,int Grap_Colour,int Grap_Word_Size,int Grap_Word_Lenth,int Grap_width,int Grap_start_x,int Grap_start_y,float Flost_32_Data,int32_t Int_32_Data);

void Judge_Character_Data_Set(Judge_client_custom_character_t* Judge_client_custom_character_Set,uint8_t* Graphic_Name,Judge_Graphic_Type_t Judge_Graphic_Type,Judge_Graphic_Control_t Judge_Graphic_Control,\
int Grap_Board_Num,int Grap_Colour,int Grap_Word_Size,int Grap_Word_Lenth,int Grap_width,int Grap_start_x,int Grap_start_y,uint8_t* Send_Character);

void Judge_Graphic_operate_tpye_Set(graphic_data_struct_t* graphic_data_struct_Update,uint8_t* Graphic_Name,Judge_Graphic_Control_t Judge_Graphic_Update);
void Judge_Word_operate_tpye_Set(Word_data_struct_t* Word_data_struct_Update,uint8_t* Graphic_Name,Judge_Graphic_Control_t Judge_Graphic_Update);
void Judge_Character_operate_tpye_Set(Judge_client_custom_character_t* Judge_client_custom_character_Update,uint8_t* Graphic_Name,Judge_Graphic_Control_t Judge_Graphic_Update);

void Judge_Delete_Data_Send(union Judge_Graphic_Delet_t *	Judge_client_custom_graphic_delete_Set,uint8_t graphic_Set,uint8_t graphic_Num);

void Judge_Data_Send(DJI_Judge_t* DJI_Judge_Send_Data,uint16_t Stu_data_cmd_id,uint16_t Stu_Get_id, uint8_t *p_data , int Data_long);
void Judge_Data_Send_To_Client(DJI_Judge_t* DJI_Judge_Send_Data,uint16_t Stu_data_cmd_id, uint8_t* p_data , int Data_long);

int Judge_Data_Update(uint8_t* Judge_Data_Get,DJI_Judge_t* DJI_Judge_Update);
void Judge_Data_check(uint8_t* Judge_Data);
DJI_Judge_Msg_t* Get_Judge_Msg(void);

#endif 
