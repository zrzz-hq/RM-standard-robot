
#include "DJI_Remote_Control.h"

#include "usart1_dbus.h"

extern unsigned char sbus_rx_buffer[25]; 

float motor_set[2];

#define SBUS_RX_BUF_NUM 36u

#define RC_FRAME_LENGTH 18u

//接收原始数据，为18个字节，给了36个字节长度，防止DMA传输越界
static uint8_t SBUS_rx_buf[2][SBUS_RX_BUF_NUM];

void remote_control_init(void)
{
    Usart1_Dbus_Init(SBUS_rx_buf[0], SBUS_rx_buf[1], SBUS_RX_BUF_NUM);
}

/*
*************遥控器数据结构体*************
@Note 	需要使用遥控器相关数据的时候
				需要先引用其他页面中的RC_Ctl变量
				extern RC_Ctl_t RC_Ctl;
				然后直接用RC_Ctl结构体中的数据即可
******************************************
*/
RC_Ctl_t RC_Ctl_Data;
//****************************************

void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_Ctl_t *rc_ctrl)
{
    if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }

				rc_ctrl->rc.ch0 = (sbus_buf[0]| (sbus_buf[1] << 8)) & 0x07ff;          	//
				rc_ctrl->rc.ch1 = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff;   //    
				rc_ctrl->rc.ch2 = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) | (sbus_buf[4] << 10)) & 0x07ff;          
				rc_ctrl->rc.ch3 = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff;   //  
				rc_ctrl->rc.ch4 = sbus_buf[16] | (sbus_buf[17] << 8);                 	//左上角波轮      
				rc_ctrl->rc.s1  = ((sbus_buf[5] >> 4)& 0x000C) >> 2;                          // 
				rc_ctrl->rc.s2  = ((sbus_buf[5] >> 4)& 0x0003);    														//
				
			
				rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //鼠标x轴        
				rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //鼠标y轴     
				rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //鼠标Z轴         
				rc_ctrl->mouse.press_l = sbus_buf[12];                                        //鼠标左键  
				rc_ctrl->mouse.press_r = sbus_buf[13];                                        //鼠标右键
				rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);   									//键盘按键值					
						
				rc_ctrl->rc.ch0 -= ((uint16_t)1024);
				rc_ctrl->rc.ch1 -= ((uint16_t)1024);
				rc_ctrl->rc.ch2 -= ((uint16_t)1024);
				rc_ctrl->rc.ch3 -= ((uint16_t)1024);
				rc_ctrl->rc.ch4 -= ((uint16_t)1024);
				
//				if(rc_ctrl->rc.ch0 > )
		
//    rc_ctrl->rc.ch[0] = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;        //!< Channel 0
//    rc_ctrl->rc.ch[1] = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; //!< Channel 1
//    rc_ctrl->rc.ch[2] = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) |          //!< Channel 2
//                         (sbus_buf[4] << 10)) &
//                        0x07ff;
//    rc_ctrl->rc.ch[3] = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; //!< Channel 3
//    rc_ctrl->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);                  //!< Switch left
//    rc_ctrl->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2;                       //!< Switch right
//    rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //!< Mouse X axis
//    rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //!< Mouse Y axis
//    rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //!< Mouse Z axis
//    rc_ctrl->mouse.press_l = sbus_buf[12];                                  //!< Mouse Left Is Press ?
//    rc_ctrl->mouse.press_r = sbus_buf[13];                                  //!< Mouse Right Is Press ?
//    rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                    //!< KeyBoard value
//    rc_ctrl->rc.ch[4] = sbus_buf[16] | (sbus_buf[17] << 8);                 //NULL

//    rc_ctrl->rc.ch[0] -= RC_CH_VALUE_OFFSET;
//    rc_ctrl->rc.ch[1] -= RC_CH_VALUE_OFFSET;
//    rc_ctrl->rc.ch[2] -= RC_CH_VALUE_OFFSET;
//    rc_ctrl->rc.ch[3] -= RC_CH_VALUE_OFFSET;
//    rc_ctrl->rc.ch[4] -= RC_CH_VALUE_OFFSET;
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(USART1);
    }
    else if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        static uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART1);

        if(DMA_GetCurrentMemoryTarget(DMA2_Stream2) == 0)
        {
            //重新设置DMA
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, SBUS_RX_BUF_NUM);
            DMA2_Stream2->CR |= DMA_SxCR_CT;
            //清DMA中断标志
            DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
            DMA_Cmd(DMA2_Stream2, ENABLE);
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                SBUS_TO_RC(SBUS_rx_buf[0], &RC_Ctl_Data);
            }
        }
        else
        {
            //重新设置DMA
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, SBUS_RX_BUF_NUM);
            DMA2_Stream2->CR &= ~(DMA_SxCR_CT);
            //清DMA中断标志
            DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
            DMA_Cmd(DMA2_Stream2, ENABLE);
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                SBUS_TO_RC(SBUS_rx_buf[1], &RC_Ctl_Data);
            }

        }
    }
}

//void DMA2_Stream5_IRQHandler(void)
//{              
//    if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
//    {
//        DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
//        DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
//				
//				RC_Ctl_Data.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;          	//
//				RC_Ctl_Data.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;   //    
//				RC_Ctl_Data.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;          
//				RC_Ctl_Data.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;   //  
//				RC_Ctl_Data.rc.ch4 = sbus_rx_buffer[16] | (sbus_rx_buffer[17] << 8);                 	//左上角波轮      
//				RC_Ctl_Data.rc.s1  = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;                          // 
//				RC_Ctl_Data.rc.s2  = ((sbus_rx_buffer[5] >> 4)& 0x0003);    														//
//				
//			
//				RC_Ctl_Data.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8);                    //鼠标x轴        
//				RC_Ctl_Data.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8);                    //鼠标y轴     
//				RC_Ctl_Data.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8);                  //鼠标Z轴         
//				RC_Ctl_Data.mouse.press_l = sbus_rx_buffer[12];                                        //鼠标左键  
//				RC_Ctl_Data.mouse.press_r = sbus_rx_buffer[13];                                        //鼠标右键
//				RC_Ctl_Data.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8);   									//键盘按键值					
//						
//				RC_Ctl_Data.rc.ch0 -= ((uint16_t)1024);
//				RC_Ctl_Data.rc.ch1 -= ((uint16_t)1024);
//				RC_Ctl_Data.rc.ch2 -= ((uint16_t)1024);
//				RC_Ctl_Data.rc.ch3 -= ((uint16_t)1024);
//				RC_Ctl_Data.rc.ch4 -= ((uint16_t)1024);
//    }
//		 			
//}

int RC_CH[12];
int RC_CH_LA[12];
int DJI_RC_Motion_Check(void)
{

	RC_CH[0] = RC_DeadBand_Limit(RC_Ctl_Data.rc.ch0,RC_DEADLINE);
	RC_CH[1] = RC_DeadBand_Limit(RC_Ctl_Data.rc.ch1,RC_DEADLINE);
	RC_CH[2] = RC_DeadBand_Limit(RC_Ctl_Data.rc.ch2,RC_DEADLINE);
	RC_CH[3] = RC_DeadBand_Limit(RC_Ctl_Data.rc.ch3,RC_DEADLINE);
	RC_CH[4] = RC_DeadBand_Limit(RC_Ctl_Data.rc.ch4,RC_DEADLINE);
	RC_CH[5] = RC_Ctl_Data.mouse.x;
	RC_CH[6] = RC_Ctl_Data.mouse.y;
	RC_CH[7] = RC_Ctl_Data.mouse.z;
	RC_CH[8] = RC_Ctl_Data.mouse.press_l;
	RC_CH[9] = RC_Ctl_Data.mouse.press_r;
	RC_CH[10] = RC_Ctl_Data.key.v;
	if((RC_CH_LA[0] == 0 ) && ( RC_CH[0] == 0) &&\
		 (RC_CH_LA[1] == 0 ) && ( RC_CH[1] == 0) &&\
		 (RC_CH_LA[2] == 0 ) && ( RC_CH[2] == 0) &&\
		 (RC_CH_LA[3] == 0 ) && ( RC_CH[3] == 0) &&\
		 (RC_CH_LA[4] == 0 ) && ( RC_CH[4] == 0) &&\
	 	 (RC_CH_LA[5] == 0 ) && ( RC_CH[5] == 0) &&\
	 	 (RC_CH_LA[6] == 0 ) && ( RC_CH[6] == 0) &&\
		 (RC_CH_LA[7] == 0 ) && ( RC_CH[7] == 0) &&\
		 (RC_CH_LA[8] == 0 ) && ( RC_CH[8] == 0) &&\
		 (RC_CH_LA[9] == 0 ) && ( RC_CH[9] == 0) &&\
		 (RC_CH_LA[10] == 0 ) && ( RC_CH[10] == 0))
	{
		for(int kk = 0;kk < 12;kk ++)
		RC_CH_LA[kk] = RC_CH[kk];
		return 0;
	}
	else
	{
	for(int kk = 0;kk < 12;kk ++)
		RC_CH_LA[kk] = RC_CH[kk];
	return 1;	
	}
}

int DJI_RC_Data_Check(void)
{
	if((RC_Ctl_Data.rc.ch0 > -660) && (RC_Ctl_Data.rc.ch0 < 660) &&\
		 (RC_Ctl_Data.rc.ch1 > -660) && (RC_Ctl_Data.rc.ch1 < 660) &&\
		 (RC_Ctl_Data.rc.ch2 > -660) && (RC_Ctl_Data.rc.ch2 < 660) &&\
		 (RC_Ctl_Data.rc.ch3 > -660) && (RC_Ctl_Data.rc.ch3 < 660) &&\
		 (RC_Ctl_Data.rc.ch4 > -660) && (RC_Ctl_Data.rc.ch4 < 660))
		return 1;
	else
		return 0;
}

void RC_Restart_Set(void)
{
	 RC_restart(SBUS_RX_BUF_NUM);
}

const RC_Ctl_t *Get_DJI_RC_Data_Address(void)
{
	return &RC_Ctl_Data;
}

int16_t RC_DeadBand_Limit(int16_t Input,int16_t DeadLine)
{
		if((Input < -DeadLine)||(Input > DeadLine))
				return Input;
		else
				return 0;
}


