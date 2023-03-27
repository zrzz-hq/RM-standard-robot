/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       oled.c
 * @brief      this file contains sd card basic operating function
 * @note         
 * @Version    V1.0.0
 * @Date       Jan-28-2018      
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */
 
#include <stdio.h>
#include <stdarg.h>
 
#include "main.h"
#include "oled.h"
#include "oledfont.h"
#include "math.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

static uint8_t current_x;
static uint8_t current_y;


//int a;
/**
 * OLED flash Addr:
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
**/

volatile uint8_t OLED_GRAM[132][8];
//volatile uint8_t OLED_GRAM_turn[8][128];

//short delay uesd in spi transmmit
//void delay_ms(uint16_t delaytimes)
//{
//    uint16_t i;
//    for (i = 0; i < delaytimes; i++ )
//    {
//        int a = 10000;  //delay based on mian clock, 168Mhz
//        while (a-- );
//    }
//}


/**
 * @brief   write data/command to OLED
 * @param   dat: the data ready to write
 * @param   cmd: 0x00,command 0x01,data
 * @retval  
 */
void oled_write_byte(uint8_t dat, uint8_t cmd)
{
    if (cmd != 0)
        OLED_CMD_Set();
    else
        OLED_CMD_Clr();

		OLED_SPI_SendData(dat);
}


/**
 * @brief   set OLED cursor position
 * @param   x: the X-axis of cursor
 * @param   y: the Y-axis of cursor
 * @retval  
 */
static void oled_set_pos(uint8_t x, uint8_t y)
{
    oled_write_byte((0xb0 + y), OLED_CMD);              //set page address y								//add a nop
		oled_write_byte((x&0x0f), OLED_CMD);                //set column low address
    oled_write_byte(((x&0xf0)>>4)|0x10, OLED_CMD);      //set column high address
		current_x = x;
		current_y = y;
}

/**
 * @brief   turn on OLED display
 * @param   None
 * @param   None
 * @retval  
 */
void oled_display_on(void)
{
    oled_write_byte(0x8d, OLED_CMD);
    oled_write_byte(0x14, OLED_CMD);
    oled_write_byte(0xaf, OLED_CMD);
}

/**
 * @brief   turn off OLED display
 * @param   None
 * @param   None
 * @retval  
 */
void oled_display_off(void)
{
    oled_write_byte(0x8d, OLED_CMD);
    oled_write_byte(0x10, OLED_CMD);
    oled_write_byte(0xae, OLED_CMD);
}

void oled_nop(void)
{
		oled_write_byte(0xe3, OLED_CMD);
}


void oled_refresh_gram_partly(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
    uint8_t i, n;
	
		if(y1)
			y1 = y1/8;
		if(y2)
			y2 = y2/8;
		
		oled_set_pos(Max_Column-1,y1);

    for (i = y1; i <= y2; i++)
    {
        oled_set_pos(x1, i);
				oled_nop();
				oled_nop();

        for (n = x1; n <= x2; n++)
        {
						oled_nop();
						oled_write_byte(OLED_GRAM[n][i], OLED_DATA);
        }
//				oled_set_pos(Max_Column-1,i);
				oled_nop();
    }
}

/**
 * @brief   refresh the RAM of OLED
 * @param   None
 * @param   None
 * @retval  
 */
void oled_refresh_gram(void)
{
    uint8_t i, n;
	
		oled_set_pos(0,0);
		oled_nop();

    for (i = 0; i < Max_Row/8; i++)
    {
        oled_set_pos(0, i);

        for (n = 0; n < Max_Column; n++)
        {
            oled_write_byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

/**
 * @brief   clear the screen
 * @param   None
 * @param   None
 * @retval  
 */
void oled_clear(Pen_Typedef pen)
{
    uint8_t i, n;

    for (i = 0; i < Max_Row; i++)
    {
        for (n = 0; n < Max_Column; n++)
        {
            if (pen == Pen_Write)
                OLED_GRAM[n][i] = 0xff;
            else if (pen == Pen_Clear)
                OLED_GRAM[n][i] = 0x00;
            else
                OLED_GRAM[n][i] = 0xff - OLED_GRAM[n][i];
        }
    }
}

/**
 * @brief   draw a point at (x, y)
 * @param   x: the X-axis of cursor
 * @param   y: the Y-axis of cursor
 * @param   pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_Typedef
 * @retval  None
 */
void oled_drawpoint(uint8_t x, uint8_t y, Pen_Typedef pen)
{
    uint8_t page = 0, row = 0;

    /* check the corrdinate */
    if ( (x > (Max_Column - 1)) || (y > (Max_Row - 1)))
        return;

    page = y / 8;
    row = y % 8;

    if (pen == Pen_Write)
        OLED_GRAM[x][page] |= 1 << row;
    else if (pen == Pen_Inversion)
        OLED_GRAM[x][page] ^= 1 << row;
    else
        OLED_GRAM[x][page] &= ~(1 << row);
}

/**
 * @brief   draw a line from (x1, y1) to (x2, y2)
 * @param   x1, y1: the start point of line
 * @param   x2, y2: the end of line
 * @param   pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_Typedef
 * @retval  None
 */
void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_Typedef pen)
{
    uint8_t col = 0, row = 0;
    uint8_t x_st = 0, x_ed = 0, y_st = 0, y_ed = 0;
    float k = 0.0f, b = 0.0f;

    if (y1 == y2)
    {
        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x1);

        for (col = x_st; col <= x_ed; col++)
        {
            oled_drawpoint(col, y1, pen);
        }
    }
    else if (x1 == x2)
    {
        (y1 <= y2) ? (y_st = y1):(y_st = y2);
        (y1 <= y2) ? (y_ed = y2):(y_ed = y1);

        for (row = y_st; row <= y_ed; row++)
        {
            oled_drawpoint(x1, row, pen);
        }
    }
    else
    {
        k = ((float)(y2 - y1)) / (x2 - x1);
        b = (float)y1 - k * x1;

        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x2);

        for (col = x_st; col <= x_ed; col++)
        {
            oled_drawpoint(col, (uint8_t)(col * k + b), pen);
        }
    }
}

void oled_Drwaline(uint8_t line_num)
{
	oled_drawline(0,line_num,127,line_num,Pen_Inversion);
}

//To add: rectangle, fillrectangle, circle, fillcircle, 

/**
 * @brief   show a character
 * @param   row: row of character
 * @param   col: column of character
 * @param   chr: the character ready to show
 * @retval  None
 */
void oled_showchar(uint8_t row, uint8_t col, uint8_t chr)
{
    uint8_t x = col * 6;
    uint8_t y = row * 12;
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr = chr - ' ';

    for (t = 0; t < 12; t++)
    {
        temp = asc2_1206[chr][t];

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp&0x80)
                oled_drawpoint(x, y, Pen_Write);
            else
                oled_drawpoint(x, y, Pen_Clear);

            temp <<= 1;
            y++;
            if ((y - y0) == 12)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

//m^n
static uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)
        result *= m;

    return result;
}

/**
 * @brief   show a number
 * @param   row: row of number
 * @param   col: column of number
 * @param   num: the number ready to show
 * @param   mode: 0x01, fill number with '0'; 0x00, fill number with spaces
 * @param   len: the length of the number
 * @retval  None
 */
void oled_shownum(uint8_t row, uint8_t col, uint32_t num, uint8_t mode, uint8_t len)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t -1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode == 0)
                    oled_showchar(row, col + t, ' ');
                else
                    oled_showchar(row, col + t, '0');
                continue;
            }
            else
                enshow = 1;
        }

        oled_showchar(row, col + t, temp + '0');
    }
}


/**
 * @brief   show a character string
 * @param   row: row of character string begin
 * @param   col: column of character string begin
 * @param   chr: the pointer to character string
 * @retval  None
 */
void oled_showstring(uint8_t row, uint8_t col, uint8_t *chr)
{
    uint8_t n =0;

    while (chr[n] != '\0')
    {
        oled_showchar(row, col, chr[n]);
        col++;

        if (col > 20)
        {
            col = 0;
            row += 1;
        }
        n++;
    }
}

/**
 * @brief   formatted output in oled 128*64
 * @param   row: row of character string begin, 0 <= row <= 4;
 * @param   col: column of character string begin, 0 <= col <= 20;
 * @param   *fmt: the pointer to format character string
 * @retval  None
 * @note    if the character length is more than one row at a time, the extra characters will be truncated
 */
void oled_printf(uint8_t row, uint8_t col, char *fmt,...)
{
    uint8_t LCD_BUF[128] = {0};
    uint8_t remain_size = 0;
    va_list ap;

    if ((row > 4) || (row < 1) || (col > 20) || (col < 1))
        return;

    va_start(ap, fmt);

    vsprintf((char *)LCD_BUF, fmt, ap);

    va_end(ap);

    remain_size = 21 - col;

    LCD_BUF[remain_size] = '\0';

    oled_showstring(row, col, LCD_BUF);
}

void oled_LOGO(void)
{
    oled_clear(Pen_Clear);
    uint8_t temp_char = 0;
    uint8_t x = 0, y = 0;
    uint8_t i = 0;
    for(; y < 64; y += 8)
    {
        for(x = 0; x < 128; x++)
        {
            temp_char = LOGO_BMP[x][y/8];
            for(i = 0; i < 8; i++)
            {
                if(temp_char & 0x80) oled_drawpoint(x, y + i,Pen_Write);
                else oled_drawpoint(x,y + i,Pen_Clear);
                temp_char <<= 1;
            }
        }
    }
}

/**
 * @brief   initialize the oled module
 * @param   None
 * @retval  None
 */
void oled_init(void)
{
		OLED_RST_Clr();
    vTaskDelay(500);
    OLED_RST_Set();

    oled_write_byte(0xae, OLED_CMD);    //turn off oled panel
    oled_write_byte(0x00, OLED_CMD);    //set low column address
    oled_write_byte(0x10, OLED_CMD);    //set high column address
    oled_write_byte(0x40, OLED_CMD);    //set start line address
    oled_write_byte(0x81, OLED_CMD);    //set contrast control resigter
    oled_write_byte(0xcf, OLED_CMD);    //set SEG output current brightness
    oled_write_byte(0xa1, OLED_CMD);    //set SEG/column mapping
    oled_write_byte(0xc8, OLED_CMD);    //set COM/row scan direction
    oled_write_byte(0xa6, OLED_CMD);    //set nomarl display
    oled_write_byte(0xa8, OLED_CMD);    //set multiplex display
    oled_write_byte(0x3f, OLED_CMD);    //1/64 duty
    oled_write_byte(0xd3, OLED_CMD);    //set display offset
    oled_write_byte(0x00, OLED_CMD);    //not offest
    oled_write_byte(0xd5, OLED_CMD);    //set display clock divide ratio/oscillator frequency
    oled_write_byte(0xf0, OLED_CMD);    //set divide ratio 
    oled_write_byte(0xd9, OLED_CMD);    //set pre-charge period
    oled_write_byte(0xf1, OLED_CMD);    //pre-charge: 15 clocks, discharge: 1 clock
    oled_write_byte(0xda, OLED_CMD);    //set com pins hardware configuration 
    oled_write_byte(0x12, OLED_CMD);    //
    oled_write_byte(0xdb, OLED_CMD);    //set vcomh
    oled_write_byte(0x40, OLED_CMD);    //set vcom deselect level
    oled_write_byte(0x20, OLED_CMD);    //set page addressing mode
    oled_write_byte(0x02, OLED_CMD);    //
    oled_write_byte(0x8d, OLED_CMD);    //set charge pump enable/disable
    oled_write_byte(0x14, OLED_CMD);    //charge pump disable
    oled_write_byte(0xa4, OLED_CMD);    //disable entire dispaly on
    oled_write_byte(0xa6, OLED_CMD);    //disable inverse display on
    oled_write_byte(0xaf, OLED_CMD);    //turn on oled panel

    //oled_write_byte(0xaf, OLED_CMD);    //display on

    oled_clear(Pen_Clear);
    oled_set_pos(0, 0);
		oled_LOGO();
		oled_refresh_gram();
		//vTaskDelay(1000);

}