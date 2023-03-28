
#include "data_task.h"

#include "freertos_usr_lib.h"
#include "gui_task.h"
#include "Start_Task.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "easyflash.h"
#include "ff.h"

Data_t Data;
TaskHandle_t DataTask_Handler;

void Send_Msg_To_Gui(Data_Msg_Type_t Type,void* Usr_Data)
{
//		Data_Msg_t Data_Msg;
//		Data_Msg.Type = Type;
//		Data_Msg.Data = Usr_Data;
//		xQueueSend(Data.Data_Msg_Queue,&Data_Msg,portMAX_DELAY);
}

uint8_t Get_Variable_Size(Variable_t* Var)
{
		switch(Var->Var_Type)
		{
			case VAR_TYPE_UINT8:
			case VAR_TYPE_INT8:
				return 1;
			case VAR_TYPE_UINT16:
			case VAR_TYPE_INT16:
				return 2;
			case VAR_TYPE_UINT32:
			case VAR_TYPE_INT32:
			case VAR_TYPE_FP32:
				return 4;
			case VAR_TYPE_FP64:
				return 8;
			default:
				return 0;
		};
}

Variable_t* Get_Next_Variable(Variable_t* Var)
{
		ListItem_t* List_Item = Get_Next_ListItem(Var->List_Item);
		if(!List_Item)
			return NULL;
		return (Variable_t*)(List_Item->xItemValue);
}

Variable_t* Get_Prev_Variable(Variable_t* Var)
{
		ListItem_t* List_Item = Get_Prev_ListItem(Var->List_Item);
		if(!List_Item)
			return NULL;
		return (Variable_t*)(List_Item->xItemValue);
}

Variable_t* Get_First_Variable()
{
		ListItem_t* List_Item = Get_First_ListItem(&Data.Var_List);
		if(!List_Item)
			return NULL;
		return (Variable_t*)(List_Item->xItemValue);
}


static int8_t Iterate_All_Variable_CallBack(ListItem_t* List_Item,void* Usr_Data1,void* Usr_Data2)
{
		Variable_t* Var = (Variable_t*)List_Item->xItemValue;
		int8_t(*Usr_CallBack)(Variable_t*,void*) = Usr_Data1;
		return Usr_CallBack(Var,Usr_Data2);
}

Variable_t* Iterate_All_Variable(void* Usr_Data,int8_t(*CallBack)(Variable_t*,void* Usr_Data))
{
		ListItem_t* List_Item = Iterate_All_ListItem(&Data.Var_List,(void*)CallBack,Usr_Data,Iterate_All_Variable_CallBack);
		if(List_Item)
			return (Variable_t*)List_Item->xItemValue;
		return NULL;
}

Variable_t* Iterate_All_Variable_In_Task(void* Usr_Data,int8_t(*CallBack)(Variable_t*,void* Usr_Data))
{
		xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
		Variable_t* Var = Iterate_All_Variable(Usr_Data,CallBack);
		xSemaphoreGive(Data.Variable_Semphr);
		return Var;
}

static Data_Res_t Load_Store_Variable_Val(Variable_t* Var,uint8_t Load)
{
		Data_Res_t Res = DATA_SUCCESS;
		uint8_t Buff_Len = Get_Variable_Size(Var);
		if(Buff_Len == 0)
				Res = DATA_INVAILD_TYPE;
		else
		{
				uint8_t* Data_Buff = pvPortMalloc(Buff_Len);
				if(!Data_Buff)
						Res = DATA_MEM_ERROR;
				else
				{
						if(Load)
						{
								size_t Size_Saved;
								size_t Size_Read = ef_get_env_blob(Var->Var_Name,Data_Buff,Buff_Len,&Size_Saved);
								if(Size_Saved == Size_Read&&Size_Read==Buff_Len)
								{
										memcpy(Var->Var_Addr,Data_Buff,Buff_Len);
								}
								else
								{
										Res = Load_Store_Variable_Val(Var,0);
								}
						}
						else
						{
								memcpy(Data_Buff,Var->Var_Addr,Buff_Len);
								if(ef_set_env_blob(Var->Var_Name,Data_Buff,Buff_Len)!=EF_NO_ERR)
									Res = DATA_IO_ERROR;
						}
						vPortFree(Data_Buff);
						
				}
		}
		return Res;
}

static Data_Res_t Set_Variable_Val_By_Ptr(Variable_t* Var,double Val)
{
		
		if(!Var)
				return DATA_INVAILD_TYPE;
		if(Val == NAN)
				return DATA_NAN;

		switch(Var->Var_Type)
		{
			case VAR_TYPE_UINT8:
				*((uint8_t*)(Var->Var_Addr)) = (uint8_t)Val;
				break;
			case VAR_TYPE_UINT16:
				*((uint16_t*)(Var->Var_Addr)) = (uint16_t)Val;
				break;
			case VAR_TYPE_UINT32:
				*((uint32_t*)(Var->Var_Addr)) = (uint32_t)Val;
				break;
			case VAR_TYPE_INT8:
				*((int8_t*)(Var->Var_Addr)) = (int8_t)Val;
				break;
			case VAR_TYPE_INT16:
				*((int16_t*)(Var->Var_Addr)) = (int16_t)Val;
				break;
			case VAR_TYPE_INT32:
				*((int32_t*)(Var->Var_Addr)) = (int32_t)Val;
				break;
			case VAR_TYPE_FP32:
				*((fp32*)(Var->Var_Addr)) = (fp32)Val;
				break;
			case VAR_TYPE_FP64:
				*((fp64*)(Var->Var_Addr)) = (fp64)Val;
				break;
			default:
				return DATA_INVAILD_TYPE;
		}
				
		if(!Var->Read_Only)
				return Load_Store_Variable_Val(Var,0);
}

static double Get_Variable_Val_By_Ptr(Variable_t* Var)
{	
		if(!Var)
			return NAN;
		
		double Val;
		//根据数据类型将变量值转化为double值
		switch(Var->Var_Type)
		{
			case VAR_TYPE_UINT8:
				Val = (double)*((uint8_t*)(Var->Var_Addr));
				break;
			case VAR_TYPE_UINT16:
				Val = (double)*((uint16_t*)(Var->Var_Addr));
				break;
			case VAR_TYPE_UINT32:
				Val = (double)*((uint32_t*)(Var->Var_Addr));
				break;
			case VAR_TYPE_INT8:
				Val = (double)*((int8_t*)(Var->Var_Addr));
				break;
			case VAR_TYPE_INT16:
				Val = (double)*((int16_t*)(Var->Var_Addr));
				break;
			case VAR_TYPE_INT32:
				Val = (double)*((int32_t*)(Var->Var_Addr));
				break;
			case VAR_TYPE_FP32:
				Val = (double)*((fp32*)(Var->Var_Addr));
				break;
			case VAR_TYPE_FP64:
				Val = *((fp64*)(Var->Var_Addr));
				break;
			default:
				return NAN;
		};
		
		return Val;
}

static int8_t Find_Variable_By_Name_CallBack(Variable_t* Var,void* Usr_Data)
{
		char* Var_Name = Usr_Data;
		if(strcmp(Var_Name,Var->Var_Name)==0)
			return 0;
		return 1;
}

static Variable_t* Find_Variable_By_Name(char* Var_Name)
{
		if(!Var_Name)
				return NULL;
		Variable_t* Var = Iterate_All_Variable(Var_Name,Find_Variable_By_Name_CallBack);
		return Var;
}

Variable_t* Find_Variable_By_Name_In_Task(char* Var_Name)
{
		//获取信号量，每次仅限一个任务访问变量对象
		xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
		Find_Variable_By_Name(Var_Name);
		//释放信号量
		xSemaphoreGive(Data.Variable_Semphr);
}

static uint8_t Push_Modify_Stack(Modify_Item_t* Modify_Item)
{
		//如果栈还没有满，那么直接压栈
		if(Data.Modify_Stack.Sp < LENGTH_OF_MODIFY_STACK-1)
				Data.Modify_Stack.Sp++;
		//如果满了，丢弃一个项目腾出空间
		else
		{
				for(uint8_t i=1;i<LENGTH_OF_MODIFY_STACK-1;i++)
				{
						Data.Modify_Stack.Modify_Array[i] = Data.Modify_Stack.Modify_Array[i+1];
				}
		}
		Data.Modify_Stack.Modify_Array[Data.Modify_Stack.Sp] = *Modify_Item; 
		
		return Data.Modify_Stack.Sp;
}

static uint8_t Pop_Modify_Stack(Modify_Item_t* Modify_Item)
{
		//已经是栈底，返回
		if(Data.Modify_Stack.Sp == 0)
				return 0;
		//不是栈底，弹出
		*Modify_Item = Data.Modify_Stack.Modify_Array[Data.Modify_Stack.Sp];
		Data.Modify_Stack.Sp--;
		
		return Data.Modify_Stack.Sp;
}

static uint8_t Resume_Modify_Stack(Modify_Item_t* Modify_Item)
{
		if(Data.Modify_Stack.Sp < LENGTH_OF_MODIFY_STACK-1)
		{
				//堆栈指针直接移动
				Data.Modify_Stack.Sp++;
				*Modify_Item = Data.Modify_Stack.Modify_Array[Data.Modify_Stack.Sp];
		}
		return Data.Modify_Stack.Sp;
}

static void Can_Undo()
{
		uint8_t Res = Data.Modify_Stack.Sp > 0;
		//发送消息
		if(Res)
				Send_Msg_To_Gui(DATA_MSG_UNDO_ENABLE,NULL);
		else
				Send_Msg_To_Gui(DATA_MSG_UNDO_DISABLE,NULL);
		
		Data.Modify_Stack.Undo_Flag = Res;
		
}

static void Can_Redo()
{
		uint8_t Res = Data.Modify_Stack.Undo_Count > 0;
		//发送消息
		if(Res)
				Send_Msg_To_Gui(DATA_MSG_REDO_ENABLE,NULL);
		else
				Send_Msg_To_Gui(DATA_MSG_REDO_DISABLE,NULL);
		
		Data.Modify_Stack.Redo_Flag = Res;
}

Data_Res_t Undo_Modify_In_Task()
{
		xSemaphoreTake(Data.Modify_Stack.Stack_Semphr,portMAX_DELAY);
		Data_Res_t Res = DATA_UNDO_FAILED;
		//判断能否撤销
		Modify_Item_t Modify_Item;
		if(Data.Modify_Stack.Undo_Flag)
		{
				
				Pop_Modify_Stack(&Modify_Item);
			
				xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
				Set_Variable_Val_By_Ptr(Modify_Item.Var,Modify_Item.Val_Before);
				xSemaphoreGive(Data.Variable_Semphr);
			
				Data.Modify_Stack.Undo_Count++;
				Res = DATA_SUCCESS;

		}
		Can_Undo();
		Can_Redo();
		xSemaphoreGive(Data.Modify_Stack.Stack_Semphr);	
		return Res;
}

Data_Res_t Redo_Modify_In_Task()
{
		//获取信号量
		xSemaphoreTake(Data.Modify_Stack.Stack_Semphr,portMAX_DELAY);
		Data_Res_t Res = DATA_REDO_FAILED;
		//判断能否重做
		if(Data.Modify_Stack.Redo_Flag)
		{
				Modify_Item_t Modify_Item;
				Resume_Modify_Stack(&Modify_Item);
			
				xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
				Set_Variable_Val_By_Ptr(Modify_Item.Var,Modify_Item.Val_After);
				xSemaphoreGive(Data.Variable_Semphr);
			
				Data.Modify_Stack.Undo_Count--;	
				Res = DATA_SUCCESS;
		}
		Can_Redo();
		Can_Undo();
		//释放信号量
		xSemaphoreGive(Data.Modify_Stack.Stack_Semphr);
		return Res;
}

static void Add_Modify(Variable_t* Var,double Old_Val,double New_Val)
{
		Modify_Item_t Modify_Item;
		Modify_Item.Var = Var;
		Modify_Item.Val_Before = Old_Val;
		Modify_Item.Val_After = New_Val;
		
		xSemaphoreTake(Data.Modify_Stack.Stack_Semphr,portMAX_DELAY);
		Push_Modify_Stack(&Modify_Item);
		Data.Modify_Stack.Undo_Count = 0;
		Can_Undo();
		Can_Redo();
		xSemaphoreGive(Data.Modify_Stack.Stack_Semphr);
		//发送消息
		Send_Msg_To_Gui(DATA_MSG_VAR_MODIFY,&Data.Modify_Stack.Modify_Array[Data.Modify_Stack.Sp]);
}

static Data_Res_t Modify_Variable_Val_By_Ptr(Variable_t* Var,double New_Val)
{
		Data_Res_t Res = DATA_SUCCESS;
		double Val = Get_Variable_Val_By_Ptr(Var);
		Res = Set_Variable_Val_By_Ptr(Var,New_Val);
		//修改成功，压栈支持回退
		if(Res == DATA_SUCCESS)
		{
				Add_Modify(Var,Val,New_Val);
		}
		return Res;
}

Data_Res_t Set_Variable_Val_By_Ptr_In_Task(Variable_t* Var,double New_Val)
{
		Data_Res_t Res = DATA_SUCCESS;
		//获取信号量，每次仅限一个任务访问变量对象
		xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
	
		Res = Modify_Variable_Val_By_Ptr(Var,New_Val);
		//释放信号量
		xSemaphoreGive(Data.Variable_Semphr);
		return Res;
}

Data_Res_t Set_Variable_Val_By_Name_In_Task(char* Var_Name,double New_Val)
{
		//获取信号量，每次仅限一个任务访问变量对象
		xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
		//根据名字查找变量
		Variable_t* Var = Find_Variable_By_Name(Var_Name);
		Data_Res_t Res = Modify_Variable_Val_By_Ptr(Var,New_Val);
		//释放信号量
		xSemaphoreGive(Data.Variable_Semphr);
		return Res;
}

double Get_Variable_Val_By_Ptr_In_Task(Variable_t* Var)
{
		xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
		double Val = Get_Variable_Val_By_Ptr(Var);
		xSemaphoreGive(Data.Variable_Semphr);
		return Val;
}

double Get_Variable_Val_By_Name_In_Task(char* Var_Name)
{	
		xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
		Variable_t* Var = Find_Variable_By_Name(Var_Name); 
		Data_Res_t Res = Get_Variable_Val_By_Ptr(Var);
		xSemaphoreGive(Data.Variable_Semphr);
}

Data_Res_t Add_Variable_By_Name_In_Task(char* Var_Name,void* Var_Addr,Variable_Type_t Var_Type,bool_t Read_Only)
{
		Data_Res_t Res = DATA_SUCCESS;
		//获取信号量，每次仅限一个任务访问变量
		xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
		if(!Find_Variable_By_Name(Var_Name))
		{
				//申请链表节点内存
				ListItem_t* New_List_Item= pvPortMalloc(sizeof(ListItem_t));
				//申请失败，返回错误
				if(!New_List_Item)
						Res = DATA_MEM_ERROR;
				else
				{
					
						//初始化链表节点
						vListInitialiseItem(New_List_Item);
						//申请新变量结构体内存
						Variable_t* New_Var = pvPortMalloc(sizeof(Variable_t));
						//申请失败，返回错误
						if(!New_Var)
						{
								vPortFree(New_List_Item);
								Res = DATA_MEM_ERROR;
						}
						else
						{
								//配置新变量结构体
								New_Var->List_Item = New_List_Item;
								New_List_Item->xItemValue = (uint32_t)New_Var;
								
								New_Var->Var_Name = Var_Name;
								New_Var->Var_Addr = Var_Addr;
								New_Var->Var_Type = Var_Type;
								New_Var->Read_Only = Read_Only;
								//插入到链表中
								vListInsertEnd(&Data.Var_List,New_List_Item);
								//如果不是只读变量，且要求刷新的话，进行刷新
								if(!Read_Only)
								{
										Load_Store_Variable_Val(New_Var,1);
								}
								else
										Data.ReadOnly_Count++;
						}
				}
		}
		else
			Res = DATA_INVAILD_NAME;
		//释放信号量
		xSemaphoreGive(Data.Variable_Semphr);
		return Res;
}

static int8_t Refresh_DataFile_CallBack(Variable_t* Var,void* Usr_Data)
{
		uint16_t* Store_Count = Usr_Data;
		if(Load_Store_Variable_Val(Var,0)==DATA_SUCCESS)
			(*Store_Count)++;
		return 1;
}

Data_Res_t Refresh_DataFile_In_Task()
{
		if(ef_env_set_default()!=EF_NO_ERR)
				return DATA_IO_ERROR;
		uint16_t Store_Count = 0;
		Iterate_All_Variable_In_Task(&Store_Count,Refresh_DataFile_CallBack);
		return DATA_SUCCESS;
}

Data_Res_t Trace_Variable_By_Ptr_In_Task(Variable_t* Var,uint16_t Max_Time,void* CallBack)
{
		if(Var)
				return DATA_INVAILD_TYPE;
		
		//获取信号量，每次仅限一个任务访问变量
		//xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
		Data_Res_t Res = DATA_SUCCESS;

		Variable_Trace_t* Trace = pvPortMalloc(sizeof(Variable_Trace_t));
		if(!Trace)
		{
				Res = DATA_MEM_ERROR;
				goto Error;
		}

		uint8_t Size = Get_Variable_Size(Var);
		Trace->SnapShort = pvPortMalloc(Size);
		if(!Trace->SnapShort)
		{
				Res = DATA_MEM_ERROR;
				goto Error;
		}

		Trace->List_Item = pvPortMalloc(sizeof(ListItem_t));
		if(!Trace->List_Item)
		{
				Res = DATA_MEM_ERROR;
				goto Error;
		}
		
		Trace->List_Item->xItemValue = (uint32_t)Trace;
		vListInsertEnd(&Data.Trace_List,Trace->List_Item);
		Trace->Var = Var;
		Trace->CallBack = CallBack;
		Trace->Size = Size;
		Trace->Max_No_Change_Time = Max_Time;
		//拍摄快照
		taskENTER_CRITICAL();
		memcpy(Trace->SnapShort,Var->Var_Addr,Size);
		taskEXIT_CRITICAL(); 
		goto End;
			
	Error:
		if(Trace->List_Item)
			vPortFree(Trace->List_Item);
		if(Trace->SnapShort)
			vPortFree(Trace->SnapShort);
		if(Trace)
			vPortFree(Trace);
			
	End:	
		//释放信号量
		//xSemaphoreGive(Data.Variable_Semphr);
		return Res;
			
}

static int8_t Iterate_All_Trace_CallBack(ListItem_t* List_Item,void* Usr_Data1,void* Usr_Data2)
{
		Variable_Trace_t* Trace = (Variable_Trace_t*)List_Item->xItemValue;
	
		//进入临界区，操作期间不允许数据修改
		taskENTER_CRITICAL();
		//比较变量当前值与快照有无差别，如果没有差别，计数器加一，否则，计数器清零
		if(memcmp(Trace->SnapShort,Trace->Var->Var_Addr,Trace->Size)==0)
			Trace->No_Change_Time++;
		else
			Trace->No_Change_Time=0;
		//创建新快照
		memcpy(Trace->SnapShort,Trace->Var->Var_Addr,Trace->Size);
		//退出临界区
		taskEXIT_CRITICAL(); 
		
		//变量很长时间没有发生改变，那么调用回调函数
		if(Trace->No_Change_Time>Trace->Max_No_Change_Time)
			Trace->CallBack(Trace->Var);
		return 1;
}

#define Iterate_All_Trace() Iterate_All_ListItem(&Data.Trace_List,NULL,NULL,Iterate_All_Trace_CallBack);


Data_Res_t Save_Data_To_SDCard(void)
{
		FATFS Fs;
		FRESULT Res;
		Res = f_mount(&Fs,"",1);
		if(Res!=FR_OK)
			return DATA_IO_ERROR;
		FIL File;
		Res = f_open(&File,DATA_FILE_PATH,DATA_FILE_WRITE_FLAG);
		if(Res!=FR_OK)
			return DATA_IO_ERROR;
		
		cJSON* JSON_Obj = pvPortMalloc(sizeof(cJSON));
		if(!JSON_Obj)
		{
			f_close(&File);
			return DATA_MEM_ERROR;
		}
		
		Variable_t* Var_Node = Get_First_Variable();
		xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
		while(Var_Node)
		{
				cJSON* JSON_Node = cJSON_CreateNumber(Get_Variable_Val_By_Ptr(Var_Node));
				cJSON_AddItemToObject(JSON_Obj,Var_Node->Var_Name,JSON_Node);
				Var_Node = Get_Next_Variable(Var_Node);
		}
		xSemaphoreGive(Data.Variable_Semphr);
			
		
		char* Buff = cJSON_Print(JSON_Obj);
		if(!Buff)
		{
				cJSON_Delete(JSON_Obj);
				f_close(&File);
				return DATA_CJSON_ERROR;
		}
		
		UINT Btw = strlen(Buff);
		UINT Bw;
		Res = f_write(&File,Buff,Btw,&Bw);
		
		cJSON_Delete(JSON_Obj);
		vPortFree(Buff);
		f_close(&File);

		if(Res!=FR_OK)
			return DATA_IO_ERROR;
		return DATA_SUCCESS;
}

Data_Res_t Load_Data_From_SDCard(void)
{
		FATFS Fs;
		FRESULT Res;
		Res = f_mount(&Fs,"",1);
		if(Res!=FR_OK)
			return DATA_IO_ERROR;
		FIL File;
		Res = f_open(&File,DATA_FILE_PATH,DATA_FILE_READ_FLAG);
		if(Res!=FR_OK)
			return DATA_IO_ERROR;
		FSIZE_t Size = f_size(&File);
		char* Buff = pvPortMalloc(Size);
		if(!Buff)
		{
			f_close(&File);
			return DATA_MEM_ERROR;
		}
		UINT Br;
		Res = f_read(&File,Buff,Size,&Br);
		if(Res!=FR_OK)
		{
			f_close(&File);
			vPortFree(Buff);
			return DATA_IO_ERROR;
		}
		
		f_close(&File);
		
		cJSON* JSON_Obj = cJSON_Parse(Buff);
		if(!JSON_Obj)
		{
				vPortFree(Buff);
				return DATA_CJSON_ERROR;
		}
		if(JSON_Obj->child)
		{
				xSemaphoreTake(Data.Variable_Semphr,portMAX_DELAY);
				cJSON* JSON_Node = JSON_Obj->child;
				do
				{
						Variable_t* Var = Find_Variable_By_Name(JSON_Node->string);
						if(Var)
							Set_Variable_Val_By_Ptr(Var,JSON_Node->valuedouble);
						JSON_Node = JSON_Node->next;
				}
				while(JSON_Node); 
				xSemaphoreGive(Data.Variable_Semphr);
		}
		cJSON_Delete(JSON_Obj);
		return DATA_SUCCESS;
		
}

void Data_Init()
{
		if(easyflash_init() == EF_NO_ERR)
			Data.Data_Is_Ready = 1;
		else
			Data.Data_Is_Ready = 0;
		Data.Variable_Semphr = xSemaphoreCreateBinary();
		Data.Modify_Stack.Stack_Semphr = xSemaphoreCreateBinary();
		xSemaphoreGive(Data.Variable_Semphr);
		xSemaphoreGive(Data.Modify_Stack.Stack_Semphr);
		
		Data.Data_Msg_Queue = xQueueCreate(DATA_MSG_QUEUE_LEN,sizeof(Data_Msg_t));
		
		xSemaphoreTake(Data.Modify_Stack.Stack_Semphr,portMAX_DELAY);
		Can_Redo();
		Can_Undo();
		xSemaphoreGive(Data.Modify_Stack.Stack_Semphr);
		
		vListInitialise(&Data.Var_List);
		vListInitialise(&Data.Trace_List);
		
		cJSON_Hooks Hooks;
		Hooks.free_fn = vPortFree;
		Hooks.malloc_fn = pvPortMalloc;
		cJSON_InitHooks(&Hooks);
}

void Data_Task(void *pvParameters)
{
		Data_Init();
		create_gui_control_task();
		Create_Start_Task();
		while(1)
		{
				Iterate_All_Trace();
				vTaskDelay(100);
		}
}

TaskHandle_t* Create_Data_Task(void)
{
    xTaskCreate((TaskFunction_t)Data_Task,          //任务函数
                (const char *)"Data_Task",          //任务名称
                (uint16_t)DATA_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                       //传递给任务函数的参数
                (UBaseType_t)DATA_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&DataTask_Handler); //任务句柄
		return &DataTask_Handler;						
}

void Data_Usr_Key_Handler()
{
		if(DataTask_Handler)
			xTaskResumeFromISR(DataTask_Handler);
}