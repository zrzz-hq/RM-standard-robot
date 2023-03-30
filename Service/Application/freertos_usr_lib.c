#include "freertos_usr_lib.h"

/*-----------------------�������йصĲ�������--------------------------*/
ListItem_t* Get_Next_ListItem(ListItem_t* ListItem)
{
		if(!ListItem)
			return NULL;
	
		//����������Ƿ�����һ������
		List_t* List = ListItem->pvContainer;
		if(!List)
				return NULL;
		//����Ƿ�Ϊ���һ��Ԫ��
		ListItem_t* Next_ListItem = listGET_NEXT(ListItem);
		if(Next_ListItem == List->pxIndex)
			return NULL;
		
		return Next_ListItem;
}

ListItem_t* Get_Prev_ListItem(ListItem_t* ListItem)
{
		if(!ListItem)
			return NULL;
		
		//����������Ƿ�����һ������
		List_t* List = ListItem->pvContainer;
		if(!List)
				return NULL;
		//����Ƿ�Ϊ��һ��Ԫ��
		ListItem_t* Prev_ListItem = ListItem->pxPrevious;
		if(Prev_ListItem == listGET_END_MARKER(List))
			return NULL;
		
		return Prev_ListItem;
}

ListItem_t* Get_First_ListItem(List_t* List)
{
		if(!List)
			return NULL;
		
		ListItem_t* ListItem = listGET_HEAD_ENTRY(List);
		if(ListItem == List->pxIndex)
			return NULL;
		
		return ListItem;
}

ListItem_t* Iterate_All_ListItem(List_t* List,void* Usr_Data1,void* Usr_Data2,int8_t(*CallBack)(ListItem_t*,void*,void*))
{
		ListItem_t* ListItem = Get_First_ListItem(List);
		while(ListItem)
		{
				int8_t Res = CallBack(ListItem,Usr_Data1,Usr_Data2);
				if(Res == 1)
					ListItem = Get_Next_ListItem(ListItem);
				else if(Res == -1)
					ListItem = Get_Prev_ListItem(ListItem);
				else if(Res == 0)
					break;
		}
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