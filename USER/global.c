/********************************************************************
  *Author:  biglai
  *Version: V0.1
  *Date:    2014/11/15
  *Description: ���Ź����������ʱ����ʼ����ȫ�ֱ���
  *History: 
*********************************************************************/
#include "global.h"

uint32_t unStartFlag;

__IO uint32_t LsiFreq = 40000;

/*  	
	IWDG_Init(IWDG_Prescaler_32,LsiFreq/32); //1s��ʱ
	IWDG_Feed();
*/
void IWDG_Init(uint8_t prer,uint16_t rlr)   //(4,625) 
{
#ifndef  USE_DEBUG
	
 /*ʹ��д����IWDG_PR and IWDG_RLR�Ĵ���*/
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG��������ʱ��: LSI/32 */
  IWDG_SetPrescaler(prer);

  /*�����ص���������ֵ ���250ms��IWDG��ʱ���԰�������ʽ����:  
     Counter Reload Value = 250ms/IWDG counter clock period
                          = 250ms / (LSI/32)
                          = 0.25s / (LsiFreq/32)
                          = LsiFreq/(32 * 4)
                          = LsiFreq/128
   */
  IWDG_SetReload(rlr);

  /* ���µ���IWDG������ */
  IWDG_ReloadCounter();

  /* ʹ��IWDG (LSI ��Ӳ��ʹ��) */
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






