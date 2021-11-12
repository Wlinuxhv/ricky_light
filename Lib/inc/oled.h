/*
        OLED(IIC):  VCC        3.3V
            GND        GND
            SCL        D6
            SDA        D7
*/

#ifndef __OLED_H
#define __OLED_H
#include "stm8s.h"
#include "delay.h"

#define NORST_PORT


#define OLED_CMD  0 //д����
#define OLED_DATA 1 //д����
#define OLED_MODE 0

/****************ʱ��*********************/
#define OLED_SCL_PORT  (GPIOD)
#define OLED_SCL_PINS  (GPIO_PIN_1)

/****************����*********************/
#define OLED_SDA_PORT  (GPIOA)
#define OLED_SDA_PINS  (GPIO_PIN_2)

#define OLED_SCL_Clr() GPIO_WriteLow(OLED_SCL_PORT, OLED_SCL_PINS)
#define OLED_SCL_Set() GPIO_WriteHigh(OLED_SCL_PORT, OLED_SCL_PINS)

#define OLED_SDA_Clr() GPIO_WriteLow(OLED_SDA_PORT, OLED_SDA_PINS)
#define OLED_SDA_Set() GPIO_WriteHigh(OLED_SDA_PORT, OLED_SDA_PINS)
#define OLED_SDA_READ() GPIO_ReadInputPin(OLED_SDA_PORT, OLED_SDA_PINS)

#define SDA_OUT_PP() GPIO_Init(OLED_SDA_PORT, OLED_SDA_PINS, GPIO_MODE_OUT_PP_HIGH_FAST) //SDA
#define SDA_IN_PU()  GPIO_Init(OLED_SDA_PORT, OLED_SDA_PINS, GPIO_MODE_OUT_PP_HIGH_FAST) //SDA

static inline void IIC_Delay_us(uint8_t x)
{
    do {
        delay_us();
    } while (x--);
}
#define IIC_Delay_2us() //IIC_Delay_us(2)
#define IIC_Delay_4us() //IIC_Delay_us(4)

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ

#define SIZE 8
#define XLevelL     0x02
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64
//-----------------OLED�˿ڶ���----------------

//OLED�����ú���
void OLED_WR_Byte(u8 dat, u8 cmd);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);
void OLED_ShowChar(u8 x, u8 y, u8 chr);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2);
void OLED_ShowString(u8 x, u8 y, u8 *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x, u8 y, u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
void OLED_ON(void);
void OLED_OFF(void);

#endif




