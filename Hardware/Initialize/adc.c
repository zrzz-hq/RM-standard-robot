#include "adc.h"

uint16_t Key_ADC;

void ADC1_Init(void)
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); 
	
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);
		
		//温度传感器
		//ADC_TempSensorVrefintCmd(ENABLE);
	
		//OLED五向按键
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
	
    ADC_InitTypeDef ADC_InitStructure;
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1,&ADC_InitStructure);
    
   	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_480Cycles);
		//ADC_RegularChannelConfig(ADC1,ADC_Channel_18,2,ADC_SampleTime_15Cycles);
		
		DMA_InitTypeDef  DMA_InitStructure;
		DMA_InitStructure.DMA_Channel = DMA_Channel_0;                                           //通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);                                //DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Key_ADC;                                   //DMA 存储器0地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                        //存储器到外设模式
		DMA_InitStructure.DMA_BufferSize = 1;                                       //数据传输量 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;               //外设非增量模式
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                        //存储器增量模式
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    //外设数据长度:8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;            //存储器数据长度:8位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                //使用普通模式 
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                          //中等优先级
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                    //存储器突发单次传输
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;            //外设突发单次传输
		DMA_Init(DMA2_Stream0, &DMA_InitStructure);                                     //初始化DMA Stream
		DMA_ClearFlag(DMA2_Stream0,DMA_IT_TC);
		DMA_Cmd(DMA2_Stream0,ENABLE);
		
		ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
		ADC_DMACmd(ADC1, ENABLE);  
		ADC_Cmd(ADC1,ENABLE);
		ADC_SoftwareStartConv(ADC1);	
}

Adc_Key_State_t Oled_Key_Read()
{
		//Key_ADC = get_oled_key_adc();
		Adc_Key_State_t key_res; 
		if(Key_ADC<100)
		{
			key_res = KEY_ENTER;		
		}
		else if(Key_ADC>=800&&Key_ADC<900)
		{
			key_res = KEY_LEFT;	
		}
		else if(Key_ADC>=1700&&Key_ADC<1800)
		{
			key_res = KEY_RIGHT;
		}
		else if(Key_ADC>=2400&&Key_ADC<2500)
		{
			key_res = KEY_UP;
		}
		else if(Key_ADC>=3100&&Key_ADC<3300)
		{
			key_res = KEY_DOWN;
		}
		else
		{
			key_res = KEY_NONE;
		}
		return key_res;
}


