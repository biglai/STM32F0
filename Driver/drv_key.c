#include "drv_key.h"

__IO static	uint8_t singleKeyCode;
__IO static	uint8_t matrixKeyCode;

static pTimer KEY_scanning_timer;

void KEY_scanningtimer_over_proc(void);

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  		//独立按键，使能 GPIOA 时钟;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);		//矩阵键盘行的时钟
  GPIO_InitStructure.GPIO_Pin =  Row1|Row2|Row3|Row4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIO_Row, &GPIO_InitStructure);
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);		//矩阵键盘列的时钟
  GPIO_InitStructure.GPIO_Pin =  Column1|Column2|Column3|Column4;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_Column, &GPIO_InitStructure);
	
	add_timer(KEY_scanning_timer,KEY_scanningtimer_over_proc,30);
	reload_timer(KEY_scanning_timer,30);
	start_timer(KEY_scanning_timer);	
}

void KEY_DeInit(void)
{
	stop_timer(KEY_scanning_timer);
	remove_timer(KEY_scanning_timer);
}

//矩阵键盘，清除行  row:1--4，从第一行开始
static void ClrRow(uint8_t row)
{
	switch(row)
	{
		case 0x01:	GPIO_ResetBits(GPIO_Row, Row1);
								GPIO_SetBits(GPIO_Row, Row2);
								GPIO_SetBits(GPIO_Row, Row3);
								GPIO_SetBits(GPIO_Row, Row4);
								break;
		case 0x02:	GPIO_SetBits(GPIO_Row, Row1);
								GPIO_ResetBits(GPIO_Row, Row2);
								GPIO_SetBits(GPIO_Row, Row3);
								GPIO_SetBits(GPIO_Row, Row4);
								break;
		case 0x03:	GPIO_SetBits(GPIO_Row, Row1);
								GPIO_SetBits(GPIO_Row, Row2);
								GPIO_ResetBits(GPIO_Row, Row3);
								GPIO_SetBits(GPIO_Row, Row4);
								break;
		case 0x04:	GPIO_SetBits(GPIO_Row, Row1);
								GPIO_SetBits(GPIO_Row, Row2);
								GPIO_SetBits(GPIO_Row, Row3);
								GPIO_ResetBits(GPIO_Row, Row4);
								break;
		case 0xff:	GPIO_ResetBits(GPIO_Row, Row1);
								GPIO_ResetBits(GPIO_Row, Row2);
								GPIO_ResetBits(GPIO_Row, Row3);
								GPIO_ResetBits(GPIO_Row, Row4);
								break;		
		default:		break;
	}
}
//矩阵键盘，读取列，列 上拉输入
static uint8_t ReadColumn(uint8_t line)
{
	uint8_t temp;
	switch(line)
	{
		case 0x01:	temp = GPIO_ReadInputDataBit(GPIO_Column, Column1);
								return temp;
		case 0x02:	temp = GPIO_ReadInputDataBit(GPIO_Column, Column2);
								return temp;
		case 0x03:	temp = GPIO_ReadInputDataBit(GPIO_Column, Column3);
								return temp;
		case 0x04:	temp = GPIO_ReadInputDataBit(GPIO_Column, Column4);
								return temp;		
		case 0xff:	temp = GPIO_ReadInputDataBit(GPIO_Column, Column1);
								temp |= (GPIO_ReadInputDataBit(GPIO_Column, Column2)<<1);
								temp |= (GPIO_ReadInputDataBit(GPIO_Column, Column3)<<2);
								temp |= (GPIO_ReadInputDataBit(GPIO_Column, Column4)<<3);
								return temp;		
		default:		return 0x0f;
	}
}
//矩阵键盘，获取键码，返回 0x23,表示按钮是第2行第3列，行列都从1开始
static uint8_t GetMatrixKeyCode(void)
{
	uint8_t keyNo=0;
	ClrRow(0xff);	//所有行清零
	if(ReadColumn(0xff) != 0x0f)	//有键按下
	{
		for(uint8_t row=1; row<5; row++)
		{
			ClrRow(row);
			switch(ReadColumn(0xff))
			{
				case 	0x0e:	keyNo=0x01|row<<4;	break;	//1110
				case	0x0d:	keyNo=0x02|row<<4;	break;
				case	0x0b:	keyNo=0x03|row<<4;	break;
				case	0x07:	keyNo=0x04|row<<4;	break;
				default:		keyNo=0x00;					break;
			}
		}
		return keyNo;
	}
	else	return 0;
}

void KEY_scanningtimer_over_proc(void)
{
	static uint16_t singleKeyNo = 0;	
	
	if(singleKeyNo == 0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == KEY_DOWN)		//如果按键按下
		singleKeyNo = 1;
		
	else if(singleKeyNo == 1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == KEY_UP)	//干扰信号
		singleKeyNo = 0;
		
//	else if(singleKeyNo == 1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == KEY_DOWN)   //如果不是干扰信号，不等待按键抬起
//	{
//		singleKeyNo = 0;		
//		singleKeyCode = isA6Down;	
//	}	
	else if(singleKeyNo == 1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == KEY_DOWN)   //如果不是干扰信号
	{
		singleKeyNo = 2;		
	}
	else if(singleKeyNo == 2 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == KEY_UP)	//如果不是干扰信号，等待按键抬起
	{
		singleKeyNo = 0;
		singleKeyCode = isA6Down;	
	}
	
//矩阵按钮
	static uint16_t matrixKeyNo = 0;
	static uint8_t newKey = 0, oldKey = 0;
	newKey = GetMatrixKeyCode();
	if(matrixKeyNo == 0 && newKey != 0)		//如果键按下
	{
		matrixKeyNo = 1;
		oldKey = newKey;
	}
	else if(matrixKeyNo == 1 && newKey != oldKey)  //是干扰信号
	{
		matrixKeyNo = 0;
		oldKey = 0;
	}
//	else if(matrixKeyNo == 1 && newKey == oldKey)  //不是干扰信号，不等待按键抬起
//	{
//		matrixKeyNo = 0;
//		oldKey = 0;
//		matrixKeyCode = newKey;
//		newKey = 0;
//	}
	else if(matrixKeyNo == 1 && newKey == oldKey)  //不是干扰信号
	{
		matrixKeyNo = 2;	
	}
	else if(matrixKeyNo == 2 && newKey == 0)  //不是干扰信号，等待按键抬起
	{
		matrixKeyCode = oldKey;
		matrixKeyNo = 0;
		oldKey = 0;
	}
//end 矩阵按钮	
	reload_timer(KEY_scanning_timer,30);
	start_timer(KEY_scanning_timer);
}

/********************************************************************
  *parameter: 
  *return:  返回独立按键的键码，返回0表示没有键按下
  *Description: 
  *History: 
*********************************************************************/
uint8_t GetSingleKey(void)
{
	uint8_t old_singleKeyCode = 0;
	old_singleKeyCode = singleKeyCode;
	singleKeyCode = 0;
	return old_singleKeyCode;
}
/********************************************************************
  *parameter: 
  *return: 返回键码，例如：0x23,表示按钮是第2行第3列，行列都从1开始
  *Description: 无法检测多个键同时按下的情况，键码只返回一次
  *History: 
*********************************************************************/
uint8_t GetMatrixKey(void)
{
	uint8_t old_matrixKeyCode = 0;
	old_matrixKeyCode = matrixKeyCode;
	matrixKeyCode = 0;
	return old_matrixKeyCode;		
}
