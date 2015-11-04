#ifndef _DRV_KEY_H_
#define _DRV_KEY_H_

#include "stm32f0xx.h"
#include "soft_timer.h"

#define KEY_UP		0		//��������̧��ĵ�ƽ
#define KEY_DOWN	1		//�����������µĵ�ƽ

#define isA6Down 0xA5

#define Row1  				GPIO_Pin_13
#define Row2  				GPIO_Pin_14
#define Row3  				GPIO_Pin_10
#define Row4  				GPIO_Pin_12
#define	GPIO_Row			GPIOB
#define Column1   		GPIO_Pin_5
#define Column2   		GPIO_Pin_6
#define Column3   		GPIO_Pin_7
#define Column4   		GPIO_Pin_4
#define	GPIO_Column		GPIOA
#define KEY_1			0x11
#define KEY_2			0x12
#define KEY_3			0x13
#define KEY_4			0x14
#define KEY_5			0x21
#define KEY_6			0x22
#define KEY_7			0x23
#define KEY_8			0x24
#define KEY_9			0x31
#define KEY_10		0x32
#define KEY_11		0x33
#define KEY_12		0x34
#define KEY_13		0x41
#define KEY_14		0x42
#define KEY_15		0x43
#define KEY_16		0x44

void KEY_GPIO_Config(void);	//�������ӳ�ʼ�����򿪰���ɨ�趨ʱ��
void KEY_DeInit(void);			//�رհ���ɨ�趨ʱ��

uint8_t GetSingleKey(void);
uint8_t GetMatrixKey(void);
								
#endif
