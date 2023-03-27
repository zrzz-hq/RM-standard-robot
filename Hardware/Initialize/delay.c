#include "delay.h"
#include "FreeRTOS.h"  
#include "task.h"  

static uint8_t fac_us = 0;
static uint32_t fac_ms = 0;

static __IO u32 TimingDelay;
 
///**
//  * @brief  ����ϵͳ�δ�ʱ�� SysTick
//  * @param  ��
//  * @retval ��
//  */
//void SysTick_Init(void)
//{
//	/* SystemFrequency / 1000    1ms�ж�һ��
//	 * SystemFrequency / 100000	 10us�ж�һ��
//	 * SystemFrequency / 1000000 1us�ж�һ��
//	 */
//	if (SysTick_Config(SystemCoreClock / 100000))
//	{ 
//		/* Capture error */ 
//		while (1);
//	}
//}

void delay_init(uint32_t TICK_RATE_HZ)
{
    uint32_t reload = 0;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SystemCoreClock / 8000000;
    fac_ms = SystemCoreClock / 8000;

    if (TICK_RATE_HZ == 0)
    {
        TICK_RATE_HZ = 1000;
    }

    reload = SystemCoreClock / TICK_RATE_HZ / 8;
    reload--;

    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = reload;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void delay_us(uint16_t nus)
{
    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = nus * fac_us;
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

void delay_ms(uint16_t nms)
{
    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = nms * fac_ms;
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}




///////**
//////  * @brief   us��ʱ����,10usΪһ����λ
//////  * @param  
//////  *		@arg nTime: delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
//////  * @retval  ��
//////  */
//////void delay_us(__IO u32 nTime)
//////{ 
//////	TimingDelay = nTime;	

//////	while(TimingDelay != 0);
//////}
//////void delay_ms(unsigned int t)
//////{
//////	int i;
//////	for( i=0;i<t;i++)
//////	{
//////        delay_us(100);
//////	}
//////}
/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}




extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
	 {
	 xPortSysTickHandler();
	 }
	TimingDelay_Decrement();
}

