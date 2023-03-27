/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Header for usbd_cdc_vcp.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_core.h"
#include "usbd_conf.h"


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ )      /* !< IAR Compiler */
#pragma data_alignment=4
#endif
#endif                          /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Exported typef ------------------------------------------------------------*/
/* The following structures groups all needed parameters to be configured for the 
   ComPort. These parameters can modified on the fly by the host through CDC class
   command class requests. */
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
}LINE_CODING;

/* Exported constants --------------------------------------------------------*/
/* The following define is used to route the USART IRQ handler to be used.
   The IRQ handler function is implemented in the usbd_cdc_vcp.c file. */
          

#define DEFAULT_CONFIG                  0
#define OTHER_CONFIG                    1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void VCP_TxData(uint8_t data);
void VCP_SendData( USB_OTG_CORE_HANDLE *pdev, uint8_t* pbuf, uint32_t  buf_len);
uint32_t VCP_CheckDataSent(void);
uint16_t VCP_DataTx(uint8_t data);
void VCP_ReceiveData(USB_OTG_CORE_HANDLE *pdev, uint8_t  *pbuf, uint32_t   buf_len);
//uint32_t VCP_CheckDataReceived(void);
uint32_t VCP_CheckDataReceived(USB_OTG_CORE_HANDLE * pdev);
#endif /* __USBD_CDC_VCP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
