#include "led.h"

#include "ALL_Includes.h"

/***********************
�������ܣ���ʼ��LED
�����������
�����������
��    ע����
***********************/
void LED_Init(void)
{
      //����IO��Ϊ���
//    LEDPort->DDR |= LEDPin;//���ģʽ
//    LEDPort->CR1 |= LEDPin;//�������
    GPIO_DeInit(GPIOB);
    GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);
}



/***********************
�������ܣ�LED��
�����������
�����������
��    ע����
***********************/
void LED_ON(void)
{
//    LEDPort->ODR &=~LEDPin;//LED�õ�
    GPIO_WriteLow(GPIOB,GPIO_PIN_5);
}

/***********************
�������ܣ�LED��
�����������
�����������
��    ע����
***********************/
void LED_OFF(void)
{
//    LEDPort->ODR |= LEDPin;//LED�ø�
    GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
}
