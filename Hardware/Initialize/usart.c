#include "usart.h"


/**
  * @brief  Configures the USART1 Peripheral.
  * @param  None
  * @retval None
  */
void USART1_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1); 
	
	/* Configure USART1 as alternate function  */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USART1, ENABLE); 
	
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        //usart rx idle interrupt  enabled
	
	/* Enable USART */
  USART_Cmd(USART1, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
 
  USART1_printf(" USART1 Printf Example: retarget the C library printf function to the USART1.\r\n");
 
}





/**
  * @brief  Configures the USART1 Peripheral.
  * @param  None
  * @retval None
  */
void USART2_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 
	
	/* Configure USART1 as alternate function  */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_DeInit(USART2);
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);        //usart rx idle interrupt  enabled
	
	/* Enable USART */
  USART_Cmd(USART2, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
 
 USART2_printf(" USART2 Printf Example: retarget the C library printf function to the USART2.\r\n");
}

/**
  * @brief  Configures the USART1 Peripheral.
  * @param  None
  * @retval None
  */
void USART3_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3); 
	
	/* Configure USART1 as alternate function  */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_DeInit(USART3);
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);        //usart rx idle interrupt  enabled
	
	/* Enable USART */
  USART_Cmd(USART3, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
 
 USART3_printf(" USART3 Printf Example: retarget the C library printf function to the USART3.\r\n");
}




/**
  * @brief  Configures the USART1 Peripheral.
  * @param  None
  * @retval None
  */
void USART6_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); 
	
	/* Configure USART1 as alternate function  */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	USART_DeInit(USART6);
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART6, &USART_InitStructure);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);        //usart rx idle interrupt  enabled
	
	/* Enable USART */
  USART_Cmd(USART6, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
 
 USART6_printf(" USART6 Printf Example: retarget the C library printf function to the USART6.\r\n");
}



/**
  * @brief  Configures the USART1 Peripheral.
  * @param  None
  * @retval None
  */
void UART7_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_UART7);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_UART7); 
	
	/* Configure USART1 as alternate function  */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	USART_DeInit(UART7);
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;

  USART_Init(UART7, &USART_InitStructure);
//	USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);        //usart rx idle interrupt  enabled
	
	/* Enable USART */
  USART_Cmd(UART7, ENABLE);
	
	
//	NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//	NVIC_Init(&NVIC_InitStructure);
// 
// UART7_printf(" UART7 Printf Example: retarget the C library printf function to the USART7.\r\n");
}


/**
  * @brief  Configures the USART1 Peripheral.
  * @param  None
  * @retval None
  */
void UART8_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_UART8);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_UART8); 
	
	/* Configure USART1 as alternate function  */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	USART_DeInit(UART8);
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(UART8, &USART_InitStructure);
	USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);        //usart rx idle interrupt  enabled
	
	/* Enable USART */
  USART_Cmd(UART8, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
 
 UART8_printf(" UART8 Printf Example: retarget the C library printf function to the UART8.\r\n");
}

int fputc(int ch, FILE *f)
{
	
	USART6_SendData((uint8_t)ch);
  return (ch);
}

void USART1_SendData(uint8_t data)
{
    USART_SendData(USART1,data);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void USART2_SendData(uint8_t data)
{
    USART_SendData(USART2,data);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

void USART3_SendData(uint8_t data)
{
    USART_SendData(USART3,data);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void USART6_SendData(uint8_t data)
{
    USART_SendData(USART6,data);
		while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
}

void UART7_SendData(uint8_t data)
{
    USART_SendData(UART7,data);
		while (USART_GetFlagStatus(UART7, USART_FLAG_TC) == RESET);
}


void UART8_SendData(unsigned char b)
{
		USART_SendData(UART8,b);
    while (USART_GetFlagStatus(UART8,USART_FLAG_TC) == RESET);
}

void USART1_printf (char *fmt, ...)
{
	char buffer[256];
	u8 i = 0;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 256, fmt, arg_ptr);
	while ((i < 255) && buffer[i])
	{
		USART_SendData(USART1, (u8) buffer[i++]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
	va_end(arg_ptr);
}
 
void USART2_printf (char *fmt, ...)
{
	char buffer[256];
	u8 i = 0;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 256, fmt, arg_ptr);
	while ((i < 255) && buffer[i])
	{
		USART_SendData(USART2, (u8) buffer[i++]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
	va_end(arg_ptr);
}

void USART3_printf (char *fmt, ...)
{
	char buffer[256];
	u8 i = 0;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 256, fmt, arg_ptr);
	while ((i < 255) && buffer[i])
	{
		USART_SendData(USART3, (u8) buffer[i++]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
	va_end(arg_ptr);
}

void USART6_printf (char *fmt, ...)
{
	char buffer[256];
	u8 i = 0;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 256, fmt, arg_ptr);
	while ((i < 255) && buffer[i])
	{
		USART_SendData(USART6, (u8) buffer[i++]);
		while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
	}
	va_end(arg_ptr);
}

void UART7_printf (char *fmt, ...)
{
	char buffer[256];
	u8 i = 0;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 256, fmt, arg_ptr);
	while ((i < 255) && buffer[i])
	{
		USART_SendData(UART7, (u8) buffer[i++]);
		while (USART_GetFlagStatus(UART7, USART_FLAG_TC) == RESET);
	}
	va_end(arg_ptr);
}

void UART8_printf (char *fmt, ...)
{
	char buffer[256];
	u8 i = 0;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 256, fmt, arg_ptr);
	while ((i < 255) && buffer[i])
	{
		USART_SendData(UART8, (u8) buffer[i++]);
		while (USART_GetFlagStatus(UART8, USART_FLAG_TC) == RESET);
	}
	va_end(arg_ptr);
}


