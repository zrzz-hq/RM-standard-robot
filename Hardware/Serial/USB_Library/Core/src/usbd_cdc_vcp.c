/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Generic media access Layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#pragma     data_alignment = 4
#endif                          /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------ */
#include "usbd_cdc_vcp.h"
#include "usb_usart.h"

#include "string.h"	
#include "stdarg.h"		 
#include "stdio.h"	


/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
__IO uint32_t data_sent;
__IO uint32_t receive_flag = 0;
__IO uint32_t receive_count = 1;

LINE_CODING linecoding = {
  115200,                       /* baud rate */
  0x00,                         /* stop bits-1 */
  0x00,                         /* parity - none */
  0x08                          /* nb. of bits 8 */
};


USART_InitTypeDef USART_InitStructure;

/* These are external variables imported from CDC core to be used for IN
 * transfer management. */
extern uint8_t APP_Rx_Buffer[]; /* Write CDC received data in this buffer.
                                 * These data will be sent over USB IN endpoint
                                 * in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;  /* Increment this pointer or roll it back to
                                 * start address when writing received data in
                                 * the buffer APP_Rx_Buffer. */

/* Private function prototypes ----------------------------------------------- */
static uint16_t VCP_Init(void);
static uint16_t VCP_DeInit(void);
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t * Buf, uint32_t Len);
static uint16_t VCP_DataTx(uint8_t data);
static uint16_t VCP_DataRx(uint8_t * Buf, uint32_t Len);

CDC_IF_Prop_TypeDef VCP_fops = {
  VCP_Init,
  VCP_DeInit,
  VCP_Ctrl,
  VCP_DataTx,
  VCP_DataRx
};

/* Private functions --------------------------------------------------------- */
/**
  * @brief  VCP_Init
  *         Initializes the Media on the STM32
  * @param  None
  * @retval Result of the operation (USBD_OK in all cases)
  */
static uint16_t VCP_Init(void)
{
//  NVIC_InitTypeDef NVIC_InitStructure;

//  /* EVAL_COM1 default configuration */
//  /* EVAL_COM1 configured as follow: - BaudRate = 115200 baud - Word Length = 8 
//   * Bits - One Stop Bit - Parity Odd - Hardware flow control disabled -
//   * Receive and transmit enabled */
//  USART_InitStructure.USART_BaudRate = 115200;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_Odd;
//  USART_InitStructure.USART_HardwareFlowControl =
//    USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//  /* Configure and enable the USART */
//  STM_EVAL_COMInit(COM1, &USART_InitStructure);

//  /* Enable the USART Receive interrupt */
//  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);

//  /* Enable USART Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

  return USBD_OK;
}

/**
  * @brief  VCP_DeInit
  *         DeInitializes the Media on the STM32
  * @param  None
  * @retval Result of the operation (USBD_OK in all cases)
  */
static uint16_t VCP_DeInit(void)
{

  return USBD_OK;
}


/**
  * @brief  VCP_Ctrl
  *         Manage the CDC class requests
  * @param  Cmd: Command code            
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation (USBD_OK in all cases)
  */
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t * Buf, uint32_t Len)
{
  switch (Cmd)
  {
  case SEND_ENCAPSULATED_COMMAND:
    /* Not needed for this driver */
    break;

  case GET_ENCAPSULATED_RESPONSE:
    /* Not needed for this driver */
    break;

  case SET_COMM_FEATURE:
    /* Not needed for this driver */
    break;

  case GET_COMM_FEATURE:
    /* Not needed for this driver */
    break;

  case CLEAR_COMM_FEATURE:
    /* Not needed for this driver */
    break;

  case SET_LINE_CODING:
    linecoding.bitrate =
      (uint32_t) (Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
    linecoding.format = Buf[4];
    linecoding.paritytype = Buf[5];
    linecoding.datatype = Buf[6];
    /* Set the new configuration */
//    VCP_COMConfig(OTHER_CONFIG);
    break;

  case GET_LINE_CODING:
    Buf[0] = (uint8_t) (linecoding.bitrate);
    Buf[1] = (uint8_t) (linecoding.bitrate >> 8);
    Buf[2] = (uint8_t) (linecoding.bitrate >> 16);
    Buf[3] = (uint8_t) (linecoding.bitrate >> 24);
    Buf[4] = linecoding.format;
    Buf[5] = linecoding.paritytype;
    Buf[6] = linecoding.datatype;
    break;

  case SET_CONTROL_LINE_STATE:
    /* Not needed for this driver */
    break;

  case SEND_BREAK:
    /* Not needed for this driver */
    break;

  default:
    break;
  }

  return USBD_OK;
}

/**
  * @brief  VCP_DataTx
  *         CDC received data to be send over USB IN endpoint are managed in 
  *         this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
  */
uint16_t VCP_DataTx(uint8_t data)
{

	APP_Rx_Buffer[APP_Rx_ptr_in]=data;	//写入发送buf
	APP_Rx_ptr_in++;  					//写位置加1
	if(APP_Rx_ptr_in==APP_RX_DATA_SIZE)	//超过buf大小了,归零.
	{
		APP_Rx_ptr_in = 0;
	}   
	
  return USBD_OK;
}

/**
  * @brief  VCP_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *           
  *         @note
  *         This function will block any OUT packet reception on USB endpoint 
  *         until exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result 
  *         in receiving more data while previous ones are still not sent.
  *                 
  * @param  Buf: Buffer of data received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
  */
static uint16_t VCP_DataRx(uint8_t * Buf, uint32_t Len)
{
//  uint32_t i;

//  for (i = 0; i < Len; i++)
//  {
//    USART_SendData(EVAL_COM1, *(Buf + i));
//    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET);
//  }

  return USBD_OK;
}

void VCP_TxData(uint8_t data)
{
	APP_Rx_Buffer[APP_Rx_ptr_in]=data;	//写入发送buf
	APP_Rx_ptr_in++;  					//写位置加1
	if(APP_Rx_ptr_in==APP_RX_DATA_SIZE)	//超过buf大小了,归零.
	{
		APP_Rx_ptr_in = 0;
	}   
	
}

/* send data function */
void VCP_SendData(USB_OTG_CORE_HANDLE * pdev, uint8_t * pbuf, uint32_t buf_len)
{
  data_sent = 0;
  DCD_EP_Tx(pdev, CDC_IN_EP, pbuf, buf_len);
}

/* check data sent */
uint32_t VCP_CheckDataSent(void)
{
  if (data_sent)
    return 1;
  return 0;
}

/* prepare data to be received */
void VCP_ReceiveData(USB_OTG_CORE_HANDLE * pdev, uint8_t * pbuf, uint32_t buf_len)
{
  DCD_EP_PrepareRx(pdev, CDC_OUT_EP, pbuf, buf_len);
}

/* check data received */
uint32_t VCP_CheckDataReceived(USB_OTG_CORE_HANDLE * pdev)
{
  data_sent = 0;
	
	receive_count = USBD_GetRxCount(pdev,(CDC_OUT_EP & 0x7F));
	
  return receive_count;
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
