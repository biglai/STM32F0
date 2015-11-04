#include "User_USART.h"

uint8_t 	usart1_send[BUF_SIZE];			//���ݷ�������
uint8_t 	usart1_receive[BUF_SIZE];		//���ݽ�������

static	uint8_t 	usart1_receive_buf[BUF_SIZE];			//���ջ�������
static	uint8_t 	usart1_send_backup[BUF_SIZE];			//ÿ֡���ݷ�����Ϻ󣬱�������֡
static	__IO uint32_t 	usart1_receive_index = 0;

static  __IO uint32_t  ReSendCount = 0;

static	pTimer usart1_receive_timer;			//��������1�������ʱ��ʱ��
static	pTimer usart1_waiterback_timer;  //�ȴ��ط���ʱ��

static	void USART1_time_over_proc(void);						//���ڵĳ�ʱ�������� 
static	void USART1_backtime_over_proc(void);

 void USART1_Config(unsigned int baudrate)	
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	//��Ӵ���1��ʱ��������ע��usart1_receive_timerover_proc��������ʱ��Ϊ5���� 5*1ms = 5ms��	
	add_timer(usart1_receive_timer,USART1_time_over_proc,RECEIVE_TIME);
	add_timer(usart1_waiterback_timer,USART1_backtime_over_proc,WAITER_BACK_TIME);
	
	usart1_receive_index =0;								//��ʼ������
		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  		//ʹ�� GPIOA ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ�� USART ʱ��

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);		//���� PA9 �ĵڶ����� TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);		//���� PA10 �ĵڶ����� RX	
		
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
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//ʹ�ܴ��ڽ����ж�
	USART_Cmd(USART1, ENABLE);   	
}

void USART1_send_chars(uint8_t * pbuf)	//��������֡����
{
	uint8_t pbuf_len = *(pbuf + 10) + 12;
	uint32_t i = 0;
	uint32_t temp = 0;
	
	for(i = 0; i < pbuf_len-1; i++) //��������������һλ�ĳ�У��͵ĵ�8λ
	{
		temp += *(pbuf + i);
	}
	*(pbuf + pbuf_len - 1) = (uint8_t) temp % 256;
	
	if(pbuf_len >= SEND_LOWRE_LIMIT)
	{		
		for(i = 0; i < pbuf_len; i++)		//��������
		{	
			USART1->TDR = (*(pbuf + i) & (uint16_t)0x01FF);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			usart1_send_backup[i] = *(pbuf + i);	//���ݷ��͵�����
		} 
		
		if(usart1_send_backup[9] >= NEED_BACK_MSG_DOWN && usart1_send_backup[9] <= NEED_BACK_MSG_UP)	//�ж��Ƿ���Ҫ�ظ�
		{
			reload_timer(usart1_waiterback_timer,WAITER_BACK_TIME);
			start_timer(usart1_waiterback_timer);
		}
	}
}

static	void USART1_backtime_over_proc(void)	//�ȴ��ظ���ʱ���ط�����֡
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

static	void USART1_time_over_proc(void)		//����1��ʱ���ֽڳ�ʱ����
{
	usart1_receive_index=0;							//���ջ���������λ�ø�λ
}

void USART1_isr(void)		//����1�жϺ���  �����������֡������ usart1_receive[] ��
{
	uint32_t i = 0;
	uint32_t temp = 0;
	uint8_t SUMValue = 0;
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		reload_timer(usart1_receive_timer,RECEIVE_TIME);//�����ַ��յ���װ��ֵ��������ʱ�������ֽڳ�ʱ���Զ����ö�ʱ����ʱ����
		start_timer(usart1_receive_timer);			

		if(usart1_receive_index >= BUF_SIZE)	//������λ��Խ�磬��ô��ͷ��ʼ���
			usart1_receive_index = 0;
		
		usart1_receive_buf[usart1_receive_index++] = (uint8_t)USART_ReceiveData(USART1);	//�����ַ�����λ�ü�1	
		
		if(usart1_receive_index == 1 && usart1_receive_buf[0] != 0x7E)	usart1_receive_index = 0; //��Ч����֡�� 0x7e ��ʼ
		if(usart1_receive_index >= SEND_LOWRE_LIMIT)
			if(usart1_receive_index == usart1_receive_buf[10] + 12)	
			{
				stop_timer(usart1_receive_timer);
				
				for(i = 0; i < usart1_receive_index-1; i++) //��������������һλ�ĳ�У��͵ĵ�8λ
				{
					temp += usart1_receive_buf[i];
				}
				SUMValue = (uint8_t) temp % 256;
				if(usart1_receive_buf[usart1_receive_index - 1] == SUMValue)
				{
					for(i = 0;i < usart1_receive_index; i++)				//���ջ����������������ݽ������顣��������
							usart1_receive[i] = usart1_receive_buf[i];

					if(usart1_receive[9] > BACK_MSG_DOWN && usart1_receive[9] < BACK_MSG_UP)	//�ж��Ƿ��ǻظ���Ϣ
					{
						ReSendCount = 0;
						stop_timer(usart1_waiterback_timer);													
					}						
				}
				usart1_receive_index=0;							//���ջ���������λ�ø�λ
			}		
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)	//���ش���ֻҪ��״̬�Ĵ��������ݼĴ����Ϳ��Զ�����˱�־
	{
		(void)USART_ReceiveData(USART1);
		usart1_receive_index = 0;							//���ջ���������λ�ø�λ
	}
}

void stop_waiterbackmsg(void)
{
	ReSendCount = 0;
	if(usart1_waiterback_timer->flag == TIMER_FLAG_RUN) 
		stop_timer(usart1_waiterback_timer);	
}


