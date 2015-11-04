/**
  ******************************************************************************
  * @file    drv_lcd.c 
  * @author  MCD Application Team
  * @version V1.0
  * @date    05/28/2014
  * @brief   driver for lcd
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "drv_lcd.h"
#include "Font.h"
#include "timeDelay.h"

unsigned char LCD_buf[64][16];  //LCD[y][x]  128*64/8
unsigned char bHaveUpdate; 

void LCD_InitPort(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	//����ṹ��		
	
	//RCC_APB2PeriphClockCmd(GPIOCLK|RCC_APB2Periph_AFIO, ENABLE);  //ʹ�ܹ��ܸ���IOʱ�ӣ�����������ʱ�Ӳ�����ʾ
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);    //�ѵ���������ͨIO��

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = LCD_GPIO_DAT;		//���ݿ����óɿ�©���ģʽ����ģʽ�¶�����Ĵ�����ֵ�õ�IO��״̬
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //��©��� 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;  
	GPIO_Init(LCD12864_GPIO , &GPIO_InitStructure);    //IO�ڳ�ʼ��������ʹ���������ã�
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = LCD_GPIO_CMD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   // 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3; 
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);	
	GPIO_InitStructure.GPIO_Pin  = RST;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   // 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3; 
	GPIO_Init(GPIOC , &GPIO_InitStructure); 	  
}


//д����
void WriteDataLCD(unsigned char WDLCD)
{
	ReadStatusLCD(); //���æ 
	LCD_RS(1);
	LCD_RW(0);
	LCD_EN(0);
	Delay_us(1);
	LCD_WriteData(WDLCD);
	LCD_EN(1);
	Delay_us(1);
	LCD_EN(0);
}

//дָ��
void WriteCommandLCD(unsigned char WCLCD) //BuysCΪ0ʱ����æ���
{
 ReadStatusLCD(); //������Ҫ���æ 
 LCD_RS(0);
 LCD_RW(0);
 LCD_EN(0);
 Delay_us(1);
 LCD_WriteData(WCLCD);
 LCD_EN(1);
 Delay_us(1);
 LCD_EN(0); 
}

//������
//unsigned char ReadDataLCD(void)
//{
// LCD_RS = 1; 
// LCD_RW = 1;
// LCD_E = 0;
// LCD_E = 0;
// LCD_E = 1;
// return(LCD_Data);
//}

//��״̬
void ReadStatusLCD(void)
{ 
 unsigned char  lcd_status;
 
 LCD_RS(0);
 LCD_RW(1); 
 LCD_Data_In();
 do
 {
  LCD_EN(1);
  Delay_us(1);
  lcd_status=GPIO_ReadInputData( LCD12864_GPIO);
  LCD_EN(0);
  }
  while (lcd_status&Busy); //���æ�ź�
  LCD_Data_Out(); 
}

void LCD_Initial(void)
{
 LCD_InitPort();  
 Delay_us(45000);
 //LCD_PSB(1);   //���ڷ�ʽ
 LCD_RST(1);
 Delay_us(100);
 LCD_RST(0);   //Һ����λ	�͵�ƽ��Ч
 Delay_us(60000);                  
 LCD_RST(1);  	//�øߵ�ƽ�ȴ���λ
// Delay_us(10000);   
   
 Delay_us(60000);
 WriteCommandLCD(0x30); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
 Delay_us(1000);
 WriteCommandLCD(0x30); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
 Delay_us(1000);
// WriteCommandLCD(0x02); //��ʾ����
 WriteCommandLCD(0x01); // ��ʾ�����������
 Delay_us(15000);
 WriteCommandLCD(0x06); // ��ʾ����ƶ�����
 Delay_us(10000);
 WriteCommandLCD(0x0C); // ��ʾ�����������
 Delay_us(1000);
 //WriteCommandLCD(0x80); // ��ʾ�����������
}

void	ClearLCDMemory(void)
{
  unsigned int i,j;	
  for(i=0;i<64;i++)
   {
	 for(j=0;j<16;j++)
	    LCD_buf[i][j]=0;
	}
}
	
void 	LCDWriteHZ(unsigned int x,unsigned int y,unsigned short HZcode,unsigned char func)//14*14 ����
{
	unsigned int j;
	unsigned char i;
	unsigned char *pHZlib;
	unsigned char *pLcd;
	pLcd=(unsigned char *)(&LCD_buf[y][x]);
	pHZlib=(unsigned char *)(&GB2312[0]+HZcode*28/*(HZcode<<5)*/);
	for(j=0;j<14;j++)
	{		
		i=*pHZlib++;		
		if(func==1)
		{
			i^=0xff;
		}			
		*pLcd=i;
		i=*pHZlib++;		
		if(func==1)
		{
			i^=0xff;
		}			
		*(pLcd+1)=i;
		pLcd+=16;
	}
}

void 	LCDWriteASC8_12(unsigned int x,unsigned int y,unsigned short ASCcode,unsigned char func)  //8*8�ַ�
{
	unsigned int j;
	unsigned char i;
	unsigned char *pASClib;
	unsigned char *pLcd;
	unsigned int ASC;
	pLcd=(unsigned char *)(&LCD_buf[y][x]);	
	ASC=ASCcode;
	pASClib=(unsigned char *)(&ASCtable8_12[0]+ASC*12/*(ASC<<4)*/);
	for(j=0;j<12;j++)
	{		
		i=*pASClib++;		
		if(func==1)
		{
			i^=0xff;
		}			
		*pLcd=i;		
		pLcd+=16;
	}
}

void 	LCDWriteASC8_16(unsigned int x,unsigned int y,unsigned short ASCcode,unsigned char func)  //8*8�ַ�
{
	unsigned int j;
	unsigned char i;
	unsigned char *pASClib;
	unsigned char *pLcd;
	unsigned int ASC;
	pLcd=(unsigned char *)(&LCD_buf[y][x]);	
	ASC=ASCcode;
	pASClib=(unsigned char *)(&ASCtable8_16[0]+ASC*16/*(ASC<<4)*/);
	for(j=0;j<16;j++)
	{		
		i=*pASClib++;		
		if(func==1)
		{
			i^=0xff;
		}			
		*pLcd=i;		
		pLcd+=16;
	}
}

//ͼ����ʾ128*64
void LCDPaintFullScreen (unsigned char *pic)
{
	unsigned char x,y,i;
	unsigned int tmp=0;
	for(i=0;i<9;){		//���������ϰ������°�������Ϊ��ʼ��ַ��ͬ����Ҫ�ֿ�
	for(x=0;x<32;x++){				//32��
		WriteCommandLCD(0x36);
		//Delay_us(200);
		WriteCommandLCD((0x80+x));//�е�ַ
		//Delay_us(200);
		WriteCommandLCD((0x80+i));	//�е�ַ���°������������е�ַ0X88 
		//Delay_us(200);
		//WriteCommandLCD(0x30);
		//Delay_us(200);
		for(y=0;y<16;y++)
		 {
			WriteDataLCD(pic[tmp+y]);//��ȡ����д��LCD
			//Delay_us(200);
		 }
		tmp+=16;		
	}
	i+=8;
	}
	//Delay_us(200);
	//WriteCommandLCD(0x36);	//���书���趨
	//Delay_us(200);
	WriteCommandLCD(0x30);
	//Delay_us(200);
}

void LCDClear(void) //����
{
  Delay_us(1000);
	WriteCommandLCD(0x01); //��ʾ����
  Delay_us(1000);
  WriteCommandLCD(0x34); // ��ʾ����ƶ�����
  Delay_us(1000);
	WriteCommandLCD(0x30); // ��ʾ�����������
  Delay_us(1000);
}

void	UpdateData(unsigned char b)
{
	if(b==FALSE)
		bHaveUpdate=FALSE;
	else
		bHaveUpdate=TRUE;	
}

void LCD_Data_Out(void)
{   
    GPIO_InitTypeDef  GPIO_InitStructure; 
    /* GPIOC �ⲿʱ��ʹ�� */
   // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); // LCD �����߶˿�ʱ��ʹ��    
	
    GPIO_InitStructure.GPIO_Pin   =  LCD_GPIO_DAT ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT ;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3; // �ٶ�����Ϊ50MHZ
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    
    GPIO_Init(LCD12864_GPIO, &GPIO_InitStructure);
}

void LCD_Data_In(void)
{   
    GPIO_InitTypeDef  GPIO_InitStructure; 
    /* GPIOC �ⲿʱ��ʹ�� */
   // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); // LCD �����߶˿�ʱ��ʹ��    
	
    GPIO_InitStructure.GPIO_Pin   = LCD_GPIO_DAT ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3; // �ٶ�����Ϊ50MHZ
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    
    GPIO_Init(LCD12864_GPIO, &GPIO_InitStructure);
}

void 	LCDWriteLogo(unsigned int x,unsigned int y,unsigned char func)  //logo
{
	unsigned int j,k;
	unsigned char i;
	unsigned char *pASClib;
	unsigned char *pLcd;

	pLcd=(unsigned char *)(&LCD_buf[y][x]);	
	pASClib=(unsigned char *)(&logopic[0]);
	for(j=0;j<27;j++)
	{		
     for(k=0;k<5;k++)
      {
		i=*pASClib++;		
		if(func==1)
		{
			i^=0xff;
		}			
		*(pLcd+k)=i;		
     }	
	 pLcd+=16;
	}
}

void 	LCDWriteASC6_8(unsigned int x,unsigned int y,unsigned short ASCcode,unsigned char func)  //8*8�ַ�
{
	unsigned int j;
	unsigned char i;
	unsigned char *pASClib;
	unsigned char *pLcd;
	unsigned int ASC;
	pLcd=(unsigned char *)(&LCD_buf[y][x]);	
	ASC=ASCcode;
	pASClib=(unsigned char *)(&ASCtable6_8[0]+ASC*12/*(ASC<<4)*/);
	for(j=0;j<8;j++)
	{		
		i=*pASClib++;		
		if(func==1)
		{
			i^=0xff;
		}			
		*pLcd=i;		
		pLcd+=16;
	}
}
