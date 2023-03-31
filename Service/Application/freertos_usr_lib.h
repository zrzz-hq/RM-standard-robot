#ifndef _RTOS_USR_LIB_
#define _RTOS_USR_LIB_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "list.h"
#include "task.h"
#include "queue.h"
#include "stream_buffer.h"
#include "message_buffer.h"
#include "semphr.h"

typedef struct
{
		uint16_t Msg_Len;
		void* Msg;
}Msg_t;

typedef struct
{
		List_t List;
		SemaphoreHandle_t List_Mutex;
}ListEx_t;

BaseType_t List_Init(ListEx_t* List);
ListItem_t* Iterate_All_ListItem(ListEx_t* List,void* Usr_Data1,void* Usr_Data2,int8_t(*CallBack)(ListItem_t*,void*,void*));
ListItem_t* Get_Next_ListItem(ListItem_t* ListItem,BaseType_t Timeout);
ListItem_t* Get_Prev_ListItem(ListItem_t* ListItem,BaseType_t Timeout);
ListItem_t* Get_First_ListItem(ListEx_t* List,BaseType_t Timeout);
BaseType_t List_Insert_End(ListEx_t* List,ListItem_t* ListItem,UBaseType_t Timeout);
BaseType_t List_Remove(ListEx_t* List,ListItem_t* ListItem,UBaseType_t Timeout);

QueueHandle_t Msg_Queue_Create(UBaseType_t Len);
BaseType_t Msg_Queue_Send(QueueHandle_t Handle,void* Data,uint16_t Data_Len,TickType_t Timeout);
BaseType_t Msg_Queue_Receive(QueueHandle_t Handle,void* Data,TickType_t Timeout);
uint16_t Msg_Queue_Get_Receive_Len(QueueHandle_t Handle,TickType_t Timeout);

void *pvPortRealloc(void *SrcAddr, size_t NewSize);
void *pvPortCalloc(size_t n, size_t size);


#endif