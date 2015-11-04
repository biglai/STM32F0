/********************************************************************
  *Author:  biglai
  *Version: V0.1
  *Date:    2014/11/15
  *Description: 看门狗程序，软件定时器初始化，全局变量
  *History: 
*********************************************************************/
#include "global.h"

uint32_t unStartFlag;

__IO uint32_t LsiFreq = 40000;

/*  	
	IWDG_Init(IWDG_Prescaler_32,LsiFreq/32); //1s超时
	IWDG_Feed();
*/
void IWDG_Init(uint8_t prer,uint16_t rlr)   //(4,625) 
{
#ifndef  USE_DEBUG
	
 /*使能写访问IWDG_PR and IWDG_RLR寄存器*/
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG计数器的时钟: LSI/32 */
  IWDG_SetPrescaler(prer);

  /*设置重导计数器的值 获得250ms的IWDG超时可以按下面算式计算:  
     Counter Reload Value = 250ms/IWDG counter clock period
                          = 250ms / (LSI/32)
                          = 0.25s / (LsiFreq/32)
                          = LsiFreq/(32 * 4)
                          = LsiFreq/128
   */
  IWDG_SetReload(rlr);

  /* 重新导入IWDG计数器 */
  IWDG_ReloadCounter();

  /* 使能IWDG (LSI 被硬件使能) */
  IWDG_Enable();
	
#endif
}

void IWDG_Feed(void)
{
#ifndef  USE_DEBUG	
	
 IWDG_ReloadCounter();
	
#endif	
}

void pTimer_init(void)
{
	extern void soft_timer_list_reset(void);  //soft_timer.c
	
  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config((SystemCoreClock) / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
  NVIC_SetPriority(SysTick_IRQn, 0x0);	
	
	soft_timer_list_reset();
}






