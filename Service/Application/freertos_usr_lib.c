#include "freertos_usr_lib.h"

/*-----------------------与链表有关的操作函数--------------------------*/
#define List_Lock(List,Timeout,Ret) if(xSemaphoreTake((List)->List_Mutex,(Timeout))==pdFALSE) return (Ret);
#define List_Unlock(List) xSemaphoreGive((List)->List_Mutex)
#define Is_Last_ListItem(List,ListItem)  (ListItem) == ((List_t*)List)->pxIndex
#define Is_First_ListItem(List,ListItem) (ListItem) == listGET_END_MARKER((List_t*)List)
#define listGET_PREV(ListItem) (ListItem)->pxPrevious

BaseType_t List_Init(ListEx_t* List)
{
		vListInitialise(&List->List);
		List->List_Mutex = xSemaphoreCreateBinary();
		if(!List->List_Mutex)
			return pdFALSE;
		
		List_Unlock(List);
		return pdTRUE;
}

BaseType_t List_Insert_End(ListEx_t* List,ListItem_t* ListItem,UBaseType_t Timeout)
{
		configASSERT(List);
		configASSERT(ListItem);
		
		List_Lock(List,Timeout,pdFALSE);
		
		vListInsertEnd(&List->List,ListItem);
		xSemaphoreGive(List->List_Mutex);
		
		List_Unlock(List);
		return pdTRUE;
}

BaseType_t List_Remove(ListEx_t* List,ListItem_t* ListItem,UBaseType_t Timeout)
{
		configASSERT(List);
		configASSERT(ListItem);
		if(ListItem->pvContainer!=&List->List)
			return pdFALSE;
		
		List_Lock(List,Timeout,pdFALSE);
		
		BaseType_t Res = uxListRemove(ListItem);

		List_Unlock(List);
		return Res;
}

ListItem_t* Get_Next_ListItem(ListItem_t* ListItem,BaseType_t Timeout)
{
		configASSERT(ListItem);
		
		//检查链表项是否属于一个链表
		List_t* List = ListItem->pvContainer;
		configASSERT(List);
		
		List_Lock((ListEx_t*)List,Timeout,NULL);
		
		//检查是否为最后一个元素
		ListItem_t* Next_ListItem = listGET_NEXT(ListItem);
		if(Next_ListItem == List->pxIndex)
				Next_ListItem = NULL;
		
		List_Unlock((ListEx_t*)List);
		
		return Next_ListItem;
}

ListItem_t* Get_Prev_ListItem(ListItem_t* ListItem,BaseType_t Timeout)
{
		configASSERT(ListItem);
		
		//检查链表项是否属于一个链表
		List_t* List = ListItem->pvContainer;
		configASSERT(List);
	
		List_Lock((ListEx_t*)List,Timeout,NULL);
	
		//检查是否为第一个元素
		ListItem_t* Prev_ListItem = listGET_PREV(ListItem);
		if(Is_First_ListItem(List,ListItem))
				Prev_ListItem = NULL;
		
		List_Unlock((ListEx_t*)List);
		
		return Prev_ListItem;
}

ListItem_t* Get_First_ListItem(ListEx_t* List,BaseType_t Timeout)
{
		configASSERT(List);
		
		List_Lock(List,Timeout,NULL);

		ListItem_t* ListItem = listGET_HEAD_ENTRY((List_t*)List);
		if(Is_Last_ListItem(List,ListItem))
			ListItem = NULL;
		
		List_Unlock(List);
		
		return ListItem;
}

ListItem_t* Iterate_All_ListItem(ListEx_t* List,void* Usr_Data1,void* Usr_Data2,int8_t(*CallBack)(ListItem_t*,void*,void*))
{
		configASSERT(List);
	
		List_Lock((ListEx_t*)List,portMAX_DELAY,NULL);
	
		ListItem_t* ListItem = listGET_HEAD_ENTRY((List_t*)List);
		if(Is_Last_ListItem(List,ListItem))
				ListItem = NULL;
		
		while(ListItem)
		{
				int8_t Res = CallBack(ListItem,Usr_Data1,Usr_Data2);
				if(Res == 1)
				{
					ListItem = listGET_NEXT(ListItem);
					if(Is_Last_ListItem(List,ListItem))
						ListItem = NULL;
				}
				else if(Res == -1)
				{
					ListItem = listGET_PREV(ListItem);
					if(Is_First_ListItem(List,ListItem))
						ListItem = NULL;
				}
				else if(Res == 0)
					break;
		}
		
		List_Unlock((ListEx_t*)List);
		return ListItem;
}

QueueHandle_t Msg_Queue_Create(UBaseType_t Len)
{
		return xQueueCreate(Len,sizeof(Msg_t));
}

BaseType_t Msg_Queue_Send(QueueHandle_t Handle,void* Data,uint16_t Data_Len,TickType_t Timeout)
{
		Msg_t Msg;
		Msg.Msg_Len = Data_Len;
		Msg.Msg = pvPortMalloc(Data_Len);
		memcpy(Msg.Msg,Data,Data_Len);
		return xQueueSend(Handle,&Msg,Timeout);
}

uint16_t Msg_Queue_Get_Receive_Len(QueueHandle_t Handle,TickType_t Timeout)
{
		Msg_t Msg;
		if(xQueuePeek(Handle,&Msg,Timeout)==pdTRUE)
			return Msg.Msg_Len;
		return 0;
}

BaseType_t Msg_Queue_Receive(QueueHandle_t Handle,void* Data,TickType_t Timeout)
{
		Msg_t Msg;
		if(xQueueReceive(Handle,&Msg,Timeout)==pdTRUE)
		{
				memcpy(Data,Msg.Msg,Msg.Msg_Len);
				vPortFree(Msg.Msg);
				return pdTRUE;
		}
		return pdFALSE;
}