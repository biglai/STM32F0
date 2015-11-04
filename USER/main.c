/********************************************************************
  *Author:  biglai
  *Version: V0.1
  *Date:    2014/11/15
  *Description: 主文件，如果是调试，在选项中加宏定义USE_DEBUG
								如果是release版，取消宏定义USE_DEBUG,使看门狗启动
  *History: 
*********************************************************************/
#include "stm32f0xx.h"
//#include "soft_timer.h"
//#include "User_GPIO.h"
//#include "global.h"

#include "drv_lcd.h"
#include "timeDelay.h"


int main(void)
{
//	pTimer_init(); 				//硬件定时器初始化，systick一毫秒中断，定时器链表复位

//	GPIO_Config();		//端口初始化
	
	bHaveUpdate = FALSE;
	Systick_Init();
	LCD_Initial();
	while(1)
	{
		if(bHaveUpdate == FALSE)
		{
			LCDWriteHZ(5,50,59,0);
			LCDWriteHZ(7,50,60,0);
			LCDWriteASC8_16(9,50,95/10,0); 
			LCDWriteASC8_16(10,50,95%10,0);
			LCDWriteASC8_16(11,50,10,0);
			LCDWriteASC8_16(12,50,46/10,0);
			LCDWriteASC8_16(13,50,46%10,0);
			LCDWriteASC8_16(14,48,28,0);
			LCDWriteASC8_16(15,48,29,0);
			LCDPaintFullScreen((unsigned char *)&LCD_buf[0][0]); 
			
			bHaveUpdate = TRUE;
		}
			
	}
}

//void menuloop(uint8_t key)
//{
// switch(menuindex)
// {
//  case 1:
//  	 if(ALARM_Occured==1)
//  	 	{
//          ALARM_Occured=0;
//		  UpdateData(FALSE);
//	    }
//      menu1(key);
//	  break;
//  case 2:
//      menu2(key);
//	  break;
//  case 3:
//      menu3(key);
//	  break;
//  case 4:
//      menu4(key);
//	  break;
//  case 5:
//      menu5(key);
//	  break;
//  case 6:
//      menu6(key);
//	  break;
//  case 7:
//  	  menu7(key);
//	  break;
//  case 8:
//  	  menu8(key);
//	  break;
//  case 9:
//  	  menu9(key);
//	  break;
//  case 10:
//  	  if(ALARM_Occured==1)
//  	  {
//        ALARM_Occured=0;
//		UpdateData(FALSE);
//	  }
//  	  menu10(key);
//	  break;
//  case 11:
//  	  menu11(key);
//	  break;
//  default:
//	  break;	  
// }
//}

//void menu1(uint8_t key)  //操作界面
//{
//  uint8_t i;
//  RTC_TimeTypeDef RTC_TimeStructure;
////  RTC_DateTypeDef  RTC_DateStructure;
//  //uint8_t year,month,date,hour,minute,second;

//  if(bHaveUpdate==FALSE)
//  {
//   UpdateData(TRUE);
//   //显示时间
//   /* Get the current Date */
//   //RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
//   //year=RTC_DateStructure.RTC_Year;
//   //month=RTC_DateStructure.RTC_Month;
//   //date=RTC_DateStructure.RTC_Date;
//   /* Get the current Time */
//   RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//   hour=RTC_TimeStructure.RTC_Hours;
//   minute=RTC_TimeStructure.RTC_Minutes;
//   second=RTC_TimeStructure.RTC_Seconds;
//   //LCDWriteASC8_12(0,0,2,0);
//   //LCDWriteASC8_12(1,0,0,0);
//   //LCDWriteASC8_12(2,0,year/10,0);
//   //LCDWriteASC8_12(3,0,year%10,0);
//   //LCDWriteASC8_12(4,0,month/10,0);
//   //LCDWriteASC8_12(5,0,month%10,0);
//   //LCDWriteASC8_12(6,0,date/10,0);
//   //LCDWriteASC8_12(7,0,date%10,0);
//   //显示信号强度
//   if(GsmRssi<=5)
//     LCDWriteASC8_12(0,0,22+GsmRssi,0);
//   //显示电池电量
//   if(BatteryPower<=5)
//     LCDWriteASC8_12(1,0,28+BatteryPower,0);
//   //显示传感器电池电量
//   if(NoSensorFlag==0)
//   {
//    LCDWriteASC8_12(3,0,35,0);
//    LCDWriteASC8_12(4,0,35,0);
//   }
//   else
//   {
//    LCDWriteASC8_12(3,0,34,0);
//    if(SensorPower<=5)
//      LCDWriteASC8_12(4,0,28+SensorPower,0);
//   	}	
//   LCDWriteASC8_12(8,0,hour/10,0);
//   LCDWriteASC8_12(9,0,hour%10,0);
//   LCDWriteASC8_12(10,0,10,0);
//   LCDWriteASC8_12(11,0,minute/10,0);
//   LCDWriteASC8_12(12,0,minute%10,0);
//   LCDWriteASC8_12(13,0,10,0);
//   LCDWriteASC8_12(14,0,second/10,0);
//   LCDWriteASC8_12(15,0,second%10,0); 	
//	//显示保压 倒计时时间
//	if(PressMain==2) //保压阶段
//	{
//     LCDWriteHZ(5,24,25,0);
//	 LCDWriteHZ(7,24,26,0);
//	 LCDWriteASC8_16(9,24,(PressCountTime/600+1)/10,0);
//	 LCDWriteASC8_16(10,24,(PressCountTime/600+1)%10,0);
//     LCDWriteHZ(11,24,63,0);
//	 LCDWriteHZ(13,24,64,0);	 
//	}
//	else
//	{
//     LCDWriteHZ(5,24,27,0);
//	 LCDWriteHZ(7,24,27,0);
//	 LCDWriteASC8_16(9,24,26,0);
//	 LCDWriteASC8_16(10,24,26,0);
//	 LCDWriteHZ(11,24,27,0);
//	 LCDWriteHZ(13,24,27,0);	
//	}
//	//显示组别
//	LCDWriteASC8_16(0,50,GroupMain,0);
//   /* if(BroadFunc==0)
//    {
//	 if(AudioInput==1)
//	 {
//	  AudioIC_Speech(18);
//	  AudioInput=0;
//	 }
//    }*/
//	//显示阶段
//	if(StageMain!=0)
//	  //LCDWriteHZ(1,50,14,0); //一
//	  LCDWriteHZ(3,50,13+StageMain,0); //二
//	  //LCDWriteHZ(5,50,16,0); //三
//	else 
//	  LCDWriteHZ(3,50,27,0);	
//    if(PressMain==0)
//    {
//     LCDWriteHZ(6,50,27,0); //
//	 LCDWriteHZ(8,50,27,0); //
//	}
//	else if(PressMain==1)
//	{
//     LCDWriteHZ(6,50,10,0); //增压
//	 LCDWriteHZ(8,50,11,0); //
//	} 	
//	else if(PressMain==2)
//	{
//     LCDWriteHZ(6,50,12,0); //保压
//	 LCDWriteHZ(8,50,13,0); //
//	}
//	//显示当前压力值
//	LCDWriteHZ(5,50,59,0);
//	LCDWriteHZ(7,50,60,0);
//	LCDWriteASC8_16(9,50,CurrentPressH/10,0); 
//	LCDWriteASC8_16(10,50,CurrentPressH%10,0);
//	LCDWriteASC8_16(11,50,10,0);
//	LCDWriteASC8_16(12,50,CurrentPressL/10,0);
//	LCDWriteASC8_16(13,50,CurrentPressL%10,0);
//	LCDWriteASC8_16(14,48,28,0);
//	LCDWriteASC8_16(15,48,29,0);
// LCDPaintFullScreen((unsigned char *)&LCD_buf[0][0]);  
//  } 
//  if(key)
//  {
//   switch(key)
//   {
//	 case KEY_SETUP:
//		 CopyParameter((unsigned char *)&PhoneNumber_Bak[1][0],(unsigned char *)&PhoneNumber[1][0],14);
//        // CopyParameter((unsigned char *)&OrderNumber_Bak[0],(unsigned char *)&OrderNumber[0],12);
//		 menuindex=2;
//		 SetPoint=0;
//		 ClearLCDMemory();
//         bHaveUpdate=FALSE;
//	     break;
//     case KEY_SEND:
//	 	 if(SMSNumber==4)
//	 	 {
//	 	  SendFailSMS=1;
//		  SendSmsFlag=1;
//		  SmsCount=3;
//		  SendSmsStatus0=0;
//	      SMSSucess=0;
//	      SendTime=SendCount;
//	 	 } 
//         bHaveUpdate=FALSE;
//	     break;
//	 case KEY_START:
//          if((StageMain==0)&&(PressMain==0))
//	 	 {
//		   Start=1;
//		   TestResult=NORESULT;
//		   if(BroadFunc==0)
//		      AudioIC_Speech(7);
//         }
//		 bHaveUpdate=FALSE;
//		 break;
//	 case KEY_SEARCH:
//	 	//CopyParameter((unsigned char *)&HDPhoneNumber[0],(unsigned char *)&PhoneNumber[0][0],11);
//	 	if(WritePoint!=0)
//	 	{
//         menuindex=5;
//		 SetPoint=0;
//		 SetMB=0;
//		 bHaveUpdate=FALSE;
//		 ClearLCDMemory();
//		 ReadPoint=WritePoint;
//		 I2C_ReadByte(eeprom_rbuf,RECORDBYTE, RECORDADDR+1+(ReadPoint-1)*RECORDBYTE,DeviceID);
//         for(i=0;i<11;i++)
//	     {
//	      HDPhoneNumber[i]=eeprom_rbuf[i+25];
//	     }
//		 for(i=0;i<12;i++)
//		 {
//          HistoryDataHD[i]=eeprom_rbuf[i+36];
//		 }
//	 	} 
//		break;
//	 case KEY_SURE:
//	 //	for(i=0;i<6;i++)
//	//	  PassWord[i]=0;
//	//	PwdCnt=0;
//     //   menuindex=7;
//	//	SetPoint=1;
//	//	bHaveUpdate=FALSE;
//	//	ClearLCDMemory();
//		break;
//	 //case KEY_RESET:
//     //   HandReset=1;
//     //   while(1);
//	  //  NVIC_SystemReset();
//	 //   break;
//	 case KEY_ONE:
//	 	if(Start==0)
//	 	{
//	 	 GroupMain=1;
//		 if(BroadFunc==0)
//		 {
//		  AudioIC_Speech(4);
//		 } 
//	 	 bHaveUpdate=FALSE;
//	 	}
//		break;
//	 case KEY_TWO:
//        if(Start==0)
//        {
//		 GroupMain=2;
//		 if(BroadFunc==0)
//		 {
//		  AudioIC_Speech(5);
//		 }
//	 	 bHaveUpdate=FALSE;
//        } 
//		break;
//	case KEY_THREE:
//        if(Start==0)
//        {
//		 GroupMain=3;
//		 if(BroadFunc==0)
//		 {
//		  AudioIC_Speech(6);
//		 }
//	 	 bHaveUpdate=FALSE;
//        } 
//		break;	
//	 default:
//		break;
//   }
//  }
// }


