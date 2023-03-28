#ifndef __GUI_TASK_H__
#define __GUI_TASK_H__
#include "stm32f4xx.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "list.h"

#include "lvgl.h"

#define GUI_TASK_PRIO 21
#define GUI_STK_SIZE 1024
extern TaskHandle_t GuiTask_Handler;

#define GUI_CONTROL_TASK_PRIO 21
#define GUI_CONTROL_STK_SIZE 256
extern TaskHandle_t GuiControlTask_Handler;

#define MAIN_PAGE_CONT_COUNT  8
#define LEN_OF_EDIT_TEXT 20
#define CHART_DATA_POINT_COUNT 120


typedef struct
{
		lv_group_t* gui_grp;
    lv_obj_t* gui_menu;
    lv_obj_t* edit_var_page;
		lv_obj_t* view_var_page;
    lv_obj_t* main_page;
		lv_obj_t* undo_cont;
		lv_obj_t* redo_cont;
		lv_theme_t * gui_theme;
		uint8_t is_gui_start;
}gui_t;

TaskHandle_t* create_gui_control_task();
void gui_start(void);
void gui_stop(void);
void gui_sys_assert_fault_handler(void);
#endif


