#ifndef __JUDGE_DATA_H__
#define __JUDGE_DATA_H__

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "freertos_usr_lib.h"

#define DJI_JUDGE_BUF_MAX_LEN 200
#define DJI_JUDGE_HEADER_SOF 0xA5

#define JUDGE_HEADER_LEN 5
#define JUDGE_CMDID_LEN 2
#define JUDGE_CRC16_LEN 2
#define JUDGE_DATALEN_LEN 2

#define JUDGE_STUDENT_DATA_MAX_LEN 113
#define JUDGE_STUDENT_DATA_HEADER_LEN 6

/*******************************************************************************
	--------------------------------Cmd_ID----------------------------------------
	0x0001	 11 		比赛状态数据，3Hz 周期发送
	0x0002	 1 		比赛结果数据，比赛结束后发送
	0x0003	 28 	比赛机器人血量数据，3Hz 周期发送
	0x0004	 3 		飞镖发射状态，飞镖发射时发送
  0x0005   11   人工智能挑战赛加成与惩罚状态，1Hz 周期发送
	0x0101	 4 		场地事件数据，3Hz 周期发送
	0x0102	 3 		场地补给站动作标识数据，动作发生后发送
	0x0103	 2 		请求补给站补弹数据，由参赛队发送，上限 10Hz。（RM 对抗赛尚未开放）
	0x0104	 2 		裁判警告数据，警告发生后发送
	0x0105	 1 		飞镖发射口倒计时，3Hz 周期发送
	0x0201	 27 	机器人状态数据，10Hz 周期发送
	0x0202	 14 	实时功率热量数据，50Hz 周期发送
	0x0203	 16 	机器人位置数据，10Hz 发送
	0x0204	 1 		机器人增益数据，1Hz 周期发送
	0x0205	 3 		空中机器人能量状态数据，10Hz 周期发送，只有空中机器人主控发送
	0x0206	 1 		伤害状态数据，伤害发生后发送
	0x0207	 6 		实时射击数据，子弹发射后发送
	0x0208	 2 		弹丸剩余发射数，仅空中机器人，哨兵机器人发送，10Hz 周期发送
	0x0209	 4 		机器人 RFID 状态，3Hz 周期发送
	0x020A	 12 	飞镖机器人客户端指令数据，10Hz 周期发送
	0x0301 	 n 		机器人间交互数据，发送方触发发送
	0x0302 	 n 		定义控制器交互数据接口，通过客户端触发发送，上限 30Hz
	0x0303 	 15 		客户端小地图交互数据，触发发送
	0x0304 	 12 		键盘、鼠标信息，通过图传串口发送
	-----------------------------------------------------------------------------
******************************************************************************/

typedef enum
{
		Target_Client,
		Target_Hero,
		Target_Engineer,
		Target_Standard1,
		Target_Standard2,
		Target_Standard3,
		Target_Aerial,
		Target_Sentry,
		Target_Radar,
}Judge_Student_Data_Target_t;

//命令码ID,用来判断接收的是什么数据
typedef enum
{ 
	Judge_CmdID_Game_State = 0x0001,
	Judge_CmdID_Game_Result = 0x0002,
	Judge_CmdID_Game_Robot_HP = 0x0003,
	Judge_CmdID_Dart_Status = 0x0004,
	Judge_CmdID_ICRA_Status = 0x0005,
	Judge_CmdID_Event = 0x0101,
	Judge_CmdID_Supply_Projectile_Action = 0x0102,
	Judge_CmdID_Referee_Warning = 0x0104,
	Judge_CmdID_Dart_Remaining_Time = 0x0105,
	Judge_CmdID_Game_Robot_Status = 0x0201,
	Judge_CmdID_Power_Heat_Data = 0x0202,
	Judge_CmdID_Game_Robot_Pos = 0x0203,
	Judge_CmdID_Buff = 0x0204,
	Judge_CmdID_Aerial_Robot_Energy = 0x0205,
	Judge_CmdID_Robot_Hurt = 0x0206,
	Judge_CmdID_Shoot_Data = 0x0207,
	Judge_CmdID_Bullet_Remaining = 0x0208,
	Judge_CmdID_RFID_Status = 0x0209,
	Judge_CmdID_Dart_Client_Cmd = 0x020A,
	Judge_CmdID_Robot_Interactive_Data = 0x0301
	
}Judge_CmdID_t;

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
}Judge_Len_t;

/***
比赛状态数据
CMD_ID:	0x0001
发送频率：3Hz
发送范围：所有机器人
****/
typedef __packed struct
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
} ext_game_status_t;

/***
比赛结果数据
CMD_ID:	0x0002
发送频率：比赛结束后发送
发送范围：所有机器人
****/
typedef __packed struct
{
	uint8_t winner;
} ext_game_result_t;

/***
机器人血量数据
CMD_ID:	0x0003
发送频率：3Hz
发送范围：所有机器人。
****/
typedef __packed struct
{
	uint16_t red_1_robot_HP;   // 红1英雄机器人血量，未上场以及罚下血量为 0
	uint16_t red_2_robot_HP;   // 红 2 工程机器人血量
	uint16_t red_3_robot_HP;   // 红 3 步兵机器人血量
	uint16_t red_4_robot_HP;   // 红 4 步兵机器人血量
	uint16_t red_5_robot_HP;   // 红 5 步兵机器人血量	
	uint16_t red_7_robot_HP;   // 红 7 哨兵机器人血量
	uint16_t red_outpost_HP;   // 红方前哨战血量
	uint16_t red_base_HP;      // 红方基地血量
	uint16_t blue_1_robot_HP;  // 蓝 1 英雄机器人血量
	uint16_t blue_2_robot_HP;  // 蓝 2 工程机器人血量
	uint16_t blue_3_robot_HP;  // 蓝 3 步兵机器人血量
	uint16_t blue_4_robot_HP;  // 蓝 4 步兵机器人血量
	uint16_t blue_5_robot_HP;  // 蓝 5 步兵机器人血量
	uint16_t blue_7_robot_HP;  // 蓝 7 哨兵机器人血量
	uint16_t blue_outpost_HP;  // 蓝方前哨站血量
	uint16_t blue_base_HP;     // 蓝方基地血量
} ext_game_robot_HP_t;

/**
 人工智能挑战赛加成\惩罚区分布与潜伏模式状态：
 CMD_ID: 0x0005。
 发送频率：1Hz 
 发送范围：所有机器人
***/
typedef __packed struct
{
	uint8_t F1_zone_status:1;
	uint8_t F1_zone_buff_debuff_status:3; 
	uint8_t F2_zone_status:1;
	uint8_t F2_zone_buff_debuff_status:3; 
	uint8_t F3_zone_status:1;
	uint8_t F3_zone_buff_debuff_status:3; 
	uint8_t F4_zone_status:1;
	uint8_t F4_zone_buff_debuff_status:3; 
	uint8_t F5_zone_status:1;
	uint8_t F5_zone_buff_debuff_status:3; 
	uint8_t F6_zone_status:1;
	uint8_t F6_zone_buff_debuff_status:3;
	uint16_t red1_bullet_left;
	uint16_t red2_bullet_left;
	uint16_t blue1_bullet_left;
	uint16_t blue2_bullet_left;
	uint8_t lurk_mode;
	uint8_t res;
} ext_ICRA_buff_debuff_zone_and_lurk_status_t;
/***
场地事件数据
CMD_ID:	0x0101。
发送频率：3Hz
发送范围：己方机器人。
****/
typedef __packed struct
{
	uint32_t event_type;
} ext_event_data_t;

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
} ext_supply_projectile_action_t;
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
} ext_referee_warning_t;

/***
飞镖发射口倒计时
CMD_ID:	0x0105
发送频率：3Hz
发送范围：己方机器人。
****/
typedef __packed struct
{
	uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;

/***
比赛机器人状态
CMD_ID:	0x0201
发送频率：10Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
	uint8_t robot_id;
	uint8_t robot_level;     //机器人等级
	uint16_t remain_HP;      // 机器人剩余血量
	uint16_t max_HP;         // 机器人上限血量
	uint16_t shooter_id1_17mm_cooling_rate;    // 机器人 1 号 17mm 枪口每秒冷却值
	uint16_t shooter_id1_17mm_cooling_limit;   // 机器人 1 号 17mm 枪口热量上限
	uint16_t shooter_id1_17mm_speed_limit;    //机器人 1 号 17mm 枪口上限速度 单位 m/s
	uint16_t shooter_id2_17mm_cooling_rate;   // 机器人 2 号 17mm 枪口每秒冷却值	
	uint16_t shooter_id2_17mm_cooling_limit;  // 机器人 2 号 17mm 枪口热量上限
	uint16_t shooter_id2_17mm_speed_limit;    //机器人 2 号 17mm 枪口上限速度 单位 m/s 
	uint16_t shooter_id1_42mm_cooling_rate;   // 机器人 42mm 枪口每秒冷却值
	uint16_t shooter_id1_42mm_cooling_limit;  // 机器人 42mm 枪口热量上限
	uint16_t shooter_id1_42mm_speed_limit;    // 机器人 42mm 枪口上限速度 单位 m/s
	uint16_t chassis_power_limit;             // 机器人底盘功率限制上限
	uint8_t mains_power_gimbal_output : 1; 
	uint8_t mains_power_chassis_output : 1;
	uint8_t mains_power_shooter_output : 1;
} ext_game_robot_status_t;


/***
实时功率热量数据
CMD_ID:	0x0202。
发送频率：50Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
	uint16_t chassis_volt;   // 底盘输出电压 单位 毫伏
	uint16_t chassis_current;  //底盘输出电流 单位 毫安
	float chassis_power;       // 底盘输出功率 单位 W 瓦
	uint16_t chassis_power_buffer;  // 底盘功率缓冲 单位 J 焦耳 备注：飞坡根据规则增加至 250J
	uint16_t shooter_id1_17mm_cooling_heat;  // 1 号 17mm 枪口热量
	uint16_t shooter_id2_17mm_cooling_heat;  // 2 号 17mm 枪口热量
	uint16_t shooter_id1_42mm_cooling_heat;  // 42mm 枪口热量
} ext_power_heat_data_t;

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
} ext_game_robot_pos_t;


/***
机器人增益
CMD_ID:	0x0204
发送频率：1Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
	uint8_t power_rune_buff;
}ext_buff_t;


/***
空中机器人能量状态
CMD_ID:	0x0205
发送频率：10Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
	uint8_t attack_time;
} aerial_robot_energy_t;


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
} ext_robot_hurt_t;

/***
实时射击信息
CMD_ID:	0x0207
发送频率：射击后发送
发送范围：单一机器人。
****/
typedef __packed struct
{
	uint8_t bullet_type;
	uint8_t shooter_id;
	uint8_t bullet_freq;
	float bullet_speed;
} ext_shoot_data_t;

/***
子弹剩余发射数
CMD_ID:	0x0208
发送频率：空中机器人，哨兵机器人发送
发送范围：单一机器人。
****/
typedef __packed struct
{
	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;
} ext_bullet_remaining_t;
/***
机器人 RFID 状态：
CMD_ID:	0x0209
发送频率：1Hz
发送范围：单一机器人。
****/
typedef __packed struct
{
	uint32_t rfid_status;
} ext_rfid_status_t;

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
	uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;



typedef __packed struct
{
	uint16_t data_cmd_id;
	uint16_t sender_ID;
	uint16_t receiver_ID;
}Judge_Student_Data_Header_t;

typedef __packed struct
{
	Judge_Student_Data_Header_t Judge_Student_Data_Header;
	uint8_t Data[JUDGE_STUDENT_DATA_MAX_LEN];
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
	ext_game_status_t Judge_game_status;
	ext_game_result_t Judge_game_result;
	ext_game_robot_HP_t Judge_game_robot_HP;
	ext_event_data_t Judge_event_data;
	ext_supply_projectile_action_t Judge_supply_projectile_action;
	ext_referee_warning_t Judge_referee_warning;
	ext_dart_remaining_time_t Judge_dart_remaining_time;
	ext_game_robot_status_t Judge_game_robot_status;
	ext_power_heat_data_t Judge_power_heat_data;
	ext_game_robot_pos_t Judge_game_robot_pos;
	ext_buff_t Judge_buff;
	aerial_robot_energy_t Judge_aerial_robot_energy;
	ext_robot_hurt_t Judge_robot_hurt;
	ext_shoot_data_t Judge_shoot_data;
	ext_bullet_remaining_t Judge_bullet_remaining;
	ext_rfid_status_t Judge_RFID_status;
	ext_dart_client_cmd_t Judge_dart_client_cmd;
	Judge_Student_Data_t Judge_Student_Data;
} Judge_Info_t;

typedef __packed struct
{
  //帧头
	uint8_t  sof;
	//数据长度
  uint16_t data_length;
	//包序号
  uint8_t  seq;
	//帧头校验
  uint8_t  crc8;
	//命令码，也算作帧头一部分
	uint16_t cmd_id;
} Judge_Frame_Header_t;

typedef struct
{
	uint8_t Judge_Usart_Rx_Buff[2][DJI_JUDGE_BUF_MAX_LEN];
	uint8_t Judge_Usart_Tx_Buff[DJI_JUDGE_BUF_MAX_LEN];
	Judge_Info_t DJI_Judge_Info;
	uint8_t Judge_Data_Send_Count;
	uint8_t Judge_Data_Receive_Count;
	SemaphoreHandle_t Judge_Data_Send_Mutex;
	SemaphoreHandle_t Judge_Data_Receive_Mutex;
	TaskHandle_t Judge_Data_Task_Handle;
	uint8_t Judge_Usart_Current_Rx_Buff;
}Judge_Data_t;

void Judge_Usart_Init(void);
void Judge_Data_Init(void);
Judge_Info_t* Get_Judge_Info(void);
void Judge_Student_Data_Send(uint8_t* Stuent_Data,uint16_t Student_Data_Len,uint16_t Student_CmdID,Judge_Student_Data_Target_t Data_Target);

#endif 
