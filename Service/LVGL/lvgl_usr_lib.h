#ifndef __LVGL_USER_LIB__
#define __LVGL_USER_LIB__

#include "lvgl.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
		LV_THREAD_RUNNING = 1,
		LV_THREAD_READY,
		LV_THREAD_SUSPENDED,
		LV_THREAD_DELETED,
		LV_THREAD_FINISHED,
}lv_thread_state_t;

typedef struct
{
		lv_obj_t obj;
		void* first;
		void* next;
		void* prev;
		uint16_t item_count;
}lv_iterator_t;

typedef struct
{
		lv_obj_t obj;
		void* thread_handle;
		void* run;
		uint8_t auto_run;
		uint8_t res;
}lv_thread_t;

typedef struct
{
    lv_obj_t cont;
    lv_obj_t* label;
    lv_obj_t* tip_label;
}lv_menu_base_cont_t;

typedef struct
{
    lv_menu_base_cont_t cont;
    lv_obj_t* page;
}lv_menu_link_cont_t;

typedef struct
{
    lv_menu_base_cont_t cont;
    void* user_data;
    void* user_data_parser;
}lv_menu_btn_cont_t;

typedef struct
{
    lv_win_t win;
    lv_group_t* win_group;
    lv_group_t* old_group;
    lv_obj_t* close_btn;
    uint8_t has_close_btn;
    lv_style_t* win_style;
}lv_base_win_t;

typedef struct
{
    lv_base_win_t msgbox;
    lv_obj_t* btn_matrix;
    lv_obj_t* msg_label;
		lv_obj_t* btn_cont;
    void* cb;
}lv_base_msgbox_t;

typedef struct
{
	lv_base_msgbox_t msgbox;
	void* exec;
	lv_obj_t* thread;
	
}lv_exec_msgbox_t;

typedef struct
{
    lv_base_win_t win;
    lv_obj_t* graph;
    lv_obj_t* val_label;
    lv_obj_t* period_label;
    lv_obj_t* inc_btn;
    lv_obj_t* dec_btn;
    lv_timer_t* refresh_timer;
    void* cb;
    uint8_t is_float;
    uint8_t y_max_too_large_count;
    uint8_t y_min_too_small_count;
    uint8_t y_mid_error_count;
    uint8_t timer_count;
    uint8_t scale_factor;
    uint8_t scale_factor_to_small_count;
    uint16_t data_count;
    uint16_t current_period;
}lv_graph_win_t;

typedef struct
{
    lv_base_win_t win;
    lv_obj_t* edit_textarea;
    lv_obj_t* edit_keyboard;
    void* cb;
    double val;
}lv_edit_win_t;

typedef struct
{
		lv_base_win_t win;
}lv_prog_win_t;

typedef enum
{
    EDIT_KB_MAP_UINT,
    EDIT_KB_MAP_INT,
    EDIT_KB_MAP_FP,
}edit_keyboard_map_t;


char* lv_menu_base_cont_get_text(lv_obj_t* cont);
void lv_menu_base_cont_set_text(lv_obj_t* obj,const char* text);
lv_obj_t* lv_menu_base_cont_create(lv_obj_t* parent);
lv_obj_t* lv_menu_link_cont_create(lv_obj_t* parent, const char* text, lv_obj_t* page);
lv_obj_t* lv_menu_btn_cont_create(lv_obj_t* parent, const char* text, void* user_data, void* user_data_parser);
lv_obj_t* lv_graph_win_create(lv_obj_t* parent, void* user_data, void* cb, uint8_t data_count, uint8_t is_float);
double lv_edit_win_get_val(lv_obj_t* win);
lv_obj_t* lv_edit_win_create(lv_obj_t* parent, double val, void* user_data, void* cb, edit_keyboard_map_t key_map);
lv_obj_t* lv_base_msgbox_create(lv_obj_t* parent, const char* title, const char* msg_text, const char* btn_map[], void* user_data, void* cb);
void lv_base_msgbox_set_msg_text(lv_obj_t* msgbox, const char* msg_text);
lv_obj_t* lv_exec_msgbox_create(lv_obj_t* parent, const char* title, const char* btn_map[],void* exec, void* user_data, void* cb);
void lv_exec_msgbox_run(lv_obj_t* obj,uint8_t cancel_able);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // !__LVGL_USER_LIB__

