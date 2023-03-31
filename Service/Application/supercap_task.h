#ifndef _SUPERCAP_TASK_H_
#define _SUPERCAP_TASK_H_

#include "main.h"
#include "Judge_Data.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#define CAP_CHARGE_POWER_LIMIT 150
#define CAP_DISCHARGE_POWER_LIMIT 300
#define DEFAULT_CHASSIS_POWER_LIMIT 60
#define DEFAULT_CHASSIS_POWER_BUFFER 60

#define SUPERCAP_TASK_PRIO 25
#define SUPERCAP_STK_SIZE 512
extern TaskHandle_t SuperCapTask_Handler;

typedef enum
{
		SUPCAP_MODE_NORMAL,
		SUPCAP_MODE_DEBUG,
		SUPCAP_MODE_NONE
}supercap_mode_t;

typedef struct
{
    uint16_t chassis_power_buffer;  //底盘功率缓冲，0~60J
    uint16_t chassis_power_limit;   //机器人底盘功率限制上限，0~120W
    int16_t output_power_limit;     //电容放电功率限制，-120~300W
    uint16_t input_power_limit;     //电容充电功率限制，0~150W
    uint16_t chassis_volt;			//底盘输出电压 单位 毫伏 **
		uint16_t chassis_current;		//底盘输出电流 单位 毫安 **
    union{
        uint16_t all;
        struct
        {
            uint16_t cap_switch : 1;    //电容开关
            uint16_t cap_record : 1;    //oled记录功能开关
            uint16_t gamegoing : 1;     //比赛进行中为1，否则为0 ***
        }bit;
    }cap_control;
}cap_send_data_t;

typedef struct
{
    float cap_v;    //电容两端电压Ucr，0~30V
    float cap_i;    //电容电流I，-20~20A
    union
    {
        uint16_t state;     //电容状态
        struct
        {
            uint16_t warning : 1;   //报警
            uint16_t cap_v_over : 1;    //电容过压
            uint16_t cap_i_over : 1;    //电容过流
            uint16_t cap_v_low : 1;     //电容欠压
            uint16_t bat_i_low : 1;     //裁判系统欠压
            uint16_t can_receive_miss : 1;    //电容未接收到CAN通信数据
        }bit;
    }cap_state;
}cap_receive_data_t;

typedef struct 
{
		supercap_mode_t supercap_mode;
		cap_send_data_t cap_send_data;
		cap_receive_data_t cap_receive_data;
		Judge_Info_t* dji_judge_info;
		Super_C_Msg_t* super_c_msg;
		uint8_t robot_id;
		uint8_t last_robot_id;
		uint8_t supercap_on;
		uint8_t game_going;
		//在接入裁判系统的情况下，使能调试模式将不使用裁判系统的数据，如果没有接入裁判系统，该位无效
		uint8_t debug_mode; 
		uint16_t chassis_power_limit;
		uint16_t chassis_power_buffer;
}supercap_t;

void supercap_task(void *pvParameters);
void supercap_data_parser(uint8_t* data);
void create_supercap_task();

#endif

