#ifndef __DATA_TASK_H__
#define __DATA_TASK_H__

#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#include "list.h"
#include "semphr.h"
#include "task.h"
#include "event_groups.h"

//#include "ff.h"
#include "sdio.h"
#include "cjson.h"

#define DATA_AUTO_REFRESH 1
#define DATA_AUTO_SAVE		0

#define DATA_FILE_DRIVE ""
#define DATA_FILE_PATH "DATA"
#define DATA_FILE_WRITE_FLAG FA_READ|FA_WRITE|FA_CREATE_ALWAYS
#define DATA_FILE_READ_FLAG	 FA_READ
#define DEFAULT_JSON "{}"

#define LENGTH_OF_MODIFY_STACK 21

#define JSON_SEMPHR_WAIT_TIME 1000

#define SD_CARD_DETECT_KEY  GPIO_ReadInputDataBit(SD_DETECT_GPIO_PORT,SD_DETECT_PIN)

#define DATA_TASK_PRIO 9
#define DATA_STK_SIZE 512

#define DATA_MSG_QUEUE_LEN 20

#define DATA_EVENT_INIT_DONE 0x01

#define Is_Last_Variable(Var) Var->List_Item->pxNext == Data.Var_List.pxIndex
#define Is_First_Variable(Var) listGET_HEAD_ENTRY(&Data.Var_List)==Var->List_Item	
#define Get_Variable_Count()   Data.Var_List.uxNumberOfItems

typedef enum
{
		DATA_SUCCESS,  	//完全成功
		DATA_MEM_ERROR,			//内存分配错误
		DATA_IO_ERROR,			//IO错误
		DATA_CJSON_ERROR,
		DATA_INVAILD_TYPE,		//非法的变量数据类型
		DATA_INVAILD_NAME,		//非法的变量名称
		DATA_NAN,							//执行过程中出现NAN
		DATA_UNDO_FAILED,			//无法撤销
		DATA_REDO_FAILED			//无法重做
}Data_Res_t;

typedef enum
{
		VAR_TYPE_UINT8,
		VAR_TYPE_UINT16,
		VAR_TYPE_UINT32,
		VAR_TYPE_INT8,
		VAR_TYPE_INT16,
		VAR_TYPE_INT32,
		VAR_TYPE_FP32,
		VAR_TYPE_FP64,
}Variable_Type_t;

typedef struct
{
		ListItem_t* List_Item;
		char* Var_Name;
		void* Var_Addr;
		Variable_Type_t Var_Type;
		bool_t Read_Only;
}Variable_t;

typedef enum
{
		OPT_END,
		OPT_NEXT,
		OPT_PREV,
		OPT_NONE
}Variable_Iterate_Opt_t;

typedef struct
{
		Variable_t* Var;
		double Val_Before;
		double Val_After;
}Modify_Item_t;

typedef struct
{
		Modify_Item_t Modify_Array[LENGTH_OF_MODIFY_STACK];
		uint8_t Sp;
		uint8_t Undo_Count;
		SemaphoreHandle_t Stack_Semphr;
		uint8_t Undo_Flag;
		uint8_t Redo_Flag;
}Modify_Stack_t;

typedef enum
{
		DATA_MSG_VAR_ADD,
		DATA_MSG_VAR_MODIFY,
		DATA_MSG_UNDO_DISABLE,
		DATA_MSG_UNDO_ENABLE,
		DATA_MSG_REDO_DISABLE,
		DATA_MSG_REDO_ENABLE,
		DATA_MSG_GUI_START,
		DATA_MSG_GUI_STOP,
}Data_Msg_Type_t;

typedef __packed struct
{
		Data_Msg_Type_t Type;
		char* Desc;
		void* Data;
}Data_Msg_t;

typedef struct
{
		List_t Var_List;
		uint8_t ReadOnly_Count;
	
		Modify_Stack_t Modify_Stack;
		SemaphoreHandle_t Variable_Semphr;
		
		uint8_t Data_Is_Ready;
		
		QueueHandle_t Data_Msg_Queue;
}Data_t;

extern TaskHandle_t DataTask_Handler;
extern Data_t Data;


TaskHandle_t* Create_Data_Task(void);


Data_Res_t Save_Variables_To_File(void);
Data_Res_t Load_Variables_From_File(void);
Data_Res_t Create_Variable_DataBase(void);


QueueHandle_t Get_Msg_Queue(void);
double Get_Variable_Val_By_Name_In_Task(char* Var_Name);
double Get_Variable_Val_By_Ptr_In_Task(Variable_t* Var);

Data_Res_t Set_Variable_Val_By_Name_In_Task(char* Var_Name,double New_Val);
Data_Res_t Set_Variable_Val_By_Ptr_In_Task(Variable_t* Var,double New_Val);

Data_Res_t Add_Variable_By_Name_In_Task(char* Var_Name,void* Var_Addr,Variable_Type_t Var_Type,bool_t Read_Only);
Data_Res_t Add_Variable_By_Ptr_In_Task(Variable_t* Var,void* Var_Addr,Variable_Type_t Var_Type,bool_t Read_Only);

Data_Res_t Undo_Modify_In_Task(void);
Data_Res_t Redo_Modify_In_Task(void);
Data_Res_t Init_DataFile(void);
Data_Res_t Save_Data_To_SDCard(void);
Data_Res_t Load_Data_From_SDCard(void);

Variable_t* Get_First_Variable();
Variable_t* Get_Prev_Variable(Variable_t* Var);
Variable_t* Get_Next_Variable(Variable_t* Var);

void Data_Usr_Key_Handler(void);

#endif 
