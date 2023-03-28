
#include "oled.h"

#include "gui_task.h"
#include "data_task.h"

#include "Judge_Data.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lvgl_usr_lib.h"

gui_t gui;
TaskHandle_t GuiTask_Handler;

static void sys_reset_wrapper(lv_obj_t* cont);
static char* create_msg_by_res(char* msg_head, Data_Res_t res);
static void data_operation_handler(lv_obj_t* cont);
static void var_operation_handler(lv_obj_t* cont);
																															
const char* msg_tails[2]={"successfully","failed"};
																																											 
static void sys_reset_wrapper(lv_obj_t* cont)
{
		NVIC_SystemReset();
}
																													 
static char* create_msg_by_res(char* msg_head, Data_Res_t res)
{
    const char* msg_tail;
    if (res == DATA_SUCCESS)
        msg_tail = msg_tails[0];
    else
        msg_tail = msg_tails[1];
    uint32_t head_len = strlen(msg_head);
    uint32_t tail_len = strlen(msg_tail);
    char* msg = (char*)lv_mem_alloc(strlen(msg_head) + strlen(msg_tail) + 2);
    if (msg)
    {
        sprintf(msg, "%s %s", msg_head, msg_tail);
    }
    return msg;
}				

static void data_operation_msgbox_exec(lv_obj_t* msgbox)
{
		lv_obj_t* cont = lv_obj_get_user_data(msgbox);
		Data_Res_t(*data_operation)() = (Data_Res_t(*)())lv_obj_get_user_data(cont);
		Data_Res_t res = data_operation();
		char* msg_head = lv_menu_base_cont_get_text(cont);
		char* msg = create_msg_by_res(msg_head,res);
		lv_base_msgbox_set_msg_text(msgbox,msg);
		lv_mem_free(msg);
}

static void set_variable_val_msgbox_exec(lv_obj_t* msgbox)
{
		lv_obj_t* edit_win = lv_obj_get_user_data(msgbox);
		Variable_t* var = (Variable_t*)lv_obj_get_user_data(edit_win);
		double val = lv_edit_win_get_val(edit_win);
		Data_Res_t res = Set_Variable_Val_By_Ptr_In_Task(var,val);
		char* msg = create_msg_by_res("Modify Variable",res);
		lv_base_msgbox_set_msg_text(msgbox,msg);
		lv_mem_free(msg);
}

static uint8_t set_variable_val_wrapper(lv_obj_t* edit_win)
{
		lv_obj_t* msgbox = lv_exec_msgbox_create(lv_scr_act(),"Message",NULL,set_variable_val_msgbox_exec,edit_win,NULL);
		lv_exec_msgbox_run(msgbox,0);
}

static double get_variable_val_wrapper(lv_obj_t* graph_win)
{
		return Get_Variable_Val_By_Ptr_In_Task(lv_obj_get_user_data(graph_win));
}

static void data_operation_handler(lv_obj_t* cont)
{
		lv_obj_t* msgbox = lv_exec_msgbox_create(lv_scr_act(),"Message",NULL,data_operation_msgbox_exec,(void*)cont,NULL);
		lv_exec_msgbox_run(msgbox,0);
}

static void var_operation_handler(lv_obj_t* cont)
{
		Variable_t* var = (Variable_t*)lv_obj_get_user_data(cont);
		if(var->Read_Only)
		{
					lv_graph_win_create(lv_scr_act(),var,get_variable_val_wrapper,120,var->Var_Type==VAR_TYPE_FP32||var->Var_Type==VAR_TYPE_FP64);
		}
		else
		{
					edit_keyboard_map_t map;
					switch(var->Var_Type)
					{
						case VAR_TYPE_UINT8:
						case VAR_TYPE_UINT16:
						case VAR_TYPE_UINT32:
							map = EDIT_KB_MAP_UINT;
							break;
						case VAR_TYPE_INT8:
						case VAR_TYPE_INT16:
						case VAR_TYPE_INT32:
							map = EDIT_KB_MAP_INT;
							break;
						case VAR_TYPE_FP32:
						case VAR_TYPE_FP64:
							map = EDIT_KB_MAP_FP;
							break;
					}
					double val = Get_Variable_Val_By_Ptr_In_Task(var);
					lv_edit_win_create(lv_scr_act(),val,var,set_variable_val_wrapper,map);
		}
		
}

void gui_task_init()
{
		//lvgl初始化及显示屏、按键初始化
		lv_init();
		lv_port_disp_init();
		lv_port_indev_init();
	
		//初始化界面主题
		gui.gui_theme = lv_theme_mono_init(NULL,1,&lv_font_unscii_8);
    lv_disp_set_theme(0, gui.gui_theme);
	
		//初始化其他数据结构
		gui.gui_grp = lv_group_create();
		lv_group_set_default(gui.gui_grp);
    lv_indev_t* dev = lv_indev_get_next(NULL);
		lv_indev_set_group(dev, gui.gui_grp);
	
		//gui.data_msg_queue=Get_Msg_Queue();
		gui.is_gui_start = 0;
}

void add_all_variable_item()
{
		//uint16_t var_count = Get_Variable_Count();
		Variable_t* var = Get_First_Variable();
		while(var)
		{
				lv_obj_t* parent;
				if(var->Read_Only)
						parent = gui.view_var_page;
				else
						parent = gui.edit_var_page;
				lv_menu_btn_cont_create(parent,var->Var_Name,var,var_operation_handler);
				var = Get_Next_Variable(var);
		}
}

void create_main_menu()
{
		gui.gui_menu = lv_menu_create(lv_scr_act());
		lv_obj_set_size(gui.gui_menu,LV_PCT(100),LV_PCT(100));
		//lv_obj_add_event_cb(gui.gui_menu,lv_menu_event_cb,LV_EVENT_KEY,gui.gui_menu);
	
		lv_obj_t* back_btn = lv_menu_get_main_header_back_btn(gui.gui_menu);
    lv_obj_t* back_btn_lb = lv_label_create(back_btn);
    lv_label_set_text(back_btn_lb, "<");

		gui.edit_var_page = lv_menu_page_create(gui.gui_menu,"Edit");
		gui.view_var_page = lv_menu_page_create(gui.gui_menu,"View");
		
		gui.main_page = lv_menu_page_create(gui.gui_menu,NULL);
		gui.undo_cont = lv_menu_btn_cont_create(gui.main_page,"Undo Modify",Undo_Modify_In_Task,data_operation_handler);
		gui.redo_cont = lv_menu_btn_cont_create(gui.main_page,"Redo Modify",Redo_Modify_In_Task,data_operation_handler);
		lv_menu_link_cont_create(gui.main_page,"Edit Variable",gui.edit_var_page);
		lv_menu_link_cont_create(gui.main_page,"View Variable",gui.view_var_page);
		lv_menu_btn_cont_create(gui.main_page,"Load Data From Sdcard",Load_Data_From_SDCard,data_operation_handler);
		lv_menu_btn_cont_create(gui.main_page,"Save Data To Sdcard",Save_Data_To_SDCard,data_operation_handler);
		lv_menu_btn_cont_create(gui.main_page,"Init DataBase",Init_DataFile,data_operation_handler);
		lv_menu_btn_cont_create(gui.main_page,"Reset",0,sys_reset_wrapper);
		
		/*-------------------------------------------*/
		lv_menu_set_page(gui.gui_menu,gui.main_page);
		add_all_variable_item();
}

void del_main_menu()
{
		lv_obj_del(gui.gui_menu);
		gui.gui_menu = NULL;
		gui.main_page = NULL;
		gui.view_var_page = NULL;
		gui.edit_var_page = NULL;
		gui.redo_cont = NULL;
		gui.undo_cont = NULL;
}

void gui_start()
{
		//绘制启动图标，打开显示
		oled_init();
		//创建主菜单
		create_main_menu();
		//恢复任务
		gui.is_gui_start = 1;
		vTaskResume(GuiTask_Handler);
}

void gui_stop()
{
		//挂起任务
		vTaskSuspend(GuiTask_Handler);
		//删除主菜单，释放资源
		del_main_menu();
		//关闭oled显示
		oled_clear(Pen_Clear);
		oled_refresh_gram();
		oled_display_off();
		//清零标志位
		gui.is_gui_start = 0;
}

uint8_t is_gui_start()
{
		return gui.is_gui_start;
}

void gui_sys_assert_fault_handler()
{
		if(lv_is_initialized())
			lv_base_msgbox_create(lv_scr_act(),"Error","System Halt Please Reset",NULL,NULL,sys_reset_wrapper);
		else
			sys_reset_wrapper(NULL);
}

void gui_task(void *pvParameters)
{
		gui_task_init();
		vTaskSuspend(NULL);
		while(1)
		{
				lv_task_handler();
				vTaskDelay(25);
		}
}

TaskHandle_t* create_gui_task()
{
		xTaskCreate((TaskFunction_t)gui_task,          //任务函数
						(const char *)"Data_Task",          //任务名称
						(uint16_t)GUI_STK_SIZE,            //任务堆栈大小
						(void *)NULL,                        //传递给任务函数的参数
						(UBaseType_t)GUI_TASK_PRIO,        //任务优先级
						(TaskHandle_t *)&GuiTask_Handler); //任务句柄
		return &GuiTask_Handler;
}