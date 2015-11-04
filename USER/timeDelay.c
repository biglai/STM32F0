#include "timeDelay.h"


	
//	uint16_t PrescalerValue;

//	uint32_t timeNo;
/* Compute the prescaler value */

uint16_t TimingDelay=0;

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
  
}

void Systick_Init(void)
{
  if (SysTick_Config(SystemCoreClock / 1000000))//1us
  { 
    /* Capture error */ 
    while (1);
  }
}

void Delay_us(uint16_t nTime)
{ 
	TimingDelay = nTime;

  while(TimingDelay != 0);
}




//void Delay_us(uint16_t nTime)
//{
//	TimingDelay = nTime;
//	TIM_Cmd(TIM3, ENABLE);
//	
//	while(TimingDelay != 0)	;
//	
//	TIM_Cmd(TIM3, DISABLE);	
//}


void time3_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	PrescalerValue = (uint16_t) (SystemCoreClock  / 6000000) - 1;
	
	

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
  /* Time base configuration */	
  TIM_TimeBaseStructure.TIM_Period = 48;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
//  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	 
  /* TIM Interrupts enable */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, DISABLE);
	
}

void TIM3_isr(void)
{
	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    /* LED1 toggling with frequency = 73.24 Hz */
		TimingDelay--;
  }
}

