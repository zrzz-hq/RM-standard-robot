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
	//ͷ֡
  frame_header_t frame_header;
	//������
	uint16_t cmdid;
	//����
	uint8_t* Data;
	//֡βУ��
	uint16_t crc16;
} Judge_Get_Data_t;

typedef __packed struct
{
	//ͷ֡
  frame_header_t frame_header;
	//������
	uint16_t cmdid;
	//ѧ�����ڷ�������ID��������ID��������ID
	Judge_student_interactive_header_data_t Judge_student_interactive_header_data;	
	//����
//	Judge_student_data_t Judge_student_data;
	//֡βУ��
	uint16_t crc16;
} Judge_Send_Data_t;

//�������ݳ���
typedef enum
{
	Graphic_Delete_Long = 2,
	Graphic_Single_Long = 15,
	Graphic_double_Long = 30,
	Graphic_five_Long = 75,
	Graphic_seven_Long = 105,
	Graphic_character_Long = 45
}Judge_Graphic_Data_Long_t;


//ͼ�β���
typedef enum
{
	Graphic_None = 0,
	Graphic_Add = 1,
	Graphic_Change = 2,
	Graphic_Delete = 3
}Judge_Graphic_Control_t;

//ͼ������
typedef enum
{
	Straight_Line = 0,//ֱ��
	Rectangle = 1,//����
	All_Circle = 2,//��Բ
	Oval_Circle = 3,//��Բ
	Arc_Circle = 4,//Բ��
	Floating = 5,//������
	Integer = 6,//������
	Character = 7//�ַ�
}Judge_Graphic_Type_t;

//ͼ������
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


//ͼ������
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



//�ͻ���ɾ��ͼ�� �����˼�ͨ�ţ�0x0301
typedef __packed struct
{
	uint8_t operate_tpye;
	uint8_t layer;
}Judge_client_custom_graphic_delete_t;


//�ͻ��˻���һ��ͼ��
typedef __packed struct
{
 graphic_data_struct_t grapic_data_struct;
}Judge_client_custom_graphic_single_t;

//�ͻ��˻��ƶ���ͼ��
typedef __packed struct
{
graphic_data_struct_t grapic_data_struct[2];
}Judge_client_custom_graphic_double_t;

//�ͻ��˻������ͼ��
typedef __packed struct
{
graphic_data_struct_t grapic_data_struct[5];
}Judge_client_custom_graphic_five_t;

//�ͻ��˻����߸�ͼ��
typedef __packed struct
{
graphic_data_struct_t grapic_data_struct[7];
}Judge_client_custom_graphic_seven_t;

//�ͻ��˻����ַ� 
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


//�ͻ��˷�������
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
	0x0001	 11 		����״̬���ݣ�3Hz ���ڷ���
	0x0002	 1 		����������ݣ�������������
	0x0003	 28 	����������Ѫ�����ݣ�3Hz ���ڷ���
	0x0004	 3 		���ڷ���״̬�����ڷ���ʱ����
  0x0005   11   �˹�������ս���ӳ���ͷ�״̬��1Hz ���ڷ���
	0x0101	 4 		�����¼����ݣ�3Hz ���ڷ���
	0x0102	 3 		���ز���վ������ʶ���ݣ�������������
	0x0103	 2 		���󲹸�վ�������ݣ��ɲ����ӷ��ͣ����� 10Hz����RM �Կ�����δ���ţ�
	0x0104	 2 		���о������ݣ����淢������
	0x0105	 1 		���ڷ���ڵ���ʱ��3Hz ���ڷ���
	0x0201	 27 	������״̬���ݣ�10Hz ���ڷ���
	0x0202	 14 	ʵʱ�����������ݣ�50Hz ���ڷ���
	0x0203	 16 	������λ�����ݣ�10Hz ����
	0x0204	 1 		�������������ݣ�1Hz ���ڷ���
	0x0205	 3 		���л���������״̬���ݣ�10Hz ���ڷ��ͣ�ֻ�п��л��������ط���
	0x0206	 1 		�˺�״̬���ݣ��˺���������
	0x0207	 6 		ʵʱ������ݣ��ӵ��������
	0x0208	 2 		����ʣ�෢�����������л����ˣ��ڱ������˷��ͣ�10Hz ���ڷ���
	0x0209	 4 		������ RFID ״̬��3Hz ���ڷ���
	0x020A	 12 	���ڻ����˿ͻ���ָ�����ݣ�10Hz ���ڷ���
	0x0301 	 n 		�����˼佻�����ݣ����ͷ���������
	0x0302 	 n 		����������������ݽӿڣ�ͨ���ͻ��˴������ͣ����� 30Hz
	0x0303 	 15 		�ͻ���С��ͼ�������ݣ���������
	0x0304 	 12 		���̡������Ϣ��ͨ��ͼ�����ڷ���
	-----------------------------------------------------------------------------
******************************************************************************/

//������ID,�����жϽ��յ���ʲô����
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
����״̬����
CMD_ID:	0x0001
����Ƶ�ʣ�3Hz
���ͷ�Χ�����л�����
****/
typedef __packed struct
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
} ext_game_status_t;

/***
�����������
CMD_ID:	0x0002
����Ƶ�ʣ�������������
���ͷ�Χ�����л�����
****/
typedef __packed struct
{
	uint8_t winner;
} ext_game_result_t;

/***
������Ѫ������
CMD_ID:	0x0003
����Ƶ�ʣ�3Hz
���ͷ�Χ�����л����ˡ�
****/
typedef __packed struct
{
	uint16_t red_1_robot_HP;   // ��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
	uint16_t red_2_robot_HP;   // �� 2 ���̻�����Ѫ��
	uint16_t red_3_robot_HP;   // �� 3 ����������Ѫ��
	uint16_t red_4_robot_HP;   // �� 4 ����������Ѫ��
	uint16_t red_5_robot_HP;   // �� 5 ����������Ѫ��	
	uint16_t red_7_robot_HP;   // �� 7 �ڱ�������Ѫ��
	uint16_t red_outpost_HP;   // �췽ǰ��սѪ��
	uint16_t red_base_HP;      // �췽����Ѫ��
	uint16_t blue_1_robot_HP;  // �� 1 Ӣ�ۻ�����Ѫ��
	uint16_t blue_2_robot_HP;  // �� 2 ���̻�����Ѫ��
	uint16_t blue_3_robot_HP;  // �� 3 ����������Ѫ��
	uint16_t blue_4_robot_HP;  // �� 4 ����������Ѫ��
	uint16_t blue_5_robot_HP;  // �� 5 ����������Ѫ��
	uint16_t blue_7_robot_HP;  // �� 7 �ڱ�������Ѫ��
	uint16_t blue_outpost_HP;  // ����ǰ��վѪ��
	uint16_t blue_base_HP;     // ��������Ѫ��
} ext_game_robot_HP_t;

/**
 �˹�������ս���ӳ�\�ͷ����ֲ���Ǳ��ģʽ״̬��
 CMD_ID: 0x0005��
 ����Ƶ�ʣ�1Hz 
 ���ͷ�Χ�����л�����
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
�����¼�����
CMD_ID:	0x0101��
����Ƶ�ʣ�3Hz
���ͷ�Χ�����������ˡ�
****/
typedef __packed struct
{
	uint32_t event_type;
} ext_event_data_t;

/***
����վ������ʶ
CMD_ID:	0x0102
����Ƶ�ʣ�������������
���ͷ�Χ�����������ˡ�
****/
typedef __packed struct
{
	uint8_t supply_projectile_id; 
	uint8_t supply_robot_id; 
	uint8_t supply_projectile_step; 
	uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;
/***
���о�����Ϣ
CMD_ID:	0x0104
����Ƶ�ʣ����淢������
���ͷ�Χ�����������ˡ�
****/
typedef __packed struct
{
	uint8_t level;
	uint8_t foul_robot_id; 
} ext_referee_warning_t;

/***
���ڷ���ڵ���ʱ
CMD_ID:	0x0105
����Ƶ�ʣ�3Hz
���ͷ�Χ�����������ˡ�
****/
typedef __packed struct
{
	uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;

/***
����������״̬
CMD_ID:	0x0201
����Ƶ�ʣ�10Hz
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	uint8_t robot_id;
	uint8_t robot_level;     //�����˵ȼ�
	uint16_t remain_HP;      // ������ʣ��Ѫ��
	uint16_t max_HP;         // ����������Ѫ��
	uint16_t shooter_id1_17mm_cooling_rate;    // ������ 1 �� 17mm ǹ��ÿ����ȴֵ
	uint16_t shooter_id1_17mm_cooling_limit;   // ������ 1 �� 17mm ǹ����������
	uint16_t shooter_id1_17mm_speed_limit;    //������ 1 �� 17mm ǹ�������ٶ� ��λ m/s
	uint16_t shooter_id2_17mm_cooling_rate;   // ������ 2 �� 17mm ǹ��ÿ����ȴֵ	
	uint16_t shooter_id2_17mm_cooling_limit;  // ������ 2 �� 17mm ǹ����������
	uint16_t shooter_id2_17mm_speed_limit;    //������ 2 �� 17mm ǹ�������ٶ� ��λ m/s 
	uint16_t shooter_id1_42mm_cooling_rate;   // ������ 42mm ǹ��ÿ����ȴֵ
	uint16_t shooter_id1_42mm_cooling_limit;  // ������ 42mm ǹ����������
	uint16_t shooter_id1_42mm_speed_limit;    // ������ 42mm ǹ�������ٶ� ��λ m/s
	uint16_t chassis_power_limit;             // �����˵��̹�����������
	uint8_t mains_power_gimbal_output : 1; 
	uint8_t mains_power_chassis_output : 1;
	uint8_t mains_power_shooter_output : 1;
} ext_game_robot_status_t;


/***
ʵʱ������������
CMD_ID:	0x0202��
����Ƶ�ʣ�50Hz
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	uint16_t chassis_volt;   // ���������ѹ ��λ ����
	uint16_t chassis_current;  //����������� ��λ ����
	float chassis_power;       // ����������� ��λ W ��
	uint16_t chassis_power_buffer;  // ���̹��ʻ��� ��λ J ���� ��ע�����¸��ݹ��������� 250J
	uint16_t shooter_id1_17mm_cooling_heat;  // 1 �� 17mm ǹ������
	uint16_t shooter_id2_17mm_cooling_heat;  // 2 �� 17mm ǹ������
	uint16_t shooter_id1_42mm_cooling_heat;  // 42mm ǹ������
} ext_power_heat_data_t;

/***
������λ��
CMD_ID:	0x0203
����Ƶ�ʣ�10Hz
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	float x;
	float y;
	float z;
	float yaw;
} ext_game_robot_pos_t;


/***
����������
CMD_ID:	0x0204
����Ƶ�ʣ�1Hz
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	uint8_t power_rune_buff;
}ext_buff_t;


/***
���л���������״̬
CMD_ID:	0x0205
����Ƶ�ʣ�10Hz
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	uint8_t attack_time;
} aerial_robot_energy_t;


/***
�˺�״̬
CMD_ID:	0x0206
����Ƶ�ʣ��˺���������
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	uint8_t armor_id : 4;
	uint8_t hurt_type : 4;
} ext_robot_hurt_t;

/***
ʵʱ�����Ϣ
CMD_ID:	0x0207
����Ƶ�ʣ��������
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	uint8_t bullet_type;
	uint8_t shooter_id;
	uint8_t bullet_freq;
	float bullet_speed;
} ext_shoot_data_t;

/***
�ӵ�ʣ�෢����
CMD_ID:	0x0208
����Ƶ�ʣ����л����ˣ��ڱ������˷���
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;
} ext_bullet_remaining_t;
/***
������ RFID ״̬��
CMD_ID:	0x0209
����Ƶ�ʣ�1Hz
���ͷ�Χ����һ�����ˡ�
****/
typedef __packed struct
{
	uint32_t rfid_status;
} ext_rfid_status_t;

/***
���ڻ����˿ͻ���ָ������
CMD_ID:	0x020A
����Ƶ�ʣ�10Hz
���ͷ�Χ����һ�����ˡ�
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
	Judge_student_interactive_header_data_t Judge_student_interactive_header_data;
	uint8_t Judge_Student_Data_Get[113];	
}Judge_Student_Data_t;


/***
��������
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
