#ifndef USER_TASK_H
#define USER_TASK_H
#include "main.h"
#define USER_TASK_PRIO 5
#define USER_STK_SIZE 128



void UserTask(void *pvParameters);
	
#endif

void Create_User_task();