#include "lvgl_usr_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lvgl.h"
#include "data_task.h"

#define LV_SUB_TASK_PRO 10
#define LV_SUB_TASK_STK_SIZE 256


/*------------------------------------------------*/
static void lv_iterator_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_iterator_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_iterator_event_cb(lv_event_t* e);

static void lv_thread_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_thread_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_thread_event_cb(lv_event_t* e);

static void lv_menu_base_cont_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_menu_base_cont_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_menu_base_cont_event_cb(lv_event_t* e);

static void lv_menu_link_cont_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_menu_link_cont_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_menu_link_cont_event_cb(lv_event_t* e);

static void lv_menu_btn_cont_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_menu_btn_cont_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_menu_btn_cont_event_cb(lv_event_t* e);

static void lv_base_win_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_base_win_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_base_win_event_cb(lv_event_t* e);

static void lv_base_msgbox_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_base_msgbox_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_base_msgbox_event_cb(lv_event_t* e);

static void lv_exec_msgbox_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_exec_msgbox_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_exec_msgbox_event_cb(lv_event_t* e);

static void lv_edit_win_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_edit_win_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_edit_win_event_cb(lv_event_t* e);

static void lv_graph_win_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_graph_win_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_graph_win_event_cb(lv_event_t* e);

static void lv_prog_win_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_prog_win_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_prog_win_event_cb(lv_event_t* e);

static uint8_t get_digits_count(double val);
static lv_coord_t lv_chart_adjust_y_range(lv_graph_win_t* graph_win, double val);

/*------------------------------------------------*/


/*------------------------------------------------*/

const lv_obj_class_t lv_iterator_class = {
    .constructor_cb = lv_iterator_constructor,
    .destructor_cb = lv_iterator_destructor,
    .base_class = &lv_obj_class,
    .instance_size = sizeof(lv_iterator_t)
};

const lv_obj_class_t lv_thread_class = {
    .constructor_cb = lv_thread_constructor,
    .destructor_cb = lv_thread_destructor,
    .base_class = &lv_obj_class,
    .instance_size = sizeof(lv_thread_t)
};

const lv_obj_class_t lv_menu_base_cont_class = {
    .constructor_cb = lv_menu_base_cont_constructor,
    .destructor_cb = lv_menu_base_cont_destructor,
    .base_class = &lv_menu_cont_class,
    .width_def = LV_PCT(100),
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_menu_base_cont_t),
    .theme_inheritable = LV_OBJ_CLASS_THEME_INHERITABLE_TRUE
};

const lv_obj_class_t lv_menu_link_cont_class = {
    .constructor_cb = lv_menu_link_cont_constructor,
    .destructor_cb = lv_menu_link_cont_destructor,
    .base_class = &lv_menu_base_cont_class,
    .width_def = LV_PCT(100),
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_menu_link_cont_t),
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .theme_inheritable = LV_OBJ_CLASS_THEME_INHERITABLE_TRUE
};

const lv_obj_class_t lv_menu_btn_cont_class = {
    .constructor_cb = lv_menu_btn_cont_constructor,
    .destructor_cb = lv_menu_btn_cont_destructor,
    .base_class = &lv_menu_base_cont_class,
    .width_def = LV_PCT(100),
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_menu_btn_cont_t),
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .theme_inheritable = LV_OBJ_CLASS_THEME_INHERITABLE_TRUE
};

const lv_obj_class_t lv_base_win_class = {
    .constructor_cb = lv_base_win_constructor,
    .destructor_cb = lv_base_win_destructor,
    .base_class = &lv_win_class,
    .instance_size = sizeof(lv_base_win_t)
};

const lv_obj_class_t lv_base_msgbox_class = {
    .constructor_cb = lv_base_msgbox_constructor,
    .destructor_cb = lv_base_msgbox_destructor,
    .base_class = &lv_base_win_class,
    .instance_size = sizeof(lv_base_msgbox_t)
};

const lv_obj_class_t lv_exec_msgbox_class = {
    .constructor_cb = lv_exec_msgbox_constructor,
    .destructor_cb = lv_exec_msgbox_destructor,
    .base_class = &lv_base_msgbox_class,
    .instance_size = sizeof(lv_exec_msgbox_t)
};

const lv_obj_class_t lv_edit_win_class = {
    .constructor_cb = lv_edit_win_constructor,
    .destructor_cb = lv_edit_win_destructor,
    .base_class = &lv_base_win_class,
    .instance_size = sizeof(lv_edit_win_t)
};

const lv_obj_class_t lv_graph_win_class = {
    .constructor_cb = lv_graph_win_constructor,
    .destructor_cb = lv_graph_win_destructor,
    .base_class = &lv_base_win_class,
    .instance_size = sizeof(lv_graph_win_t)
};

const lv_obj_class_t lv_prog_win_class = {
    .constructor_cb = lv_prog_win_constructor,
    .destructor_cb = lv_prog_win_destructor,
    .base_class = &lv_base_win_class,
    .instance_size = sizeof(lv_prog_win_t)
};

/*------------------------------------------------*/

static const char* edit_kb_fp_map[] = { "OK", "0" , "1", "2", "3", "<-", "\n",
                                       "+/-", "." , "4", "5", "6", "7", "8", "9","" };

static const lv_btnmatrix_ctrl_t edit_kb_fp_ctrl_map[] = { LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 1, 1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS | 3,
                                                                                        3, 1, 1, 1, 1, 1, 1, 1 };

static const char* edit_kb_uint_map[] = { "OK", "0", "1", "<-", "\n",
                                           "2", "3", "4", "5", "6", "7", "8", "9","" };

static const lv_btnmatrix_ctrl_t edit_kb_uint_ctrl_map[] = { LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS | 3,
                                                                                    1, 1, 1, 1, 1, 1, 1, 1 };

static const char* edit_kb_int_map[] = { "OK", "0", "1", "2", "3", "<-", "\n",
                                        "+/-", "4", "5", "6", "7", "8", "9", "" };

static const lv_btnmatrix_ctrl_t edit_kb_int_ctrl_map[] = { LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 1, 1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS | 3,
                                                                                         3, 1, 1, 1, 1, 1, 1 };

																																												 
																																												 
/****************************************************/
lv_obj_t* lv_iterator_create(lv_obj_t* parent,void* next,void* prev,void* first,uint16_t item_count)
{
		lv_obj_t* obj = lv_obj_class_create_obj(&lv_thread_class, parent);
		lv_obj_class_init_obj(obj);
		lv_iterator_t* iter = (lv_iterator_t*)obj;
		
		iter->next = next;
		iter->prev = prev;
		iter->first = first;
		iter->item_count = item_count;
}
																																												 
static void lv_iterator_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		
}
static void lv_iterator_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
	
}
/****************************************************/																																												 
static void* lv_thread_create_hook(const char* name,void* func,void* param)
{
		TaskHandle_t LV_SubTask_Handle;
		xTaskCreate((TaskFunction_t)func,          //任务函数
						(const char *)name,          //任务名称
						(uint16_t)LV_SUB_TASK_STK_SIZE,            //任务堆栈大小
						(void *)param,                        //传递给任务函数的参数
						(UBaseType_t)LV_SUB_TASK_PRO,        //任务优先级
						(TaskHandle_t *)&LV_SubTask_Handle); //任务句柄
		return LV_SubTask_Handle;
}

static void lv_thread_kill_hook(void* thread_handle)
{
		vTaskDelete(thread_handle);
}

static uint8_t lv_thread_suspend_hook(void* thread_handle)
{
		vTaskSuspend(thread_handle);
}

static uint8_t lv_thread_resume_hook(void* thread_handle)
{
		vTaskResume(thread_handle);
}
/****************************************************/	
uint8_t lv_thread_resume(lv_obj_t* obj)
{
		lv_thread_t* thread = (lv_thread_t*)obj;
		if (lv_thread_resume_hook(thread->thread_handle))
		{
				lv_event_send(obj, LV_EVENT_VALUE_CHANGED, (void*)LV_THREAD_READY);
				return 1;
		}
		return 0;
}

uint8_t lv_thread_suspend(lv_obj_t* obj)
{
		lv_thread_t* thread = (lv_thread_t*)obj;
		if (lv_thread_suspend_hook(thread->thread_handle))
		{
				lv_event_send(obj, LV_EVENT_VALUE_CHANGED, (void*)LV_THREAD_SUSPENDED);
				return 1;
		}
		return 0;
}

void lv_thread_kill(lv_obj_t* obj)
{
	lv_thread_t* thread = (lv_thread_t*)obj;
	lv_thread_kill_hook(thread->thread_handle);
	lv_event_send(obj,LV_EVENT_VALUE_CHANGED,(void*)LV_THREAD_DELETED);
}

static void lv_thread_main(void* obj)
{
		lv_thread_t* thread = (lv_thread_t*)obj;

		lv_event_send(obj,LV_EVENT_VALUE_CHANGED,(void*)LV_THREAD_RUNNING);
	
		uint8_t(*run)(void*) = (uint8_t(*)(void*))thread->run;

		if (run)
			thread->res = run(obj);
		
		lv_thread_suspend(obj);
}

lv_obj_t* lv_thread_create(lv_obj_t* parent,void* run,void* user_data,uint8_t auto_run)
{
		lv_obj_t* obj = lv_obj_class_create_obj(&lv_thread_class, parent);
		lv_obj_class_init_obj(obj);
		lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
		lv_obj_set_user_data(obj,user_data);
		lv_thread_t* thread = (lv_thread_t*)obj;
	
		thread->run = run;
		if (auto_run)
				lv_thread_resume(obj);
		return obj;
}

static void lv_thread_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		lv_thread_t* thread = (lv_thread_t*)obj;
		thread->thread_handle = lv_thread_create_hook("LV_Thread",lv_thread_main,obj);
		lv_thread_suspend(obj);
}
static void lv_thread_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		lv_thread_t* thread = (lv_thread_t*)obj;
		lv_thread_kill(obj);
}	
/****************************************************/
lv_obj_t* lv_menu_base_cont_create(lv_obj_t* parent)
{
    lv_obj_t* obj = lv_obj_class_create_obj(&lv_menu_base_cont_class, parent);
    lv_obj_class_init_obj(obj);
		return obj;
}

char* lv_menu_base_cont_get_text(lv_obj_t* cont)
{
		lv_obj_t* label = lv_obj_get_child(cont,0);
		return lv_label_get_text(label);
}

void lv_menu_base_cont_set_text(lv_obj_t* obj,const char* text)
{
    lv_menu_base_cont_t* cont = (lv_menu_base_cont_t*)obj;
    lv_label_set_text(cont->label,text);
}

static void lv_menu_base_cont_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    lv_menu_base_cont_t* cont = (lv_menu_base_cont_t*)obj;
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    cont->label = lv_label_create(obj);
    lv_obj_set_flex_grow(cont->label, 1);

    cont->tip_label = lv_label_create(obj);
    lv_obj_set_width(cont->tip_label, 10);

    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

}
static void lv_menu_base_cont_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{

}
/****************************************************/
lv_obj_t* lv_menu_link_cont_create(lv_obj_t* parent, const char* text, lv_obj_t* page)
{
    lv_obj_t* obj = lv_obj_class_create_obj(&lv_menu_link_cont_class, parent);
    lv_obj_class_init_obj(obj);

    lv_menu_link_cont_t* cont = (lv_menu_link_cont_t*)obj;
    lv_menu_base_cont_set_text(obj, text);
    cont->page = page;
	
		return obj;
}

static void lv_menu_link_cont_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_menu_link_cont_t* cont = (lv_menu_link_cont_t*)obj;
    lv_obj_t* menu = lv_obj_get_parent(lv_obj_get_parent(cont->page));
    if (code == LV_EVENT_PRESSED && cont->page)
    {
        lv_menu_set_page(menu, cont->page);
    }
}

static void lv_menu_link_cont_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    lv_menu_link_cont_t* cont = (lv_menu_link_cont_t*)obj;
    lv_label_set_text(cont->cont.tip_label, ">");
    lv_obj_add_event_cb(obj, lv_menu_link_cont_event_cb,LV_EVENT_PRESSED,NULL);
}

static void lv_menu_link_cont_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{

}
/****************************************************/
lv_obj_t* lv_menu_btn_cont_create(lv_obj_t* parent, const char* text, void* user_data, void* user_data_parser)
{
    lv_obj_t* obj = lv_obj_class_create_obj(&lv_menu_btn_cont_class, parent);
    lv_obj_class_init_obj(obj);

    lv_menu_btn_cont_t* cont = (lv_menu_btn_cont_t*)obj;
    lv_obj_set_user_data(obj,user_data);
    cont->user_data_parser = user_data_parser;
    lv_menu_base_cont_set_text(obj,text);
	
		return obj;
}

static void lv_menu_btn_cont_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_menu_btn_cont_t* cont = (lv_menu_btn_cont_t*)obj;
    void* user_data = cont->user_data;
    void(*user_data_parser)(lv_obj_t*) = cont->user_data_parser;
    if (code == LV_EVENT_PRESSED && user_data_parser)
    {
        user_data_parser(obj);
    }
}

static void lv_menu_btn_cont_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    lv_menu_btn_cont_t* cont = (lv_menu_btn_cont_t*)obj;
    lv_label_set_text(cont->cont.tip_label,"-");
    lv_obj_add_event_cb(obj, lv_menu_btn_cont_event_cb, LV_EVENT_PRESSED, NULL);
}

static void lv_menu_btn_cont_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    
}
/****************************************************/
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

static void lv_base_win_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_obj_t* win = lv_event_get_user_data(e);
    if (code == LV_EVENT_PRESSED)
    {
        lv_obj_del(win);
    }
}

lv_obj_t* lv_base_win_add_close_btn(lv_obj_t* obj)
{
    lv_base_win_t* win = (lv_base_win_t*)obj;
    win->close_btn = lv_win_add_btn(obj, NULL, 15);
    lv_obj_t* close_btn_label = lv_label_create(win->close_btn);
    lv_label_set_text(close_btn_label, "X");
    lv_obj_center(close_btn_label);
    lv_obj_add_flag(win->close_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(win->close_btn, lv_base_win_event_cb, LV_EVENT_PRESSED, win);
    return win->close_btn;
}

lv_obj_t* lv_base_win_disable_close_btn(lv_obj_t* obj)
{
    lv_base_win_t* win = (lv_base_win_t*)obj;
    lv_obj_add_flag(win->close_btn, LV_OBJ_FLAG_HIDDEN);
}

lv_obj_t* lv_base_win_enable_close_btn(lv_obj_t* obj)
{
    lv_base_win_t* win = (lv_base_win_t*)obj;
    lv_obj_clear_flag(win->close_btn, LV_OBJ_FLAG_HIDDEN);
}

lv_group_t* lv_group_switch(lv_group_t* new_group)
{
    lv_group_t* old_group = lv_group_get_default();
    lv_group_set_default(new_group);
    return old_group;
}

void lv_base_win_set_title(lv_obj_t* win, const char* title)
{
    lv_obj_t* title_label = lv_obj_get_child(lv_win_get_header(win), 0);
    lv_label_set_text(title_label, title);
}

lv_obj_t* lv_base_win_create(lv_obj_t* parent, char* title, uint8_t close_btn)
{
    lv_obj_t* obj = lv_obj_class_create_obj(&lv_menu_btn_cont_class, parent);
    lv_obj_class_init_obj(obj);

    lv_base_win_t* win = (lv_base_win_t*)obj;
    if (close_btn)
        lv_base_win_add_close_btn(obj);

    lv_win_add_title(obj,title);

    if (close_btn == 0 && title == NULL)
        lv_obj_add_flag(lv_win_get_header(obj),LV_OBJ_FLAG_HIDDEN);

    return obj;
}

static void lv_base_win_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    lv_base_win_t* win = (lv_base_win_t*)obj;
    win->win_group = lv_group_create();
    win->old_group = lv_group_switch(win->win_group);
    lv_indev_set_keypad_encoder_group(win->win_group);

    lv_obj_t* header = lv_win_get_header(obj);
    lv_obj_set_size(header,LV_PCT(100),15);

    lv_obj_center(obj);
    win->win_style = lv_mem_alloc(sizeof(lv_style_t));
    LV_ASSERT_MALLOC(win->win_style);
    lv_style_init(win->win_style);
    lv_style_set_outline_width(win->win_style, 1);
    lv_style_set_pad_column(win->win_style, 0);
    lv_style_set_pad_row(win->win_style, 0);
    lv_obj_add_style(obj, win->win_style, 0);
    lv_obj_add_style(lv_win_get_header(obj), win->win_style, 0);
    lv_obj_add_style(lv_win_get_content(obj), win->win_style, 0);
}
static void lv_base_win_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    lv_base_win_t* win = (lv_base_win_t*)obj;
    lv_mem_free(win->win_style);
    lv_group_switch(win->old_group);
    lv_indev_set_keypad_encoder_group(win->old_group);
    lv_group_del(win->win_group);
}
/****************************************************/
static void lv_base_msgbox_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_base_msgbox_t* msgbox = lv_event_get_user_data(e);
    uint8_t(*cb)(uint32_t, void*) = msgbox->cb;
    if (code == LV_EVENT_PRESSED && cb)
    {
        uint32_t btn_id = (uint32_t)lv_obj_get_user_data(obj);
        if (!cb(btn_id, lv_obj_get_user_data((lv_obj_t*)msgbox)))
            lv_obj_del((lv_obj_t*)msgbox);
    }
}


lv_obj_t* lv_base_msgbox_add_btns(lv_obj_t* obj, const char* btn_map[])
{
    lv_base_msgbox_t* msgbox = (lv_base_msgbox_t*)obj;
    if ((btn_map != NULL) && (strcmp(btn_map[0], "") != 0))
		{
				uint8_t i = 0;
				do
				{
						lv_obj_t* btn = lv_btn_create(msgbox->btn_cont);
						lv_obj_t* label = lv_label_create(btn);
						lv_label_set_text(label, btn_map[i]);
						lv_obj_set_user_data(btn, (void*)((uint32_t)i));
						lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
				} while (strcmp(btn_map[++i], "") != 0);
				lv_obj_add_event_cb(msgbox->btn_cont, lv_base_msgbox_event_cb, LV_EVENT_PRESSED, msgbox);
				lv_obj_clear_flag(msgbox->btn_cont,LV_OBJ_FLAG_HIDDEN);
		}
    return msgbox->btn_cont;
}

void lv_base_msgbox_set_msg_text(lv_obj_t* msgbox, const char* msg_text)
{
    lv_obj_t* cont = lv_win_get_content(msgbox);
		lv_obj_t* msg_label = lv_obj_get_child(cont,0);
    lv_label_set_text(msg_label, msg_text);
}

void lv_base_msgbox_set_cb(lv_obj_t* obj,void* cb)
{
		lv_base_msgbox_t* msgbox = (lv_base_msgbox_t*)obj;
		msgbox->cb=cb;
}

lv_obj_t* lv_base_msgbox_create(lv_obj_t* parent, const char* title, const char* msg_text, const char* btn_map[], void* user_data, void* cb)
{
    lv_obj_t* obj = lv_obj_class_create_obj(&lv_base_msgbox_class, parent);
    lv_obj_class_init_obj(obj);
    lv_base_msgbox_t* msgbox = (lv_base_msgbox_t*)obj;

    lv_base_win_set_title(obj, title);
    lv_base_msgbox_add_btns(obj,btn_map);
    lv_base_msgbox_set_msg_text(obj,msg_text);
    
    lv_obj_set_user_data(obj,user_data);
    lv_base_msgbox_set_cb(obj,cb);

    return obj;
}

static void lv_base_msgbox_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		lv_base_msgbox_t* msgbox = (lv_base_msgbox_t*)obj;
	
		lv_obj_t* cont = lv_win_get_content(obj);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_win_add_title(obj, "Message");
    lv_base_win_add_close_btn(obj);

    msgbox->msg_label = lv_label_create(cont);
    lv_label_set_long_mode(msgbox->msg_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(msgbox->msg_label, lv_obj_get_width(cont) - 4);
    lv_obj_set_flex_grow(msgbox->msg_label, 1);
	
		msgbox->btn_cont = lv_obj_create(cont);
		lv_obj_clear_flag(msgbox->btn_cont, LV_OBJ_FLAG_SCROLLABLE);
		lv_obj_add_flag(msgbox->btn_cont, LV_OBJ_FLAG_HIDDEN);
		lv_obj_set_flex_flow(msgbox->btn_cont, LV_FLEX_FLOW_ROW);
		lv_obj_set_flex_grow(msgbox->btn_cont, 1);
		lv_obj_set_flex_align(msgbox->btn_cont, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		
}

static void lv_base_msgbox_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		
}
/****************************************************/

static void lv_exec_msgbox_async_cb(void* obj)
{
		lv_exec_msgbox_t* msgbox = (lv_exec_msgbox_t*)obj;
    void (*exec)(void*) = (void (*)(void*))msgbox->exec;
    if (exec)
    {
        exec(msgbox);
				lv_base_win_enable_close_btn((lv_obj_t*)msgbox);
    }
}

void lv_exec_msgbox_run(lv_obj_t* obj,uint8_t cancel_able)
{
    lv_refr_now(NULL);
		lv_exec_msgbox_t* msgbox = (lv_exec_msgbox_t*)obj;
    if (!cancel_able)
        lv_base_win_disable_close_btn(obj);
    lv_async_call(lv_exec_msgbox_async_cb,obj);
}

lv_obj_t* lv_exec_msgbox_create(lv_obj_t* parent, const char* title, const char* btn_map[],void* exec, void* user_data, void* cb)
{
		lv_obj_t* obj = lv_obj_class_create_obj(&lv_exec_msgbox_class, parent);
    lv_obj_class_init_obj(obj);
    lv_exec_msgbox_t* msgbox = (lv_exec_msgbox_t*)obj;

    lv_base_win_set_title(obj, title);
    lv_base_msgbox_add_btns(obj,btn_map);
    lv_base_msgbox_set_msg_text(obj,"Please Wait...");

    msgbox->exec = exec;
  
    lv_obj_set_user_data(obj,user_data);
    lv_base_msgbox_set_cb(obj,cb);
		
    return obj;
}

static void lv_exec_msgbox_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		
}
static void lv_exec_msgbox_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		
}
/****************************************************/
static void lv_edit_win_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_edit_win_t* win = (lv_edit_win_t*)lv_event_get_user_data(e);
    uint8_t(*cb)(lv_obj_t*) = win->cb;
    if (code == LV_EVENT_READY && cb)
    { 
        cb((lv_obj_t*)win);
				lv_group_focus_obj(win->edit_textarea);
    }
}

double lv_edit_win_get_val(lv_obj_t* win)
{
		lv_edit_win_t* edit_win = (lv_edit_win_t*)win;
		return atof(lv_textarea_get_text(edit_win->edit_textarea));
}

lv_obj_t* lv_edit_win_create(lv_obj_t* parent,double val, void* user_data, void* cb, edit_keyboard_map_t key_map)
{
    lv_obj_t* obj = lv_obj_class_create_obj(&lv_edit_win_class, parent);
    lv_obj_class_init_obj(obj);
    lv_win_add_title(obj, "Edit");
    lv_base_win_add_close_btn(obj);
    lv_edit_win_t* edit_win = (lv_edit_win_t*)obj;

    lv_obj_t* cont = lv_win_get_content(obj);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    edit_win->edit_textarea = lv_textarea_create(cont);
    lv_obj_set_flex_grow(edit_win->edit_textarea, 1);
    lv_obj_set_width(edit_win->edit_textarea, lv_obj_get_width(cont) - 4);
    lv_obj_clear_flag(edit_win->edit_textarea, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_one_line(edit_win->edit_textarea, 1);
    lv_obj_add_flag(edit_win->edit_textarea, LV_OBJ_FLAG_EVENT_BUBBLE);

    edit_win->edit_keyboard = lv_keyboard_create(cont);
    lv_keyboard_set_map(edit_win->edit_keyboard, LV_KEYBOARD_MODE_USER_1, edit_kb_uint_map, edit_kb_uint_ctrl_map);
    lv_keyboard_set_mode(edit_win->edit_keyboard, LV_KEYBOARD_MODE_USER_1);
    lv_keyboard_set_map(edit_win->edit_keyboard, LV_KEYBOARD_MODE_USER_2, edit_kb_int_map, edit_kb_int_ctrl_map);
    lv_keyboard_set_mode(edit_win->edit_keyboard, LV_KEYBOARD_MODE_USER_2);
    lv_keyboard_set_map(edit_win->edit_keyboard, LV_KEYBOARD_MODE_USER_3, edit_kb_fp_map, edit_kb_fp_ctrl_map);
    lv_keyboard_set_mode(edit_win->edit_keyboard, LV_KEYBOARD_MODE_USER_1 + key_map);
    lv_keyboard_set_textarea(edit_win->edit_keyboard, edit_win->edit_textarea);
    lv_obj_set_flex_grow(edit_win->edit_keyboard, 3);

    uint8_t digits_count = get_digits_count(val);
    char* val_str = lv_mem_alloc(digits_count + 5);
    LV_ASSERT_MALLOC(val_str);
    switch (key_map)
    {
    case EDIT_KB_MAP_FP:
        sprintf(val_str, "%.3f", val);
				break;
    case EDIT_KB_MAP_INT:
        sprintf(val_str, "%d", (int32_t)val);
				break;
    case EDIT_KB_MAP_UINT:
        sprintf(val_str, "%u", (uint32_t)val);
				break;
    }
    lv_textarea_set_text(edit_win->edit_textarea, val_str);
    lv_mem_free(val_str);

    lv_obj_add_event_cb(edit_win->edit_keyboard, lv_edit_win_event_cb, LV_EVENT_READY, edit_win);

    lv_obj_set_user_data(obj,user_data);
    edit_win->cb = cb;
    
    return obj;
}

static void lv_edit_win_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{

}
static void lv_edit_win_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{

}
/****************************************************/
static void lv_graph_win_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    uint32_t id = (uint32_t)lv_obj_get_user_data(obj);
    lv_graph_win_t* graph_win = lv_event_get_user_data(e);
    uint32_t period = graph_win->current_period;
    if (code == LV_EVENT_PRESSED)
    {
        if (id == 1)
            period += 10;
        else if (id == 2)
            period -= 10;
        if (period < 10)
            period = 10;
        if (period > 1000)
            period = 1000;
        lv_timer_pause(graph_win->refresh_timer);
        lv_timer_set_period(graph_win->refresh_timer, period);
        lv_timer_resume(graph_win->refresh_timer);

        graph_win->current_period = period;

        //printf("period:%d\n", period);
        char period_str[5];
        sprintf(period_str, "%dms", period);
        lv_label_set_text(graph_win->period_label, period_str);
    }
}

lv_obj_t* lv_graph_win_create(lv_obj_t* parent,void* user_data,void* cb,uint8_t data_count,uint8_t is_float)
{
    lv_obj_t* obj = lv_obj_class_create_obj(&lv_graph_win_class, parent);
    lv_obj_class_init_obj(obj);
    lv_win_add_title(obj, "Graph");
    lv_base_win_add_close_btn(obj);

    lv_graph_win_t* graph_win = (lv_graph_win_t*)obj;
    lv_obj_t* cont = lv_win_get_content(obj);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    graph_win->graph = lv_chart_create(cont);
    lv_chart_set_type(graph_win->graph, LV_CHART_TYPE_LINE);
    lv_chart_series_t* chart_data_ser = lv_chart_add_series(graph_win->graph, lv_color_white(), LV_CHART_AXIS_PRIMARY_Y);
    lv_obj_set_style_size(graph_win->graph, 0, LV_PART_INDICATOR);
    lv_chart_set_point_count(graph_win->graph, data_count);
    lv_chart_set_div_line_count(graph_win->graph, 0, 0);
    lv_chart_set_update_mode(graph_win->graph, LV_CHART_UPDATE_MODE_SHIFT);
    lv_obj_center(graph_win->graph);
    lv_obj_set_flex_grow(graph_win->graph, 3);

    lv_obj_t* sub_cont = lv_obj_create(cont);
    lv_obj_set_width(sub_cont, lv_obj_get_width(cont) - 4);
    lv_obj_clear_flag(sub_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_grow(sub_cont, 1);
    lv_obj_set_flex_flow(sub_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(sub_cont, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    graph_win->val_label = lv_label_create(sub_cont);
    lv_obj_set_flex_grow(graph_win->val_label, 1);

    lv_obj_t* inc_btn = lv_btn_create(sub_cont);
    lv_obj_t* label = lv_label_create(inc_btn);
    lv_label_set_text(label, "+");
    graph_win->period_label = lv_label_create(sub_cont);
    lv_obj_t* dec_btn = lv_btn_create(sub_cont);
    label = lv_label_create(dec_btn);
    lv_label_set_text(label, "-");
    lv_obj_add_flag(inc_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_user_data(inc_btn, (void*)1);
    lv_obj_add_flag(dec_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_user_data(dec_btn, (void*)2);

    lv_obj_add_event_cb(sub_cont, lv_graph_win_event_cb, LV_EVENT_PRESSED, graph_win);
    lv_event_send(sub_cont, LV_EVENT_PRESSED, NULL);

    graph_win->cb = cb;
    graph_win->is_float = is_float;
    graph_win->data_count = data_count;
		lv_obj_set_user_data(obj,user_data);

    return obj;
}

static void lv_chart_timer_cb(lv_timer_t* timer)
{
    lv_graph_win_t* graph_win = (lv_graph_win_t*)timer->user_data;

    double (*cb)(lv_obj_t*) = graph_win->cb;
    if (cb)
    {
        lv_chart_series_t* chart_ser = lv_chart_get_series_next(graph_win->graph, NULL);
        double val = cb((lv_obj_t*)graph_win);

        lv_coord_t int_val = lv_chart_adjust_y_range(graph_win, val);
        lv_chart_set_next_value(graph_win->graph, chart_ser, int_val);

        char* val_str = lv_mem_alloc(get_digits_count(val) + 5);
        LV_ASSERT_MALLOC(val_str);
        if (graph_win->is_float)
            sprintf(val_str, "%.3f", val);
        else
            sprintf(val_str, "%d", int_val);
        lv_label_set_text(graph_win->val_label, val_str);
        lv_mem_free(val_str);
    }
}

static void lv_graph_win_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    lv_graph_win_t* graph_win = (lv_graph_win_t*)obj;
    graph_win->refresh_timer = lv_timer_create(lv_chart_timer_cb, 5, obj);
}

static void lv_graph_win_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    lv_graph_win_t* graph_win = (lv_graph_win_t*)obj;
    lv_timer_del(graph_win->refresh_timer);
}

static lv_coord_t lv_chart_adjust_y_range(lv_graph_win_t* graph_win,double val)
{
    lv_chart_series_t* chart_ser = lv_chart_get_series_next(graph_win->graph, NULL);
    lv_coord_t* y_data = lv_chart_get_y_array(graph_win->graph, chart_ser);
    lv_coord_t int_val;

    //绘制满一整屏幕的数据后，清零标志位
    if (graph_win->timer_count < graph_win->data_count - 1)
    {
        graph_win->timer_count++;
    }
    else
    {
        graph_win->timer_count = 0;
        graph_win->y_mid_error_count = 0;
        graph_win->y_max_too_large_count = 0;
        graph_win->y_min_too_small_count = 0;
        graph_win->scale_factor_to_small_count = 0;
    }

    if (graph_win->is_float)
    {
        uint32_t now_factor = 0;
        uint8_t change_factor = 0;
        //计算放大因子，把浮点数放大到有3位的整数
        while (get_digits_count(val * pow(10, now_factor)) < 2)
        {
            now_factor += 1;
            if (now_factor == 9)
                break;
        }
        //printf("now_factor:%d\n", now_factor);
        //如果这次的因子更大，那么更新因子
        if (now_factor > graph_win->scale_factor)
        {
            change_factor = 1;
        }
        //如果比较小，那么累积到一定数量后再更新因子
        else if(now_factor < graph_win->scale_factor)
        {
            graph_win->scale_factor_to_small_count++;
        }

        if (graph_win->scale_factor_to_small_count > graph_win->data_count)
        {
            change_factor = 1;
        }

        if (change_factor)
        {
            graph_win->scale_factor_to_small_count = 0;
            for (uint8_t i = 0; i < graph_win->data_count - 1; i++)
            {
                if (y_data[i] != LV_CHART_POINT_NONE)
                    y_data[i] = (lv_coord_t)((double)y_data[i] * pow(10, now_factor - graph_win->scale_factor));
            }
            graph_win->scale_factor = now_factor;
        }

        int_val = (val * pow(10, now_factor));
    }
    else
    {
        int_val = val;
    }
    /*----------------------------------------------*/
    lv_coord_t y_min = ((lv_chart_t*)graph_win->graph)->ymin[0];
    lv_coord_t y_max = ((lv_chart_t*)graph_win->graph)->ymax[0];
    //判断数据是否填满整个屏幕
    lv_coord_t first_point = y_data[0];
    if (first_point != LV_CHART_POINT_NONE)
    {
        lv_coord_t y_data_max = first_point;
        lv_coord_t y_data_min = first_point;
        //查找最大值和最小值
        for (int i = 0; i < graph_win->data_count; i++)
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

        //如果中值偏移太多，那么一段时间后修改中值
        if (abs(y_mid - y_data_mid) > 5)
        {
            graph_win->y_mid_error_count++;
        }
        //如果当前数据太小，那么一段时间后缩小y轴范围，缩小时保持y轴中值不变
        if ((val - y_mid) > 0 && (val - y_mid) < (y_max - y_mid) / 2)
            graph_win->y_max_too_large_count++;
        else if ((val - y_mid) < 0 && (y_mid - val) < (y_mid - y_min) / 2)
            graph_win->y_min_too_small_count++;
        printf("too small count:%d,too large count:%d\n", graph_win->y_min_too_small_count, graph_win->y_max_too_large_count);
        //printf("mid error count:%d\n", graph_win->y_mid_error_count);

        //如果中值偏移计数器累积到一定值，调整中值
        if (graph_win->y_mid_error_count > graph_win->data_count)
        {
            y_max += (y_data_mid - y_mid);
            y_min += (y_data_mid - y_mid);
            graph_win->y_mid_error_count = 0;
            graph_win->y_max_too_large_count = 0;
            graph_win->y_min_too_small_count = 0;
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

            if (graph_win->y_max_too_large_count > (graph_win->data_count / 3))
            {
                y_max = y_data_max;
                y_min = y_mid - (y_max - y_mid);
                graph_win->y_max_too_large_count = 0;
            }
            if (graph_win->y_min_too_small_count > (graph_win->data_count / 3))
            {
                y_min = y_data_min;
                y_max = y_mid + (y_mid - y_min);
                graph_win->y_min_too_small_count = 0;
            }
        }
        lv_chart_set_range(graph_win->graph, LV_CHART_AXIS_PRIMARY_Y, y_min, y_max);
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
/****************************************************/
static void lv_prog_win_create(lv_obj_t* parent)
{
		
}

static void lv_prog_win_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		
}
static void lv_prog_win_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
		
}
static void lv_prog_win_event_cb(lv_event_t* e);

/****************************************************/

