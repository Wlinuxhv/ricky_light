/*
    GT-24 V1.0 (RF24L01) ���Կ�������

    ���ߣ����ƿƼ����޹�˾
    �Ա�����:https://shop532831232.taobao.com
	
    ����оƬ�� STM8S105    
	
   ��·���ӣ�  RF24L01�� VCC        3.3V
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
						
    ����޸�ʱ�䣻2018��8��1�� 12:00:00
  ������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
*/


#include "spi.h"


//#ifndef __USE_SOFT_SPI_INTERFACE__
/** Ӳ��SPI */

//SPI�ȴ���ʱ
#define SPI_WAIT_TIMEOUT			((uint16_t)0xFFFF)


/**
  * @brief :SPI��ʼ����Ӳ����
  * @param :��
  * @note  :��
  * @retval:��
  */
void drv_spi_init( void )
{
	//SPI�������� SCK MOSI NSS ����Ϊ������� MISO����Ϊ����
	GPIO_Init( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST ); 
	GPIO_Init( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW );
	GPIO_Init( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW  );
	GPIO_Init( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN, GPIO_MODE_IN_PU_NO_IT  );
	
	//SPI��������
	CLK_PeripheralClockConfig( CLK_PERIPHERAL_SPI,ENABLE );		//��SPIʱ��
	SPI_DeInit( );		//SPI��λ
	//SPI�����ʼ��
	SPI_Init( SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_8, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT,SPI_CRC_TX );
	SPI_Cmd( ENABLE );	//SPIʹ��
}

/**
  * @brief :SPI�շ�һ���ֽ�
  * @param :
  *			@TxByte: ���͵������ֽ�
  * @note  :�Ƕ���ʽ��һ���ȴ���ʱ���������Զ��˳�
  * @retval:���յ����ֽ�
  */
uint8_t drv_spi_read_write_byte( uint8_t TxByte )
{
	uint8_t l_Data = 0;
	uint16_t l_WaitTime = 0;
	
	while( RESET == SPI_GetFlagStatus( SPI_FLAG_TXE ) )		//�ȴ����ͻ�������
	{
		if( SPI_WAIT_TIMEOUT == ++l_WaitTime )
		{
			break;			//�ȴ���ʱ
		}
	}
	SPI_SendData( TxByte );	//��������
	l_WaitTime = SPI_WAIT_TIMEOUT / 2;
	while( RESET == SPI_GetFlagStatus( SPI_FLAG_RXNE ) )	//�ȴ����ջ������ǿ�
	{
		if( SPI_WAIT_TIMEOUT == ++l_WaitTime )
		{
			break;			//�ȴ���ʱ
		}
	}
	
	l_Data = SPI_ReceiveData( );
	return l_Data;	//��������
}

/**
  * @brief :SPI�շ��ַ���
  * @param :
  *			@ReadBuffer: �������ݻ�������ַ
  *			@WriteBuffer:�����ֽڻ�������ַ
  *			@Length:�ֽڳ���
  * @note  :�Ƕ���ʽ��һ���ȴ���ʱ���������Զ��˳�
  * @retval:��
  */
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
{
	GPIO_WriteLow( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN);			//Ƭѡ
	while( Length-- )
	{
		*ReadBuffer = drv_spi_read_write_byte( *WriteBuffer );		//һ���ֽڵ������շ�
		ReadBuffer++;
		WriteBuffer++;				//��ַ��1
	}
	 GPIO_WriteHigh( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN);				//ȡ��Ƭѡ
}

/** Ӳ��SPI */
//#endif






//#ifdef __USE_SOFT_SPI_INTERFACE__
///** ����SPI */
//
//
///**
//  * @brief :SPI��ʼ����������
//  * @param :��
//  * @note  :��
//  * @retval:��
//  */
//void drv_spi_init( void )
//{
//	//SPI�������� SCK MOSI NSS ����Ϊ������� MISO����Ϊ����
//	GPIO_Init( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast  );
//	GPIO_Init( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast  );
//	GPIO_Init( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN, GPIO_Mode_Out_PP_High_Slow  );
//	GPIO_Init( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN, GPIO_Mode_In_PU_No_IT  );
//}
//
///**
//  * @brief :SPI�շ�һ���ֽ�
//  * @param :
//  *			@TxByte: ���͵������ֽ�
//  * @note  :�Ƕ���ʽ��һ���ȴ���ʱ���������Զ��˳�
//  * @retval:���յ����ֽ�
//  */
//uint8_t drv_spi_read_write_byte( uint8_t TxByte )
//{
//	uint8_t i = 0, Data = 0;
//	
//	spi_set_clk_low( );
//	
//	/** ���� */
//	for( i = 0; i < 8; i++ )			//һ���ֽ�8λ��ѭ��8��	
//	{
//		
//		if( 0x80 == ( TxByte & 0x80 ))
//		{
//			spi_set_mosi_hight( );		//bit == 1�������ø�
//		}
//		else
//		{
//			spi_set_mosi_low( );		//bit == 0�������õ�
//		}
//		TxByte <<= 1;					//������λ��MSB�ȷ�
//		
//		spi_set_clk_high( );			//ʱ���ø�
//		
//		;
//		
//		/** ���� */
//		Data <<= 1;						//������������
//		if( 1 == spi_get_miso( ))
//		{
//			Data |= 0x01;				//�������״̬Ϊ�ߣ�����Ӧbit = 1
//		}
//		
//		spi_set_clk_low( );				//ʱ���õ�
//		
//		;
//	}
//	
//	return Data;		//���ؽ��յ�������
//}
//
///**
//  * @brief :SPI�շ��ַ���
//  * @param :
//  *			@ReadBuffer: �������ݻ�������ַ
//  *			@WriteBuffer:�����ֽڻ�������ַ
//  *			@Length:�ֽڳ���
//  * @note  :�Ƕ���ʽ��һ���ȴ���ʱ���������Զ��˳�
//  * @retval:��
//  */
//void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
//{
//	spi_set_nss_low( );			//Ƭѡ
//	
//	while( Length-- )
//	{
//		*ReadBuffer = drv_spi_read_write_byte( *WriteBuffer );		//����һ���ֽ�
//		ReadBuffer++;
//		WriteBuffer++;			//��ַ��1
//	}
//	
//	spi_set_nss_high( );		//ȡ��Ƭѡ
//}
//
//
///** ����SPI */
////#endif
//
//