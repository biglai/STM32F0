#include "PLCmodule.h"
#include "User_GPIO.h"

static pTimer Rst_S1650_timer;

__IO static uint32_t RSTing = 0x5A;
void Rst_S1650timer_over_proc(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_5);	//200ms后拉高复位管脚
	RSTing = 0xA5;
}

void PLCS1650_Init(void)
{
	uint8_t SetAID[] = {0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
											0x00,0x05,0x01,0x2A,0x11,0x11,0xCA,0x00};	//设置AID：CA 11 11 2A 数组最后一位为校验位，求和，先置0	
	
	add_timer(Rst_S1650_timer,Rst_S1650timer_over_proc,200);
	start_timer(Rst_S1650_timer);  //A5拉低200ms使模块复位

	while(RSTing != 0xA5)   IWDG_Feed();
	RSTing = 0x5A;

	stop_timer(Rst_S1650_timer);
	remove_timer(Rst_S1650_timer);
											
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	
	usart1_receive[10] = 0x55; usart1_receive[11] = 0x55; 				
	
	while(!(usart1_receive[10] == 1 && usart1_receive[11] == 0))	//等待AID正确设置
	{		
		if(	usart1_receive[10] == 1 && usart1_receive[11] == 5 )									
		{		
			usart1_receive[10] = 0x55; usart1_receive[11] = 0x55; 								
			USART1_send_chars(SetAID);
		}
		IWDG_Feed();		
	}

	stop_timer(LED_flashing_timer);	//AID设置正确，LED停止闪烁
	LED_B1(OFF);
}

