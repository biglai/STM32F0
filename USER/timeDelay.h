#ifndef _TIMEDELAY_H
#define _TIMEDELAY_H
#include "stm32f0xx.h"

void time3_Init(void);
void TIM3_isr(void);

void Systick_Init(void);
void Delay_us(uint16_t us);


#endif
