#ifndef __DRV_LCD_H
#define __DRV_LCD_H


#include "stm32f0xx.h"


#define  LCD12864_GPIO    GPIOB


#define  RS		     GPIO_Pin_8  //GPIOA  
#define  RW		     GPIO_Pin_11 //GPIOA  
#define  EN		     GPIO_Pin_12 //GPIOA  
//#define  PSB	     GPIO_Pin_13
#define  RST	     GPIO_Pin_14 //GPIOC  
//#define  Busy        GPIO_Pin_15
#define  LCD_GPIO_DAT   GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7 ////GPIOB
#define  LCD_GPIO_CMD	GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_4

#define FALSE  0x00
#define TRUE   0x01

#define    LCD_RS(x)   x ? GPIO_SetBits(GPIOA,RS):  GPIO_ResetBits(GPIOA,RS)
#define    LCD_RW(x)   x ? GPIO_SetBits(GPIOA,RW):  GPIO_ResetBits(GPIOA,RW)
#define    LCD_EN(x)   x ? GPIO_SetBits(GPIOA,EN):  GPIO_ResetBits(GPIOA,EN)
//#define    LCD_PSB(x)  x ? GPIO_SetBits(LCD12864_GPIO,PSB): GPIO_ResetBits(LCD12864_GPIO,PSB)
#define    LCD_RST(x)  x ? GPIO_SetBits(GPIOC,RST): GPIO_ResetBits(GPIOC,RST)


#define  Busy	         0x80  //PBin(7)	  
//#define  LCD_WriteData(x)     {LCD12864_GPIO->BSRR = x<<8 & 0xff00;LCD12864_GPIO->BRR = ((~x)<<8) & 0xff00;}  //高8位的数据
#define  LCD_WriteData(x)   {LCD12864_GPIO->BSRR = x    & 0x00ff;LCD12864_GPIO->BRR = (~x) & 0x00ff;}  //低8位的数据

extern unsigned char LCD_buf[64][16];  //LCD[y][x]  128*64/8
extern unsigned char bHaveUpdate; 
void LCD_InitPort(void);	  //硬件端口初始化
void LCD_Initial(void);// LCD初始化
void WriteDataLCD(unsigned char WDLCD);
void WriteCommandLCD(unsigned char WCLCD);
void ReadStatusLCD(void);
void ClearLCDMemory(void);
void LCDWriteHZ(unsigned int x,unsigned int y,unsigned short HZcode,unsigned char func);
void LCDWriteASC8_12(unsigned int x,unsigned int y,unsigned short ASCcode,unsigned char func);
void LCDPaintFullScreen (unsigned char *pic);
void LCD_Data_Out(void);
void LCD_Data_In(void);
void LCDClear(void);
void LCDWriteASC8_16(unsigned int x,unsigned int y,unsigned short ASCcode,unsigned char func);
void UpdateData(unsigned char b);
void LCDWriteLogo(unsigned int x,unsigned int y,unsigned char func);
void LCDWriteASC6_8(unsigned int x,unsigned int y,unsigned short ASCcode,unsigned char func);  //8*8字符

#endif
