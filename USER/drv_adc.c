#include "drv_adc.h"

/**
  * @brief  ADC Configuration
  * @param  None
  * @retval None
  */

void ADC_Config(void)
{
  ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
  
  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
  /* Configure ADC Channel11 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
   ADC_ChannelConfig(ADC1, ADC_Channel_0|ADC_Channel_1, ADC_SampleTime_239_5Cycles);
  /* Convert the ADC1 Channel 11 with 239.5 Cycles as sampling time */ 
 // ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles);
  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  //ADC_StartOfConversion(ADC1);
  ADC_StopOfConversion(ADC1);
  
}

uint16_t  sysGetAdcResult(uint8_t adaddress)
{
 uint16_t adcresult;
	// 必须先调用ADC_Cmd()
    //timeout=10000;
    // 需要等待一段时间
    //sysDelay(200);
  /* switch (adaddress)
        {
            case 0: ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles); break;
            case 1: ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles); break;
            default:break;
		}*/
    if(adaddress==0)
    {
	 ADC_Cmd(ADC1, ENABLE);
     //  ADC_TempSensorVrefintCmd(ENABLE);
     ADC_StartOfConversion(ADC1);
	// Delay_us(1000);
    }
    while((!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))/*&&(timeout--)*/);//等待转换结束 
    if(adaddress==0)
      /*ADCConvertedValue[ADCount][adaddress]*/adcresult= ADC_GetConversionValue(ADC1)&0xffc;
	else
      /*ADCConvertedValue[ADCount][adaddress]*/adcresult = ADC_GetConversionValue(ADC1); 
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    // ADC_TempSensorVrefintCmd(DISABLE);
    //ADC_StartOfConversion(ADC1, DISABLE);
    if(adaddress==1)
	{
     ADC_StopOfConversion(ADC1);
     ADC_Cmd(ADC1, DISABLE);
	 AdcInterTick=0;
	 AdcInterFlag=0;
    }
   //IWDG_Feed();
   return adcresult;
}

uint16_t *Filter(uint16_t *adcout)
{
 uint8_t i,j,count,channel;
 uint16_t temp;
 uint32_t sum=0,temp1;
 static uint16_t filterout[2];
 static uint16_t filterold[2];
 uint16_t adcoutarry[N][2];
 //uint16_t *pfilter=(uint16_t*)(&filterout[0]);
	
	for(j=0;j<2;j++)
	{
	 for(i=0;i<N;i++)
		 adcoutarry[i][j]=*(adcout+i*2+j);
	}
  for(channel=0;channel<2;channel++)
  {
   for (j=0;j<N-1;j++) 
    { 
      for (i=0;i<N-j-1;i++) 
      { 
         if ( adcoutarry[i][channel]>adcoutarry[i+1][channel] ) 
         { 
            temp = adcoutarry[i][channel]; 
            adcoutarry[i][channel] = adcoutarry[i+1][channel];  
             adcoutarry[i+1][channel] = temp; 
         } 
      } 
   } 
   for(count=1;count<N-1;count++) 
      sum += adcoutarry[count][channel]; 
   temp1= sum/(N-2);  
   filterout[channel] = filterold[channel]*(1-a)+temp1*a;
   //AfterFilter[channel]=temp1;
   sum=0;
   filterold[channel] =filterout[channel];
  }
  return filterout;
 }

