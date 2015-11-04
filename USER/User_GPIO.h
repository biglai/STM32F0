#ifndef _USER_GPIO_H_
#define _USER_GPIO_H_
#include "global.h"
#include "stm32f0xx.h"
#include "soft_timer.h"

#define	ON	0	//LED亮的电平
#define OFF	1	//LED灭的电平


/*	库函数操作	*/
#define	LED_B1(a)	if(a)	GPIO_SetBits(GPIOB, GPIO_Pin_1); \
									else	GPIO_ResetBits(GPIOB, GPIO_Pin_1)

extern pTimer LED_flashing_timer;					

void LED_GPIO_Config(void);

								
#endif
