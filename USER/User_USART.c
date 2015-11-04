#include "User_USART.h"

uint8_t 	usart1_send[BUF_SIZE];			//数据发送数组
uint8_t 	usart1_receive[BUF_SIZE];		//数据接收数组

static	uint8_t 	usart1_receive_buf[BUF_SIZE];			//接收缓冲数组
static	uint8_t 	usart1_send_backup[BUF_SIZE];			//每帧数据发送完毕后，备份数据帧
static	__IO uint32_t 	usart1_receive_index = 0;

static  __IO uint32_t  ReSendCount = 0;

static	pTimer usart1_receive_timer;			//创建串口1的软件超时定时器
static	pTimer usart1_waiterback_timer;  //等待回发定时器

static	void USART1_time_over_proc(void);						//串口的超时函数声明 
static	void USART1_backtime_over_proc(void);

 void USART1_Config(unsigned int baudrate)	
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	//添加串口1定时器至链表，注册usart1_receive_timerover_proc函数，超时设为5（即 5*1ms = 5ms）	
	add_timer(usart1_receive_timer,USART1_time_over_proc,RECEIVE_TIME);
	add_timer(usart1_waiterback_timer,USART1_backtime_over_proc,WAITER_BACK_TIME);
	
	usart1_receive_index =0;								//初始化变量
		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  		//使能 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能 USART 时钟

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);		//设置 PA9 的第二功能 TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);		//设置 PA10 的第二功能 RX	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	USART_ClearFlag(USART1, USART_FLAG_TC);  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//使能串口接收中断
	USART_Cmd(USART1, ENABLE);   	
}

void USART1_send_chars(uint8_t * pbuf)	//发送数据帧函数
{
	uint8_t pbuf_len = *(pbuf + 10) + 12;
	uint32_t i = 0;
	uint32_t temp = 0;
	
	for(i = 0; i < pbuf_len-1; i++) //将发送数组的最后一位改成校验和的低8位
	{
		temp += *(pbuf + i);
	}
	*(pbuf + pbuf_len - 1) = (uint8_t) temp % 256;
	
	if(pbuf_len >= SEND_LOWRE_LIMIT)
	{		
		for(i = 0; i < pbuf_len; i++)		//发送数据
		{	
			USART1->TDR = (*(pbuf + i) & (uint16_t)0x01FF);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			usart1_send_backup[i] = *(pbuf + i);	//备份发送的数据
		} 
		
		if(usart1_send_backup[9] >= NEED_BACK_MSG_DOWN && usart1_send_backup[9] <= NEED_BACK_MSG_UP)	//判断是否需要回复
		{
			reload_timer(usart1_waiterback_timer,WAITER_BACK_TIME);
			start_timer(usart1_waiterback_timer);
		}
	}
}

static	void USART1_backtime_over_proc(void)	//等待回复超时后，重发数据帧
{
		if( ++ReSendCount < 15) 
		{	
			USART1_send_chars(usart1_send_backup);				
		}
		else
		{
			ReSendCount = 0;			
		}				
}

static	void USART1_time_over_proc(void)		//串口1定时器字节超时处理
{
	usart1_receive_index=0;							//接收缓冲区接收位置复位
}

void USART1_isr(void)		//串口1中断函数  接收完的数据帧保存在 usart1_receive[] 中
{
	uint32_t i = 0;
	uint32_t temp = 0;
	uint8_t SUMValue = 0;
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		reload_timer(usart1_receive_timer,RECEIVE_TIME);//若有字符收到，装初值并启动定时器，若字节超时，自动调用定时器超时函数
		start_timer(usart1_receive_timer);			

		if(usart1_receive_index >= BUF_SIZE)	//若接收位置越界，那么重头开始存放
			usart1_receive_index = 0;
		
		usart1_receive_buf[usart1_receive_index++] = (uint8_t)USART_ReceiveData(USART1);	//接收字符，并位置加1	
		
		if(usart1_receive_index == 1 && usart1_receive_buf[0] != 0x7E)	usart1_receive_index = 0; //有效数据帧从 0x7e 开始
		if(usart1_receive_index >= SEND_LOWRE_LIMIT)
			if(usart1_receive_index == usart1_receive_buf[10] + 12)	
			{
				stop_timer(usart1_receive_timer);
				
				for(i = 0; i < usart1_receive_index-1; i++) //将发送数组的最后一位改成校验和的低8位
				{
					temp += usart1_receive_buf[i];
				}
				SUMValue = (uint8_t) temp % 256;
				if(usart1_receive_buf[usart1_receive_index - 1] == SUMValue)
				{
					for(i = 0;i < usart1_receive_index; i++)				//接收缓冲区数据移至数据接收数组。保存数据
							usart1_receive[i] = usart1_receive_buf[i];

					if(usart1_receive[9] > BACK_MSG_DOWN && usart1_receive[9] < BACK_MSG_UP)	//判断是否是回复信息
					{
						ReSendCount = 0;
						stop_timer(usart1_waiterback_timer);													
					}						
				}
				usart1_receive_index=0;							//接收缓冲区接收位置复位
			}		
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)	//过载处理，只要读状态寄存器和数据寄存器就可自动清理此标志
	{
		(void)USART_ReceiveData(USART1);
		usart1_receive_index = 0;							//接收缓冲区接收位置复位
	}
}

void stop_waiterbackmsg(void)
{
	ReSendCount = 0;
	if(usart1_waiterback_timer->flag == TIMER_FLAG_RUN) 
		stop_timer(usart1_waiterback_timer);	
}


