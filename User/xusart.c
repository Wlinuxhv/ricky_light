/**
  *****************************************************************************
  *                             串口设置相关的函数
  *
  *                       (C) Copyright 2000-2020, ***
  *                            All Rights Reserved
  *****************************************************************************
  *
  * @File    : usart.c
  * @By      : 陈桂东
  * @Version : V1.0
  * @Date    : 2018 / 01 / 19
  *
  *****************************************************************************
  *                                   Update
  * @Version : V1.*
  * @By      : ***
  * @Date    : 20** / ** / **
  * @Brief   : ***
  *
  *****************************************************************************
**/


#include "usart.h"


/**
  *****************************************************************************
  * @Name   : 重定义out_char函数
  *
  * @Brief  : none
  *
  * @Input  : c: 发送字符
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
int putchar(int c)
{
  UART1->DR = (u8)c;  //发送数据
  while ((UART1->SR & UART1_SR_TC) == 0);  //等待发送完毕
  
  return c;
}

/**
  *****************************************************************************
  * @Name   : 串口初始化
  *
  * @Brief  : none
  *
  * @Input  : BaudRate: 波特率
  *           HSE 16MHz实测可行波特率有：19200、38400、57600、76800、115200、128000、230400、256000等
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void UART_Init(u32 BaudRate)
{
  u32 clock_freq = 0;
  
  //管脚初始化
  //GPIOD->DDR |= 1<<5 | 1<<6;  //输出模式
  //GPIOD->CR1 |= 1<<5 | 1<<6;  //推挽输出
  //GPIOD->CR2 |= 1<<5 | 1<<6;  //10MHz
  
  //复位UART1
  (void)UART1->SR;
  (void)UART1->DR;
  
  UART1->BRR2 = UART1_BRR2_RESET_VALUE;
  UART1->BRR1 = UART1_BRR1_RESET_VALUE;
  
  UART1->CR1 = UART1_CR1_RESET_VALUE;
  UART1->CR2 = UART1_CR2_RESET_VALUE;
  UART1->CR3 = UART1_CR3_RESET_VALUE;
  UART1->CR4 = UART1_CR4_RESET_VALUE;
  UART1->CR5 = UART1_CR5_RESET_VALUE;
  
  UART1->GTR = UART1_GTR_RESET_VALUE;
  UART1->PSCR = UART1_PSCR_RESET_VALUE;
  
  //配置UART1
  UART1->CR1 &= ~(UART1_CR1_M | UART1_CR1_PCEN | UART1_CR1_PS);  //8位数据，一个起始位，无奇偶校验
  UART1->CR3 &= ~UART1_CR3_STOP;  //1个停止位
  UART1->BRR1 &= ~UART1_BRR1_DIVM;
  UART1->BRR2 &= ~(UART1_BRR2_DIVM | UART1_BRR2_DIVF);
  //得到时钟频率
  clock_freq = Sys_GetCPUClockFrequency();
  //计算波特率寄存器值
  //先写BRR2再写BRR1
  UART1->BRR2 = (((clock_freq / BaudRate) >> 8) & 0xF0) | ((clock_freq / BaudRate) & 0x0F);
  UART1->BRR1 = ((clock_freq / BaudRate) >> 4) & 0xFF;
  
  UART1->CR2 |= UART1_CR2_TEN | UART1_CR2_REN;  //TXD and RXD
  
  UART1->CR3 &= ~(UART1_CR3_CKEN | UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL);  //关闭SCLK，非同步模式
  
  UART1->CR1 &= ~UART1_CR1_UARTD;  //使能UART1
}
