#ifndef __DRV_ADC_H
#define __DRV_ADC_H

#include "stm32f0xx.h"

#define N 20
#define a 0.7


//uint16_t ADCConvertedValue[N][2];
//uint16_t AfterFilter[2],OldFilter[2];


extern uint8_t ADCount;
extern uint8_t ADCChannel;
extern uint16_t AdcInterTick;
extern uint8_t  AdcInterFlag;

void ADC_Config(void);
uint16_t sysGetAdcResult(uint8_t adaddress);
uint16_t *Filter(uint16_t *adcout);

extern void IWDG_Feed(void);
//extern void Delay_us(uint32_t nTime);


#endif

