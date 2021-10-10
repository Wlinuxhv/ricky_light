/*
    GT-24 V1.0 (RF24L01) 测试开发程序

    编者：果云科技有限公司
    淘宝店铺:https://shop532831232.taobao.com
	
    适用芯片： STM8S105    
	
   线路连接：  RF24L01： VCC        3.3V
			GND        GND
			 CE        B3
			IRQ        C4
                        CSN        E5
			SCK        C5
			MOSI        C6
			MISO        C7
						
	    OLED(IIC):  VCC        3.3V
			GND        GND
			SCL        D6
			SDA        D7
						
    最后修改时间；2018年8月1日 12:00:00
  本程序只供学习使用，未经作者许可，不得用于其它任何用途
*/

#ifndef __SPI_H__
#define __SPI_H__


//#ifndef __USE_SOFT_SPI_INTERFACE__

//#endif
#include "stm8s.h"
#include "stm8s_spi.h"
#include "stm8s_clk.h"


//SPI硬件接口IO定义
#define SPI_CLK_GPIO_PORT			GPIOC
#define SPI_CLK_GPIO_PIN			GPIO_PIN_5

#define SPI_MISO_GPIO_PORT			GPIOC
#define SPI_MISO_GPIO_PIN			GPIO_PIN_7

#define SPI_MOSI_GPIO_PORT			GPIOC
#define SPI_MOSI_GPIO_PIN			GPIO_PIN_6

#define SPI_NSS_GPIO_PORT			GPIOA
#define SPI_NSS_GPIO_PIN			GPIO_PIN_3


#define spi_set_nss_high( )			GPIO_WriteHigh( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN )		//NSS置高，取消片选
#define spi_set_nss_low( )			GPIO_WriteLow( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN )	        //NSS置低，片选从设备

//#ifdef __USE_SOFT_SPI_INTERFACE__					
///** 软件模拟SPI操作函数定义 */
//
//#define spi_set_clk_high( )			GPIO_SetBits( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN )		//时钟置高
//#define spi_set_clk_low( )			GPIO_ResetBits( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN )	//时钟置低
//
//#define spi_set_mosi_hight( )		GPIO_SetBits( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN )	//输出置高（主机）
//#define spi_set_mosi_low( )			GPIO_ResetBits( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN )	//输出置低（主机）
//
//#define spi_get_miso( )				( 0 == GPIO_ReadInputDataBit( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN )) ? 0 : 1 // 读取输入状态
//
//#endif
void drv_spi_init( void );
uint8_t drv_spi_read_write_byte( uint8_t TxByte );
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );


#endif

