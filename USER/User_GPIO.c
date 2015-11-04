#include "User_GPIO.h"

pTimer LED_flashing_timer;

static	void LED_flashingtime_over_proc(void);

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开启GPIO外设时钟	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  		//使能 GPIOB 时钟	
	
	//设置要控制的GPIO引脚、模式、速率
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	//关闭LED
	GPIO_SetBits(GPIOB, GPIO_Pin_1);	
	
	add_timer(LED_flashing_timer,LED_flashingtime_over_proc,100);
	reload_timer(LED_flashing_timer,100);
	start_timer(LED_flashing_timer);
}

void LED_flashingtime_over_proc(void)
{
	LED_B1(!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_1));

	reload_timer(LED_flashing_timer,100);
	start_timer(LED_flashing_timer);
//	LED_B1(OFF);
}

