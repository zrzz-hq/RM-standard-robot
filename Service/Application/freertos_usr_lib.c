#include "freertos_usr_lib.h"

/*-----------------------与链表有关的操作函数--------------------------*/
ListItem_t* Get_Next_ListItem(ListItem_t* ListItem)
{
		if(!ListItem)
			return NULL;
	
		//检查链表项是否属于一个链表
		List_t* List = ListItem->pvContainer;
		if(!List)
				return NULL;
		//检查是否为最后一个元素
		ListItem_t* Next_ListItem = listGET_NEXT(ListItem);
		if(Next_ListItem == List->pxIndex)
			return NULL;
		
		return Next_ListItem;
}

ListItem_t* Get_Prev_ListItem(ListItem_t* ListItem)
{
		if(!ListItem)
			return NULL;
		
		//检查链表项是否属于一个链表
		List_t* List = ListItem->pvContainer;
		if(!List)
				return NULL;
		//检查是否为第一个元素
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