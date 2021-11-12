/**
  *****************************************************************************
  *                            ϵͳ������صĺ���
  *
  *                       (C) Copyright 2000-2020, ***
  *                            All Rights Reserved
  *****************************************************************************
  *
  * @File    : sys.c
  * @By      : �¹�
  * @Version : V1.0
  * @Date    : 2018 / 01 / 19
  *
  *****************************************************************************
**/


#include "sys.h"

/*************** (C) COPYRIGHT  EW������ ***************************************
 * �ļ���  ��sysclock.c
 * ����    ��ϵͳʱ������    
 * ʵ��ƽ̨��EW STM8���Ű� V1.0
 * ��汾  ��V2.1.0
 * QQȺ	   ��261825684
 * �޸�ʱ�䣺2013-08-20
*******************************************************************************/

/*******************************************************************************
 * ����: Sysclock_Init���޷�Ƶ��16MHZ����
 * ����: ����ϵͳʱ��Ƶ��
 * �β�: ��
 * ����: ��
 * ˵��: ʱ��ѡ��

 ******************************************************************************/
void SystemClock_Init(void)
{
//ʹ�ú궨���д��
#ifdef  HSE_Enable
     while (!CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE,\
            CLK_CURRENTCLOCKSTATE_DISABLE));
     //�л����ⲿʱ��,���ȴ�ʱ��ȴ���ɹ�
#else
     CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
#endif   

}

/**
  *****************************************************************************
  *                            ϵͳ������صĺ���
  *
  *                       (C) Copyright 2000-2020, ***
  *                            All Rights Reserved
  *****************************************************************************
  *
  * @File    : sys.c
  * @By      : �¹�
  * @Version : V1.0
  * @Date    : 2018 / 01 / 19
  *
  *****************************************************************************
**/

/******************************************************************************
                                   �������
******************************************************************************/

const u8 HSIDivPrescaler[4] = {1, 2, 4, 8};
const u8 CPUDivPrescaler[8] = {1, 2, 4, 8, 16, 32, 64, 128};

/**
  *****************************************************************************
  * @Name   : ϵͳʱ�ӳ�ʼ�����˷�Ƶ��2MHZ����
  *
  * @Brief  : ������������HSE����HSEʧ���Զ��л���HSI
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void Sys_ClockInit(void)
{
  u32 StartUpCounter = 0, HSEStatus = 0;
  
  CLK->ECKR |= CLK_ECKR_HSEEN;  //�ⲿ����
  //�ȴ�HSE׼������
  do
  {
    HSEStatus = CLK->ECKR & CLK_ECKR_HSERDY;
    StartUpCounter++;
  } while ((HSEStatus == 0) && (StartUpCounter != 0x5000));
  
  if ((CLK->ECKR & CLK_ECKR_HSERDY) != 0)
  {
    HSEStatus = 1;
  }
  else
  {
    HSEStatus = 0;
  }
  
  if (HSEStatus != 0)  //HSE
  {
    CLK->SWCR |= CLK_SWCR_SWEN;  //ʹ��ʱ���л�
    CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);  //��ֹʱ���л��ж�
    //ѡ����ʱ��Դ
    CLK->SWR  = 0xB4;  //��ʱ���л�
                       //0xE1��HSIΪ��ʱ��Դ(��λֵ)
                       //0xD2��LSIΪ��ʱ��Դ(����LSI_ENѡ��λΪ1ʱ)
                       //0xB4��HSEΪ��ʱ��Դ
    while ((CLK->SWCR & CLK_SWCR_SWBSY) != 0);  //�ȴ�ʱ���л�
    
    //�ر�HSI
    CLK->ICKR &= ~CLK_ICKR_HSIEN;
  }
  else  //ʹ���ڲ�HSI
  {
    CLK->ICKR |= CLK_ICKR_HSIEN;  //ʹ���ڲ����پ���
    //�ȴ�HSI׼������
    while ((CLK->ICKR & CLK_ICKR_HSIRDY) == 0);
    
    CLK->SWCR |= CLK_SWCR_SWEN;  //ʹ��ʱ���л�
    CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);  //��ֹʱ���л��ж�
    //ѡ����ʱ��Դ
    CLK->SWR  = 0xE1;  //��ʱ���л�
                       //0xE1��HSIΪ��ʱ��Դ(��λֵ)
                       //0xD2��LSIΪ��ʱ��Դ(����LSI_ENѡ��λΪ1ʱ)
                       //0xB4��HSEΪ��ʱ��Դ
    while ((CLK->SWCR & CLK_SWCR_SWBSY) != 0);  //�ȴ�ʱ���л�
    
    //�ر�HSE
    CLK->ECKR &= ~CLK_ECKR_HSEEN;
  }
}

/**
  *****************************************************************************
  * @Name   : ��ȡHSI��Ƶϵ��
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
u8 Sys_GetHSIPrescaler(void)
{
  u8 div = 0;
  
  div = CLK->CKDIVR & CLK_CKDIVR_HSIDIV;  //�õ�HSI��Ƶϵ��
  div >>= 3;
  
  return HSIDivPrescaler[div];
}

/**
  *****************************************************************************
  * @Name   : ��ȡCPU��Ƶϵ��
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
u8 Sys_GetCPUPrescaler(void)
{
  u8 div = 0;
  
  div = CLK->CKDIVR & CLK_CKDIVR_CPUDIV;  //�õ�CPU��Ƶϵ��
  
  return CPUDivPrescaler[div];
}

/**
  *****************************************************************************
  * @Name   : ��ȡCPU��Ƶϵ��
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
u32 Sys_GetCPUClockFrequency(void)
{
  u8 clk_src = 0;
  
  clk_src = CLK->CMSR;  //�õ���ʱ��״̬
  if (clk_src == 0xE1)  //HSI
  {
    return (HSI_VALUE / Sys_GetHSIPrescaler());  //����õ�ʱ��Ƶ��
  }
  else if (clk_src == 0xD2)  //LSI
  {
    return LSI_VALUE;
  }
  else if (clk_src == 0xB4)  //HSE
  {
    return HSE_VALUE;
  }
  
  return (HSI_VALUE / 8);  //Ĭ��HSI 8��Ƶ
}

/**
  *****************************************************************************
  * @Name   : ϵͳʱ���������
  *
  * @Brief  : Ĭ���������CPUƵ�ʣ�����Ƶ
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void Sys_ClockOutConfig(void)
{
  //��ʼ���ܽ�
  GPIOC->DDR |= 1<<4;  //���ģʽ
  GPIOC->CR1 |= 1<<4;  //�������
  GPIOC->CR2 |= 1<<4;  //10MHz
  
  //�������ʱ��
  CLK->CCOR |= (0x04<<1);  //fcpu���
  CLK->CCOR |= 1<<0;  //ʹ��CCO���
}

/**
  *****************************************************************************
  * @Name   : ���Դ�����
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  printf("\n\r Wrong parameter value detected on\r\n");
  printf("file  %s\r\n", file);
  printf("line  %d\r\n", line);
  
  /* Infinite loop */
  while (1)
  {}
}
#else
void __aeabi_assert(const char * x1, const char * x2, int x3)
{
}
#endif
