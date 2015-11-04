#ifndef _USER_USART_H_
#define _USER_USART_H_
#include "global.h"
#include "stm32f0xx.h"
#include "soft_timer.h"

#define SEND_LOWRE_LIMIT 		13			//��������һ֡��������13���ֽ�
#define	WAITER_BACK_TIME		700	//��λms���ȴ��ط�ʱ��
#define	RECEIVE_TIME				300		//��λms��һ֡������ÿ���ֽڽ��յ������ʱ��

#define NEED_BACK_MSG_DOWN	0x01
#define NEED_BACK_MSG_UP		0x0f      //֡��� 011x xxxx ���ͷ���Ҫ�ظ�����
#define BACK_MSG_DOWN				0x10
#define BACK_MSG_UP					0x20		 //֡��� 1xxx xxxx �����շ��Ļظ���Ϣ

#define BUF_SIZE 128

extern uint8_t 	usart1_receive[BUF_SIZE];				//���ݽ�������
extern uint8_t 	usart1_send[BUF_SIZE];					//���ݷ�������

//extern uint16_t  ReSendCount;

void USART1_Config(unsigned int baudrate);								//���ڳ�ʼ��
void USART1_send_chars(uint8_t * pbuf);
void USART1_isr(void); 																		//�жϷ�����

void stop_waiterbackmsg(void); //�����ģ��δ�յ��ظ�������ѭ������ָ��״̬��ֹͣѭ������

#endif
