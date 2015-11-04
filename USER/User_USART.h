#ifndef _USER_USART_H_
#define _USER_USART_H_
#include "global.h"
#include "stm32f0xx.h"
#include "soft_timer.h"

#define SEND_LOWRE_LIMIT 		13			//发送数据一帧不能少于13个字节
#define	WAITER_BACK_TIME		700	//单位ms，等待回发时间
#define	RECEIVE_TIME				300		//单位ms，一帧数据内每个字节接收的最大间隔时间

#define NEED_BACK_MSG_DOWN	0x01
#define NEED_BACK_MSG_UP		0x0f      //帧序号 011x xxxx 发送方需要回复报文
#define BACK_MSG_DOWN				0x10
#define BACK_MSG_UP					0x20		 //帧序号 1xxx xxxx ：接收方的回复信息

#define BUF_SIZE 128

extern uint8_t 	usart1_receive[BUF_SIZE];				//数据接收数组
extern uint8_t 	usart1_send[BUF_SIZE];					//数据发送数组

//extern uint16_t  ReSendCount;

void USART1_Config(unsigned int baudrate);								//串口初始化
void USART1_send_chars(uint8_t * pbuf);
void USART1_isr(void); 																		//中断服务函数

void stop_waiterbackmsg(void); //如果主模块未收到回复，处于循环发送指令状态，停止循环发送

#endif
