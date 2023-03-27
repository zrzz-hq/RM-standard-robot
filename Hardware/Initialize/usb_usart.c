#include "usb_usart.h"


__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END;

u8 Usb_Write_Buffer[256];

void Usb_Init(void){
	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
}

void Usb_Read_Data(uint8_t * pbuf, uint32_t buf_len){
	DCD_EP_PrepareRx(&USB_OTG_dev, CDC_OUT_EP, pbuf, buf_len);
}

void Usb_Write_Data(char* fmt,...){  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)Usb_Write_Buffer,fmt,ap);
	va_end(ap);
	i=strlen((const char*)Usb_Write_Buffer);
	for(j=0;j<i;j++){
		VCP_TxData(Usb_Write_Buffer[j]); 
	}
} 
