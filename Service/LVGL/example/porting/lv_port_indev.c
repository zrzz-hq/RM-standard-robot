/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "../../lvgl.h"
#include "gpio.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void keypad_init(void);
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

/**********************
 *  STATIC VARIABLES
 **********************/;
lv_indev_t * indev_keypad;
static uint8_t last_key = KEY_NONE;
static uint8_t key_pressed;
static uint8_t key_released;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Keypad
     * -----------------*/

    /*Initialize your keypad or keyboard if you have*/
    keypad_init();

    /*Register a keypad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_keypad, group);`*/

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Keypad
 * -----------------*/

/*Initialize your keypad*/
static void keypad_init(void)
{
    
}

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    /*Get whether the a key is pressed and save the pressed key*/
    uint8_t act_key = Oled_Key_Read();
    if(act_key != KEY_NONE) 
		{
        data->state = LV_INDEV_STATE_PR;
        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch(act_key) {
            case KEY_DOWN:
                act_key = LV_KEY_NEXT;
                break;
            case KEY_UP:
                act_key = LV_KEY_PREV;
                break;
            case KEY_LEFT:
                act_key = LV_KEY_LEFT;
                break;
            case KEY_RIGHT:
                act_key = LV_KEY_RIGHT;
                break;
            case KEY_ENTER:
                act_key = LV_KEY_ENTER;
                break;
        }
				last_key = act_key;
    }
    else 
		{
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
