#ifndef __LVGL_USER_LIB__
#define __LVGL_USER_LIB__

#include "src/extra/widgets/menu/lv_menu.h"
#define MAIN_PAGE_CONT_COUNT 7

#ifdef __cplusplus
extern "C" {
#endif

#define LEN_OF_EDIT_TEXT 20
#define CHART_DATA_POINT_COUNT 120

typedef enum
{
	 KB_MAP_UINT,
	 KB_MAP_INT,
	 KB_MAP_FP
}edit_kb_map_t;

typedef struct
{
    void* usr_data;
    void* callback;
    uint8_t is_float;
    lv_obj_t* data_label;
    lv_obj_t* var_chart;
    lv_obj_t* period_label;
    uint8_t y_max_too_large_count;
    uint8_t y_min_too_small_count;
    uint8_t y_mid_error_count;
    uint8_t timer_count;
}lv_timer_usr_data_t;

typedef enum
{
	EVENT_USR_DATA_TYPE_RAW,
	EVENT_USR_DATA_TYPE_CALLBACK,
	EVENT_USR_DATA_TYPE_CMD,
}lv_menu_usr_data_type_t;

typedef struct
{
		uint32_t data; 
		void* parser;
}lv_menu_usr_data_t;

lv_obj_t* lv_win_create_wrapper(const char* title, lv_coord_t hor_res, lv_coord_t ver_res,uint8_t close_btn);
lv_obj_t* lv_msgbox_create_wrapper(const char* title,const char*btn_map[], const char* msg, void* cb);
lv_obj_t* lv_edit_win_create(void* usr_data, double val,void* cb, edit_kb_map_t key_map);
lv_obj_t* lv_graph_win_create(void* usr_data,void* cb,uint8_t is_float);
lv_obj_t* lv_menu_base_cont_create(lv_obj_t* pg, const char* text);
lv_obj_t* lv_menu_link_cont_create(lv_obj_t*pg,const char* text,lv_obj_t* child);
lv_obj_t* lv_menu_btn_cont_create(lv_obj_t* pg, const char* text, uint32_t data,void* parser);
char* lv_menu_cont_get_text(lv_obj_t* cont);
lv_obj_t* lv_menu_create_wrapper(lv_obj_t* parent);
lv_obj_t* lv_menu_page_create_wrapper(lv_obj_t* menu,char* title);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // !__LVGL_USER_LIB__

