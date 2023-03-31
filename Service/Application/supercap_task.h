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
    uint16_t chassis_power_buffer;  //���̹��ʻ��壬0~60J
    uint16_t chassis_power_limit;   //�����˵��̹����������ޣ�0~120W
    int16_t output_power_limit;     //���ݷŵ繦�����ƣ�-120~300W
    uint16_t input_power_limit;     //���ݳ�繦�����ƣ�0~150W
    uint16_t chassis_volt;			//���������ѹ ��λ ���� **
		uint16_t chassis_current;		//����������� ��λ ���� **
    union{
        uint16_t all;
        struct
        {
            uint16_t cap_switch : 1;    //���ݿ���
            uint16_t cap_record : 1;    //oled��¼���ܿ���
            uint16_t gamegoing : 1;     //����������Ϊ1������Ϊ0 ***
        }bit;
    }cap_control;
}cap_send_data_t;

typedef struct
{
    float cap_v;    //�������˵�ѹUcr��0~30V
    float cap_i;    //���ݵ���I��-20~20A
    union
    {
        uint16_t state;     //����״̬
        struct
        {
            uint16_t warning : 1;   //����
            uint16_t cap_v_over : 1;    //���ݹ�ѹ
            uint16_t cap_i_over : 1;    //���ݹ���
            uint16_t cap_v_low : 1;     //����Ƿѹ
            uint16_t bat_i_low : 1;     //����ϵͳǷѹ
            uint16_t can_receive_miss : 1;    //����δ���յ�CANͨ������
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
		//�ڽ������ϵͳ������£�ʹ�ܵ���ģʽ����ʹ�ò���ϵͳ�����ݣ����û�н������ϵͳ����λ��Ч
		uint8_t debug_mode; 
		uint16_t chassis_power_limit;
		uint16_t chassis_power_buffer;
}supercap_t;

void supercap_task(void *pvParameters);
void supercap_data_parser(uint8_t* data);
void create_supercap_task();

#endif

