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


#include "spi.h"


//#ifndef __USE_SOFT_SPI_INTERFACE__
/** 硬件SPI */

//SPI等待超时
#define SPI_WAIT_TIMEOUT			((uint16_t)0xFFFF)


/**
  * @brief :SPI初始化（硬件）
  * @param :无
  * @note  :无
  * @retval:无
  */
void drv_spi_init( void )
{
	//SPI引脚配置 SCK MOSI NSS 配置为推挽输出 MISO配置为输入
	GPIO_Init( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST ); 
	GPIO_Init( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW );
	GPIO_Init( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW  );
	GPIO_Init( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN, GPIO_MODE_IN_PU_NO_IT  );
	
	//SPI外设配置
	CLK_PeripheralClockConfig( CLK_PERIPHERAL_SPI,ENABLE );		//开SPI时钟
	SPI_DeInit( );		//SPI复位
	//SPI外设初始化
	SPI_Init( SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_8, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT,SPI_CRC_TX );
	SPI_Cmd( ENABLE );	//SPI使能
}

/**
  * @brief :SPI收发一个字节
  * @param :
  *			@TxByte: 发送的数据字节
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:接收到的字节
  */
uint8_t drv_spi_read_write_byte( uint8_t TxByte )
{
	uint8_t l_Data = 0;
	uint16_t l_WaitTime = 0;
	
	while( RESET == SPI_GetFlagStatus( SPI_FLAG_TXE ) )		//等待发送缓冲区空
	{
		if( SPI_WAIT_TIMEOUT == ++l_WaitTime )
		{
			break;			//等待超时
		}
	}
	SPI_SendData( TxByte );	//发送数据
	l_WaitTime = SPI_WAIT_TIMEOUT / 2;
	while( RESET == SPI_GetFlagStatus( SPI_FLAG_RXNE ) )	//等待接收缓冲区非空
	{
		if( SPI_WAIT_TIMEOUT == ++l_WaitTime )
		{
			break;			//等待超时
		}
	}
	
	l_Data = SPI_ReceiveData( );
	return l_Data;	//返回数据
}

/**
  * @brief :SPI收发字符串
  * @param :
  *			@ReadBuffer: 接收数据缓冲区地址
  *			@WriteBuffer:发送字节缓冲区地址
  *			@Length:字节长度
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:无
  */
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
{
	GPIO_WriteLow( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN);			//片选
	while( Length-- )
	{
		*ReadBuffer = drv_spi_read_write_byte( *WriteBuffer );		//一个字节的数据收发
		ReadBuffer++;
		WriteBuffer++;				//地址加1
	}
	 GPIO_WriteHigh( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN);				//取消片选
}

/** 硬件SPI */
//#endif






//#ifdef __USE_SOFT_SPI_INTERFACE__
///** 软件SPI */
//
//
///**
//  * @brief :SPI初始化（软件）
//  * @param :无
//  * @note  :无
//  * @retval:无
//  */
//void drv_spi_init( void )
//{
//	//SPI引脚配置 SCK MOSI NSS 配置为推挽输出 MISO配置为输入
//	GPIO_Init( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast  );
//	GPIO_Init( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast  );
//	GPIO_Init( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN, GPIO_Mode_Out_PP_High_Slow  );
//	GPIO_Init( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN, GPIO_Mode_In_PU_No_IT  );
//}
//
///**
//  * @brief :SPI收发一个字节
//  * @param :
//  *			@TxByte: 发送的数据字节
//  * @note  :非堵塞式，一旦等待超时，函数会自动退出
//  * @retval:接收到的字节
//  */
//uint8_t drv_spi_read_write_byte( uint8_t TxByte )
//{
//	uint8_t i = 0, Data = 0;
//	
//	spi_set_clk_low( );
//	
//	/** 发送 */
//	for( i = 0; i < 8; i++ )			//一个字节8位，循环8次	
//	{
//		
//		if( 0x80 == ( TxByte & 0x80 ))
//		{
//			spi_set_mosi_hight( );		//bit == 1，口线置高
//		}
//		else
//		{
//			spi_set_mosi_low( );		//bit == 0，口线置低
//		}
//		TxByte <<= 1;					//向右移位，MSB先发
//		
//		spi_set_clk_high( );			//时钟置高
//		
//		;
//		
//		/** 接收 */
//		Data <<= 1;						//接收数据右移
//		if( 1 == spi_get_miso( ))
//		{
//			Data |= 0x01;				//如果口线状态为高，则相应bit = 1
//		}
//		
//		spi_set_clk_low( );				//时钟置低
//		
//		;
//	}
//	
//	return Data;		//返回接收到的数据
//}
//
///**
//  * @brief :SPI收发字符串
//  * @param :
//  *			@ReadBuffer: 接收数据缓冲区地址
//  *			@WriteBuffer:发送字节缓冲区地址
//  *			@Length:字节长度
//  * @note  :非堵塞式，一旦等待超时，函数会自动退出
//  * @retval:无
//  */
//void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
//{
//	spi_set_nss_low( );			//片选
//	
//	while( Length-- )
//	{
//		*ReadBuffer = drv_spi_read_write_byte( *WriteBuffer );		//交换一个字节
//		ReadBuffer++;
//		WriteBuffer++;			//地址加1
//	}
//	
//	spi_set_nss_high( );		//取消片选
//}
//
//
///** 软件SPI */
////#endif
//
//
