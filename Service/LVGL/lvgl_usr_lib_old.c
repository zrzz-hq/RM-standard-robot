#include "lvgl_usr_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lvgl.h"

static uint8_t get_digits_count(double val);
static lv_coord_t lv_chart_adjust_y_range(double val,lv_timer_usr_data_t* usr_data,uint8_t is_float);
static void lv_indev_set_keypad_encoder_group(lv_group_t* grp);
static void lv_win_del_wrapper(lv_obj_t* win);
static void lv_win_event_cb(lv_event_t* e);
static void lv_msgbox_event_cb(lv_event_t* e);
static void lv_edit_win_event_cb(lv_event_t* e);
static void lv_period_btn_event_cb(lv_event_t* e);
static void lv_chart_timer_cb(lv_timer_t* timer);
static void lv_graph_win_event_cb(lv_event_t* e);
static void lv_menu_event_cb(lv_event_t* e);
static void lv_menu_page_event_cb(lv_event_t* e);
static lv_menu_usr_data_t* lv_menu_usr_data_create(lv_obj_t* pg,uint32_t data,void* parser);
static void lv_menu_usr_data_remove(lv_obj_t* pg,lv_menu_usr_data_t* menu_usr_data);
static void lv_menu_usr_data_clear(lv_obj_t* pg);
static void lv_menu_set_page_wrapper(lv_obj_t* menu, lv_obj_t* cont, uint32_t data);


static const char* edit_kb_fp_map[] = { "OK", "0" , "1", "2", "3", "<-", "\n",
																			 "+/-", "." , "4", "5", "6", "7", "8", "9","" };

static const lv_btnmatrix_ctrl_t edit_kb_fp_ctrl_map[] = {LV_KEYBOARD_CTRL_BTN_FLAGS |3, 1, 1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS | 3,
																																											3, 1, 1, 1, 1, 1, 1, 1 };

static const char* edit_kb_uint_map[] = { "OK", "0", "1", "<-", "\n",
																				   "2", "3", "4", "5", "6", "7", "8", "9",""};

static const lv_btnmatrix_ctrl_t edit_kb_uint_ctrl_map[] = {LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS | 3,
																																												 1, 1, 1,	1, 1, 1, 1, 1 };

static const char* edit_kb_int_map[] = { "OK", "0", "1", "2", "3", "<-", "\n",
																				"+/-", "4", "5", "6", "7", "8", "9", "" };

static const lv_btnmatrix_ctrl_t edit_kb_int_ctrl_map[] = {LV_KEYBOARD_CTRL_BTN_FLAGS |3, 1, 1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS | 3,
																																											 3, 1, 1, 1, 1, 1, 1 };


static void lv_indev_set_keypad_encoder_group(lv_group_t* grp)
{
    lv_indev_t* indev = lv_indev_get_next(NULL);
    while (indev)
    {
        lv_indev_type_t type = lv_indev_get_type(indev);
        if (type == LV_INDEV_TYPE_KEYPAD || type == lv_indev_get_type(indev))
            lv_indev_set_group(indev, grp);
        indev = lv_indev_get_next(indev);
    }
}

static void lv_win_del_wrapper(lv_obj_t* win)
{
    lv_group_t* win_grp = lv_group_get_default();
    lv_group_t* old_grp = (lv_group_t*)lv_obj_get_user_data(win);
    lv_indev_set_keypad_encoder_group(old_grp);
    lv_group_set_default(old_grp);
    lv_group_del(win_grp);
    lv_obj_del(win);
}

static void lv_win_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_obj_t* win = (lv_obj_t*)lv_event_get_user_data(e);
    if (code == LV_EVENT_PRESSED)
    {
        lv_style_t* win_style = (lv_style_t*)lv_obj_get_user_data(lv_win_get_header(win));
        lv_mem_free(win_style);
        lv_win_del_wrapper(win);
    }
}

static lv_obj_t* lv_win_create_wrapper(const char* title, lv_coord_t hor_res, lv_coord_t ver_res,uint8_t close_btn)
{
    lv_obj_t* win = lv_win_create(lv_scr_act(), 15);
    lv_obj_t* header = lv_win_get_header(win);
    lv_obj_set_size(win, hor_res , ver_res);
    lv_win_add_title(win, title);
    lv_group_t* win_grp = lv_group_create();
    lv_indev_set_keypad_encoder_group(win_grp);
    lv_obj_set_user_data(win,lv_group_get_default());
    lv_group_set_default(win_grp);

    if (close_btn)
    {
        lv_obj_t* close_btn = lv_win_add_btn(win, NULL, 15);
        lv_obj_t* close_btn_label = lv_label_create(close_btn);
        lv_label_set_text(close_btn_label, "X");
        lv_obj_center(close_btn_label);
        lv_obj_add_flag(close_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_set_user_data(close_btn, (void*)0);
        lv_obj_add_event_cb(close_btn, lv_win_event_cb, LV_EVENT_PRESSED, win);
    }
    lv_obj_center(win);
    lv_style_t* win_style = (lv_style_t*)lv_mem_alloc(sizeof(lv_style_t));
    lv_obj_set_user_data(header,win_style);
    lv_style_init(win_style);
    lv_style_set_outline_width(win_style, 1);
    lv_style_set_pad_column(win_style, 0);
    lv_style_set_pad_row(win_style, 0);
    lv_obj_add_style(win, win_style, 0);
    lv_obj_add_style(header, win_style, 0);
    lv_obj_add_style(lv_win_get_content(win), win_style,0);
    return win;
}

static void lv_msgbox_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_obj_t* msgbox = (lv_obj_t*)lv_event_get_user_data(e);
    uint8_t(*cb)(uint8_t) = (uint8_t(*)(uint8_t))lv_obj_get_user_data(lv_win_get_content(msgbox));
    uint32_t id = (uint32_t)lv_obj_get_user_data(obj);
    if (code == LV_EVENT_PRESSED&&cb)
    {
        uint8_t res = cb(id);
        if (!res)
            lv_win_del_wrapper(msgbox);
    }
}

lv_obj_t* lv_msgbox_create_wrapper(const char* title,const char*btn_map[], const char* msg, void* cb)
{
    lv_obj_t* msgbox = lv_win_create_wrapper(title, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL), 1);

    lv_obj_t* cont = lv_win_get_content(msgbox);
    lv_obj_set_user_data(cont, cb);
    lv_obj_add_flag(cont, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* msg_label = lv_label_create(cont);
    lv_label_set_long_mode(msg_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(msg_label, msg);
    lv_obj_set_width(msg_label, lv_obj_get_width(cont)-4);

    if((btn_map!=NULL)&& (strcmp(btn_map[0], "") != 0))
    {
        lv_obj_t* btn_cont = lv_obj_create(cont);
        lv_obj_clear_flag(btn_cont, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(btn_cont, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_set_flex_flow(btn_cont, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_grow(btn_cont, 1);
        lv_obj_set_flex_align(btn_cont, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        uint8_t i = 0;
        do
        {
            lv_obj_t* btn = lv_btn_create(btn_cont);
            lv_obj_t* label = lv_label_create(btn);
            lv_label_set_text(label, btn_map[i]);
            lv_obj_set_user_data(btn,(void*)((uint32_t)(i+1)));
            lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
        } while (strcmp(btn_map[++i], "") != 0);
        lv_obj_set_flex_grow(msg_label, 1);
    }

    lv_obj_add_event_cb(msgbox, lv_msgbox_event_cb, LV_EVENT_PRESSED, msgbox);
    return msgbox;
}

static void lv_edit_win_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_obj_t* cont = lv_obj_get_parent(obj);
    lv_obj_t* edit_ta = lv_obj_get_child(cont,0);
    uint8_t(*cb)(void*,double) = (uint8_t(*)(void*,double))lv_obj_get_user_data(obj);
    void* usr_data = lv_event_get_user_data(e);
    if (code == LV_EVENT_READY && cb)
    {
        double data = atof(lv_textarea_get_text(edit_ta));
				lv_group_focus_obj(edit_ta);
        uint8_t res = cb(usr_data,data);
    }
}

lv_obj_t* lv_edit_win_create(void* usr_data, double val,void* cb, edit_kb_map_t key_map)
{
    lv_obj_t* edit_win = lv_win_create_wrapper("Edit", lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL),1);
    lv_obj_t* cont = lv_win_get_content(edit_win);
    lv_obj_add_flag(cont, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* edit_ta = lv_textarea_create(cont);
    lv_obj_set_flex_grow(edit_ta , 1);
    lv_obj_set_width(edit_ta,lv_obj_get_width(cont)-4);
    lv_obj_clear_flag(edit_ta,LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_one_line(edit_ta , 1);
    lv_obj_add_flag(edit_ta,LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* edit_kb = lv_keyboard_create(cont);
    lv_keyboard_set_map(edit_kb, LV_KEYBOARD_MODE_USER_1, edit_kb_uint_map, edit_kb_uint_ctrl_map);
    lv_keyboard_set_mode(edit_kb, LV_KEYBOARD_MODE_USER_1);
    lv_keyboard_set_map(edit_kb, LV_KEYBOARD_MODE_USER_2, edit_kb_int_map, edit_kb_int_ctrl_map);
    lv_keyboard_set_mode(edit_kb, LV_KEYBOARD_MODE_USER_2);
    lv_keyboard_set_map(edit_kb, LV_KEYBOARD_MODE_USER_3, edit_kb_fp_map, edit_kb_fp_ctrl_map);
    lv_keyboard_set_mode(edit_kb, LV_KEYBOARD_MODE_USER_1+key_map);
    lv_keyboard_set_textarea(edit_kb, edit_ta);
    lv_obj_set_flex_grow(edit_kb, 3);
    lv_obj_add_flag(edit_kb,LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_user_data(edit_kb,cb);
		
		uint8_t digits_count = get_digits_count(val);
		char* val_str = lv_mem_alloc(digits_count+5);
		switch(key_map)
		{
			case KB_MAP_FP:
				sprintf(val_str,"%.3f",val);
			case KB_MAP_INT:
				sprintf(val_str,"%d",(int32_t)val);
			case KB_MAP_UINT:
				sprintf(val_str,"%u",(uint32_t)val);
		}
		lv_textarea_set_text(edit_ta,val_str);
		lv_mem_free(val_str);
		
    lv_obj_add_event_cb(edit_win, lv_edit_win_event_cb,LV_EVENT_READY,usr_data);
    return edit_win;
}

static void lv_period_btn_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    uint32_t id = (uint32_t)lv_obj_get_user_data(obj);
    lv_timer_t* timer = (lv_timer_t*)lv_event_get_user_data(e);
    lv_timer_usr_data_t* timer_usr_data = (lv_timer_usr_data_t*)(timer->user_data);
    lv_obj_t* period_label = timer_usr_data->period_label;
    uint32_t current_period = (uint32_t)lv_obj_get_user_data(period_label);
    if (code == LV_EVENT_PRESSED)
    {
        if (id == 1)
            current_period += 10;
        else if (id == 2)
            current_period -= 10;
        if (current_period < 10)
            current_period = 10;
        if (current_period > 1000)
            current_period = 1000;
        lv_timer_pause(timer);
        lv_timer_set_period(timer, current_period);
        lv_obj_set_user_data(period_label, (void*)current_period);
        lv_timer_resume(timer);
        printf("period:%d\n", current_period);
        char period_str[7];
        sprintf(period_str, "%dms", current_period);
        lv_label_set_text(period_label, period_str);
    }
}

static void lv_chart_timer_cb(lv_timer_t* timer)
{
    lv_timer_usr_data_t* timer_usr_data = (lv_timer_usr_data_t*)timer->user_data;
    double (*cb)(void*) = (double (*)(void*))timer_usr_data->callback;
    lv_chart_series_t* chart_ser = lv_chart_get_series_next(timer_usr_data->var_chart,NULL);
    double val = cb(timer_usr_data->usr_data);
    lv_coord_t int_val = lv_chart_adjust_y_range(val, timer_usr_data, timer_usr_data->is_float);
    lv_chart_set_next_value(timer_usr_data->var_chart,chart_ser,int_val);
    char val_str[20];
    if (timer_usr_data->is_float)
        sprintf(val_str, "%.3f", val);
    else
        sprintf(val_str, "%d", int_val);
    lv_label_set_text(timer_usr_data->data_label,val_str);
}

static void lv_graph_win_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_timer_t* timer = (lv_timer_t*)lv_event_get_user_data(e);
    lv_timer_usr_data_t* usr_data = (lv_timer_usr_data_t*)(timer->user_data);
    if (code == LV_EVENT_DELETE)
    {
        lv_timer_del(timer);
        lv_mem_free(usr_data);
    }
}

lv_obj_t* lv_graph_win_create(void* usr_data,void* cb,uint8_t is_float)
{
    lv_obj_t* graph_win = lv_win_create_wrapper("Graph", lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL), 1);
    lv_obj_t* cont = lv_win_get_content(graph_win);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* var_chart = lv_chart_create(cont);
    lv_chart_set_type(var_chart, LV_CHART_TYPE_LINE);
    lv_chart_series_t* chart_data_ser = lv_chart_add_series(var_chart, lv_color_white(), LV_CHART_AXIS_PRIMARY_Y);
    lv_obj_set_style_size(var_chart, 0,LV_PART_INDICATOR);
    lv_chart_set_point_count(var_chart,CHART_DATA_POINT_COUNT);
    lv_chart_set_div_line_count(var_chart, 0, 0);
    lv_chart_set_update_mode(var_chart,LV_CHART_UPDATE_MODE_SHIFT);
    lv_obj_set_user_data(var_chart,(void*)1);
    lv_obj_center(var_chart);
    lv_obj_set_flex_grow(var_chart, 3);

    lv_obj_t* sub_cont = lv_obj_create(cont);
    lv_obj_set_width(sub_cont,lv_obj_get_width(cont)-4);
    lv_obj_clear_flag(sub_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_grow(sub_cont, 1);
    lv_obj_set_flex_flow(sub_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(sub_cont, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* data_label = lv_label_create(sub_cont);

    lv_obj_t* inc_btn = lv_btn_create(sub_cont);
    lv_obj_t* label = lv_label_create(inc_btn);
    lv_label_set_text(label, "+");
    lv_obj_t* period_label = lv_label_create(sub_cont);
    lv_obj_set_user_data(period_label, (void*)0);
    lv_obj_t* dec_btn = lv_btn_create(sub_cont);
    label = lv_label_create(dec_btn);
    lv_label_set_text(label, "-");
    lv_obj_add_flag(inc_btn,LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_user_data(inc_btn,(void*)1);
    lv_obj_add_flag(dec_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_user_data(dec_btn, (void*)2);

    lv_obj_set_flex_grow(data_label, 1);

    lv_timer_usr_data_t* lv_timer_usr_data = (lv_timer_usr_data_t*)lv_mem_alloc(sizeof(lv_timer_usr_data_t));
    lv_timer_usr_data->usr_data = usr_data;
    lv_timer_usr_data->callback = cb;
    lv_timer_usr_data->data_label = data_label;
    lv_timer_usr_data->var_chart = var_chart;
    lv_timer_usr_data->period_label = period_label;
    lv_timer_usr_data->is_float = is_float;

    lv_timer_t * chart_timer = lv_timer_create(lv_chart_timer_cb, 5, lv_timer_usr_data);

    lv_obj_add_event_cb(sub_cont, lv_period_btn_event_cb, LV_EVENT_PRESSED, chart_timer);
    lv_obj_add_event_cb(graph_win, lv_graph_win_event_cb, LV_EVENT_DELETE, chart_timer);
    lv_event_send(sub_cont, LV_EVENT_PRESSED, NULL);

    return graph_win;
}

static lv_menu_usr_data_t* lv_menu_usr_data_create(lv_obj_t* pg,uint32_t data,void* parser)
{
		lv_ll_t* usr_data_ll = lv_obj_get_user_data(pg);
		lv_menu_usr_data_t* menu_usr_data = (lv_menu_usr_data_t*)_lv_ll_ins_head(usr_data_ll);
		menu_usr_data->data = data;
		menu_usr_data->parser = parser;
		return menu_usr_data;
}

static void lv_menu_usr_data_remove(lv_obj_t* pg,lv_menu_usr_data_t* menu_usr_data)
{
		lv_ll_t* usr_data_ll = lv_obj_get_user_data(pg);
		_lv_ll_remove(usr_data_ll,menu_usr_data);
}

static void lv_menu_usr_data_clear(lv_obj_t* pg)
{
		lv_ll_t* usr_data_ll = lv_obj_get_user_data(pg);
		_lv_ll_clear(usr_data_ll);
		lv_mem_free(usr_data_ll);
}

static void lv_menu_set_page_wrapper(lv_obj_t* menu, lv_obj_t* cont, uint32_t data)
{
		lv_obj_t* tar_page = (lv_obj_t*)data;
		lv_menu_set_page(menu,tar_page);
}

char* lv_menu_cont_get_text(lv_obj_t* cont)
{
		lv_obj_t* label = lv_obj_get_child(cont,0);
		return lv_label_get_text(label);
}

static void lv_menu_cont_set_tip(lv_obj_t* cont,char* tip)
{
		lv_obj_t* tip_label = lv_obj_get_child(cont,1);
		lv_label_set_text(tip_label,tip);
}

lv_obj_t* lv_menu_base_cont_create(lv_obj_t* pg, const char* text)
{
    lv_obj_t* cont = lv_menu_cont_create(pg);
		lv_obj_set_flex_flow(cont,LV_FLEX_FLOW_ROW);
		lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		lv_obj_add_flag(cont,LV_OBJ_FLAG_EVENT_BUBBLE);
	
    lv_obj_t* label = lv_label_create(cont);
    lv_label_set_text(label, text);
		//lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
		lv_obj_set_flex_grow(label,1);
	
		lv_obj_t* sym_label = lv_label_create(cont);
		lv_obj_set_width(sym_label,10);
    return cont;
}

lv_obj_t* lv_menu_link_cont_create(lv_obj_t*pg,const char* text,lv_obj_t* child)
{
		lv_obj_t* cont = lv_menu_base_cont_create(pg,text);
		lv_menu_usr_data_t* menu_usr_data = lv_menu_usr_data_create(pg,(uint32_t)child,lv_menu_set_page_wrapper);
		lv_obj_set_user_data(cont,menu_usr_data);
		lv_menu_cont_set_tip(cont,">");
		return cont;
}

lv_obj_t* lv_menu_btn_cont_create(lv_obj_t* pg, const char* text, uint32_t data,void* parser)
{
		lv_obj_t* cont = lv_menu_base_cont_create(pg,text);
		lv_menu_usr_data_t* menu_usr_data = lv_menu_usr_data_create(pg,data,parser);
		lv_obj_set_user_data(cont,menu_usr_data);
		lv_menu_cont_set_tip(cont,"-");
		return cont;
}

lv_obj_t* lv_menu_create_wrapper(lv_obj_t* parent)
{
		lv_obj_t* menu = lv_menu_create(parent);
		lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
		lv_obj_center(menu);
		lv_obj_add_event_cb(menu,lv_menu_event_cb,LV_EVENT_PRESSED,menu);

		lv_obj_t* back_btn = lv_menu_get_main_header_back_btn(menu);
		lv_obj_t* back_btn_lb = lv_label_create(back_btn);
		lv_label_set_text(back_btn_lb, "<");
		return menu;
}

static void lv_menu_event_cb(lv_event_t* e)
{
		lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
		lv_menu_usr_data_t* usr_data = (lv_menu_usr_data_t*)lv_obj_get_user_data(obj);
		lv_obj_t* menu = lv_event_get_user_data(e);
		void (*parser)(lv_obj_t*,lv_obj_t*,uint32_t) = (void (*)(lv_obj_t*,lv_obj_t*,uint32_t))usr_data->parser;
		if (code == LV_EVENT_PRESSED&&usr_data)
    {
				if(parser)
				{
						parser(menu,obj,usr_data->data);
				}
		}
}

static void lv_menu_page_event_cb(lv_event_t* e)
{
		lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
		if(code == LV_EVENT_DELETE)
		{
				lv_menu_usr_data_clear(obj);
		}
}

lv_obj_t* lv_menu_page_create_wrapper(lv_obj_t* menu,char* title)
{
		lv_obj_t* page = lv_menu_page_create(menu,title);
		lv_ll_t* usr_data_ll = lv_mem_alloc(sizeof(lv_ll_t));
		_lv_ll_init(usr_data_ll,sizeof(lv_menu_usr_data_t));
		lv_obj_set_user_data(page,usr_data_ll);
		lv_obj_add_event_cb(page,lv_menu_page_event_cb,LV_EVENT_DELETE,NULL);
		return page;
}

static lv_coord_t lv_chart_adjust_y_range(double val,lv_timer_usr_data_t* usr_data,uint8_t is_float)
{
    lv_chart_t* var_chart = (lv_chart_t*)usr_data->var_chart;
    lv_chart_series_t* chart_ser = lv_chart_get_series_next(usr_data->var_chart, NULL);
    lv_coord_t* y_data = lv_chart_get_y_array(usr_data->var_chart, chart_ser);
    uint32_t scale_factor = (uint32_t)lv_obj_get_user_data(usr_data->var_chart);
    lv_coord_t int_val;
    if (is_float)
    {
        uint32_t now_factor = 0;
        //计算放大因子，把浮点数放大到有3位的整数
        while (get_digits_count(val * pow(10, now_factor)) < 2)
        {
            now_factor += 1;
            if (now_factor == 9)
                break;
        }
        //printf("now_factor:%d\n", now_factor);
        //如果这次的因子更大，那么更新因子
        if (now_factor > scale_factor)
        {
            for (uint8_t i = 0; i < CHART_DATA_POINT_COUNT - 1; i++)
            {
                if (y_data[i] != LV_CHART_POINT_NONE)
                    y_data[i] = (lv_coord_t)((double)y_data[i] * pow(10, now_factor - scale_factor));
            }
            scale_factor = now_factor;
            lv_obj_set_user_data(usr_data->var_chart, (void*)scale_factor);
        }
        int_val = (val * pow(10, now_factor));
    }
    else
    {
        int_val = val;
    }
    /*----------------------------------------------*/
    lv_coord_t y_min = var_chart->ymin[0];
    lv_coord_t y_max = var_chart->ymax[0];
    //判断数据是否填满整个屏幕
    lv_coord_t first_point = y_data[0];
    if (first_point != LV_CHART_POINT_NONE)
    {
        lv_coord_t y_data_max = first_point;
        lv_coord_t y_data_min = first_point;
        //查找最大值和最小值
        for (int i = 0; i < CHART_DATA_POINT_COUNT; i++)
        {
            if (y_data[i] != LV_CHART_POINT_NONE)
            {

                if (y_data[i] > y_data_max)
                    y_data_max = y_data[i];
                if (y_data[i] < y_data_min)
                    y_data_min = y_data[i];
            }
        }
        //计算图像中值，数据中值
        lv_coord_t y_mid = ((float)y_max + (float)y_min) / 2;
        lv_coord_t y_data_mid = ((float)y_data_max + (float)y_data_min) / 2;

        //统计在整个绘图周期中中值错误的个数
        if (usr_data->timer_count < CHART_DATA_POINT_COUNT - 1)
        {
            usr_data->timer_count++;
        }
        else
        {
            usr_data->timer_count = 0;
            usr_data->y_mid_error_count = 0;
            usr_data->y_max_too_large_count = 0;
            usr_data->y_min_too_small_count = 0;
        }

        //如果中值偏移太多，那么一段时间后修改中值
        if (abs(y_mid - y_data_mid) > 5)
        {
            usr_data->y_mid_error_count++;
        }
        //如果当前数据太小，那么一段时间后缩小y轴范围，缩小时保持y轴中值不变
        if ((val - y_mid) > 0 && (val - y_mid) < (y_max - y_mid) / 2)
            usr_data->y_max_too_large_count++;
        else if ((val - y_mid) < 0 && (y_mid - val) < (y_mid - y_min) / 2)
            usr_data->y_min_too_small_count++;
        printf("too small count:%d,too large count:%d\n", usr_data->y_min_too_small_count, usr_data->y_max_too_large_count);
        //printf("mid error count:%d\n", usr_data->y_mid_error_count);

        //如果中值偏移计数器累积到一定值，调整中值
        if (usr_data->y_mid_error_count > (CHART_DATA_POINT_COUNT ))
        {
            y_max += (y_data_mid - y_mid);
            y_min += (y_data_mid - y_mid);
            usr_data->y_mid_error_count = 0;
            usr_data->y_max_too_large_count = 0;
            usr_data->y_min_too_small_count = 0;
        }
        //否则保持中值不变，根据数据调整y轴范围
        else
        {
            //如果最大数据超过了y轴范围，放大y轴范围
            if (y_data_min < y_min)
            {
                y_min = y_data_min + 4;
                y_max = y_mid + (y_mid - y_min);
            }
            if (y_data_max > y_max)
            {
                y_max = y_data_max + 4;
                y_min = y_mid - (y_max - y_mid);
            }

            if (usr_data->y_max_too_large_count > (CHART_DATA_POINT_COUNT / 3))
            {
                y_max = y_data_max;
                y_min = y_mid - (y_max - y_mid);
                usr_data->y_max_too_large_count = 0;
            }
            if (usr_data->y_min_too_small_count > (CHART_DATA_POINT_COUNT / 3))
            {
                y_min = y_data_min;
                y_max = y_mid + (y_mid - y_min);
                usr_data->y_min_too_small_count = 0;
            }
        }
        lv_chart_set_range(usr_data->var_chart, LV_CHART_AXIS_PRIMARY_Y, y_min, y_max);
    }
    return int_val;
}

static uint8_t get_digits_count(double val)
{
    uint8_t digits_count = 0;
    double abs_val = fabs(val);
    while (val >= 1)
    {
        val /= 10;
        digits_count++;
    }
    return digits_count;
}