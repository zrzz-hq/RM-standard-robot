#ifndef _RTOS_USR_LIB_
#define _RTOS_USR_LIB_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "list.h"
#include "task.h"

ListItem_t* Iterate_All_ListItem(List_t* List,void* Usr_Data1,void* Usr_Data2,int8_t(*CallBack)(ListItem_t*,void*,void*));
ListItem_t* Get_First_ListItem(List_t* List);
ListItem_t* Get_Prev_ListItem(ListItem_t* ListItem);
ListItem_t* Get_Next_ListItem(ListItem_t* ListItem);

#endif