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

typedef __packed struct
{
		uint16_t Msg_Len;
		void* Msg;
}Msg_t;

ListItem_t* Iterate_All_ListItem(List_t* List,void* Usr_Data1,void* Usr_Data2,int8_t(*CallBack)(ListItem_t*,void*,void*));
ListItem_t* Get_First_ListItem(List_t* List);
ListItem_t* Get_Prev_ListItem(ListItem_t* ListItem);
ListItem_t* Get_Next_ListItem(ListItem_t* ListItem);

QueueHandle_t Msg_Queue_Create(UBaseType_t Len);
BaseType_t Msg_Queue_Send(QueueHandle_t Handle,void* Data,uint16_t Data_Len,TickType_t Timeout);
BaseType_t Msg_Queue_Receive(QueueHandle_t Handle,void* Data,TickType_t Timeout);
uint16_t Msg_Queue_Get_Receive_Len(QueueHandle_t Handle,TickType_t Timeout);

void *pvPortRealloc(void *SrcAddr, size_t NewSize);
void *pvPortCalloc(size_t n, size_t size);


#endif