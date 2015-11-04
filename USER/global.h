#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "stm32f0xx.h"

extern uint32_t unStartFlag;


extern __IO uint32_t LsiFreq;
																
void IWDG_Init(uint8_t prer,uint16_t rlr);
void IWDG_Feed(void);

void pTimer_init(void);


#endif
