/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       oled.h
 * @brief      this file contains sd card basic operating function
 * @note         
 * @Version    V1.0.0
 * @Date       Jan-28-2018      
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */

#ifndef __OLED__H
#define __OLED__H

#include "stm32f4xx.h"
#include "main.h"
//#include "spi.h"
#include <stdint.h>

#define Max_Column      132
#define Max_Row         64

#define OLED_CMD        0x00
#define OLED_DATA       0x01

#define CHAR_SIZE_WIDTH     4
#define VHAR_SIZE_HIGHT     12

#define OLED_CMD_Set()      GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define OLED_CMD_Clr()      GPIO_ResetBits(GPIOB, GPIO_Pin_9)

#define OLED_RST_Set()      GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define OLED_RST_Clr()      GPIO_ResetBits(GPIOB, GPIO_Pin_10)



#define OLED_DC_Pin GPIO_Pin_9
#define OLED_DC_GPIO_Port GPIOB
#define OLED_RST_Pin GPIO_Pin_10
#define OLED_RST_GPIO_Port GPIOB

typedef enum
{
    Pen_Clear = 0x00,
    Pen_Write = 0x01,
    Pen_Inversion = 0x02,
}Pen_Typedef;

/* function define */
void oled_init(void);
void oled_write_byte(uint8_t dat, uint8_t cmd);
void oled_display_on(void);
void oled_display_off(void);
void oled_refresh_gram(void);
void oled_clear(Pen_Typedef pen);
void oled_drawpoint(uint8_t x, uint8_t y, Pen_Typedef pen);
void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_Typedef pen);
void oled_showchar(uint8_t row, uint8_t col, uint8_t chr);
void oled_shownum(uint8_t row, uint8_t col, uint32_t num, uint8_t mode, uint8_t len);
void oled_showstring(uint8_t row, uint8_t col, uint8_t *chr);
void oled_printf(uint8_t row, uint8_t col, char *fmt,...);
void oled_LOGO(void);
void oled_refresh_gram_partly(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
//extern uint8_t SPI1_ReadWriteByte(uint8_t TxData);

#endif
