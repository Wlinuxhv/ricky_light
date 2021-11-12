/**
  *****************************************************************************
  *                            系统设置相关的函数
  *
  *                       (C) Copyright 2000-2020, ***
  *                            All Rights Reserved
  *****************************************************************************
  *
  * @File    : sys.c
  * @By      : 陈桂东
  * @Version : V1.0
  * @Date    : 2018 / 01 / 19
  *
  *****************************************************************************
**/


#include "sys.h"

/*************** (C) COPYRIGHT  EW工作室 ***************************************
 * 文件名  ：sysclock.c
 * 描述    ：系统时钟设置    
 * 实验平台：EW STM8入门板 V1.0
 * 库版本  ：V2.1.0
 * QQ群	   ：261825684
 * 修改时间：2013-08-20
*******************************************************************************/

/*******************************************************************************
 * 名称: Sysclock_Init，无分频，16MHZ晶振
 * 功能: 设置系统时钟频率
 * 形参: 无
 * 返回: 无
 * 说明: 时钟选择

 ******************************************************************************/
void SystemClock_Init(void)
{
//使用宏定义的写法
#ifdef  HSE_Enable
     while (!CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE,\
            CLK_CURRENTCLOCKSTATE_DISABLE));
     //切换到外部时钟,并等待时钟却换成功
#else
     CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
#endif   

}

/**
  *****************************************************************************
  *                            系统设置相关的函数
  *
  *                       (C) Copyright 2000-2020, ***
  *                            All Rights Reserved
  *****************************************************************************
  *
  * @File    : sys.c
  * @By      : 陈桂东
  * @Version : V1.0
  * @Date    : 2018 / 01 / 19
  *
  *****************************************************************************
**/

/******************************************************************************
                                   定义变量
******************************************************************************/

const u8 HSIDivPrescaler[4] = {1, 2, 4, 8};
const u8 CPUDivPrescaler[8] = {1, 2, 4, 8, 16, 32, 64, 128};

/**
  *****************************************************************************
  * @Name   : 系统时钟初始化，八分频，2MHZ晶振
  *
  * @Brief  : 首先启动的是HSE，当HSE失败自动切换到HSI
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
  
  CLK->ECKR |= CLK_ECKR_HSEEN;  //外部晶振
  //等待HSE准备就绪
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
    CLK->SWCR |= CLK_SWCR_SWEN;  //使能时钟切换
    CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);  //禁止时钟切换中断
    //选择主时钟源
    CLK->SWR  = 0xB4;  //主时钟切换
                       //0xE1：HSI为主时钟源(复位值)
                       //0xD2：LSI为主时钟源(仅当LSI_EN选项位为1时)
                       //0xB4：HSE为主时钟源
    while ((CLK->SWCR & CLK_SWCR_SWBSY) != 0);  //等待时钟切换
    
    //关闭HSI
    CLK->ICKR &= ~CLK_ICKR_HSIEN;
  }
  else  //使用内部HSI
  {
    CLK->ICKR |= CLK_ICKR_HSIEN;  //使能内部高速晶振
    //等待HSI准备就绪
    while ((CLK->ICKR & CLK_ICKR_HSIRDY) == 0);
    
    CLK->SWCR |= CLK_SWCR_SWEN;  //使能时钟切换
    CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);  //禁止时钟切换中断
    //选择主时钟源
    CLK->SWR  = 0xE1;  //主时钟切换
                       //0xE1：HSI为主时钟源(复位值)
                       //0xD2：LSI为主时钟源(仅当LSI_EN选项位为1时)
                       //0xB4：HSE为主时钟源
    while ((CLK->SWCR & CLK_SWCR_SWBSY) != 0);  //等待时钟切换
    
    //关闭HSE
    CLK->ECKR &= ~CLK_ECKR_HSEEN;
  }
}

/**
  *****************************************************************************
  * @Name   : 获取HSI分频系数
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
  
  div = CLK->CKDIVR & CLK_CKDIVR_HSIDIV;  //得到HSI分频系数
  div >>= 3;
  
  return HSIDivPrescaler[div];
}

/**
  *****************************************************************************
  * @Name   : 获取CPU分频系数
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
  
  div = CLK->CKDIVR & CLK_CKDIVR_CPUDIV;  //得到CPU分频系数
  
  return CPUDivPrescaler[div];
}

/**
  *****************************************************************************
  * @Name   : 获取CPU分频系数
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
  
  clk_src = CLK->CMSR;  //得到主时钟状态
  if (clk_src == 0xE1)  //HSI
  {
    return (HSI_VALUE / Sys_GetHSIPrescaler());  //计算得到时钟频率
  }
  else if (clk_src == 0xD2)  //LSI
  {
    return LSI_VALUE;
  }
  else if (clk_src == 0xB4)  //HSE
  {
    return HSE_VALUE;
  }
  
  return (HSI_VALUE / 8);  //默认HSI 8分频
}

/**
  *****************************************************************************
  * @Name   : 系统时钟输出配置
  *
  * @Brief  : 默认输出的是CPU频率，不分频
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
  //初始化管脚
  GPIOC->DDR |= 1<<4;  //输出模式
  GPIOC->CR1 |= 1<<4;  //推挽输出
  GPIOC->CR2 |= 1<<4;  //10MHz
  
  //配置输出时钟
  CLK->CCOR |= (0x04<<1);  //fcpu输出
  CLK->CCOR |= 1<<0;  //使能CCO输出
}

/**
  *****************************************************************************
  * @Name   : 断言错误处理
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
