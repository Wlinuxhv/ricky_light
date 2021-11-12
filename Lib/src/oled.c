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
  ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*/
#include "oled.h"
//#include "stdlib.h"
#include "oledfont.h"
#include "stm8s.h"
//#include "delay.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127



//////////////////////////////////////////////////////////////
void IIC_Start(void)
{
    OLED_SDA_Set();//SDA��λ
    OLED_SCL_Set();//SCL��λ
    IIC_Delay_4us();//��ʱ
    OLED_SDA_Clr();//SDA����
    IIC_Delay_4us();//��ʱ
    OLED_SCL_Clr();//SCL����
}

void IIC_Stop()
{
    OLED_SDA_Clr();//SDA����
    OLED_SCL_Set();//SCL��λ
    IIC_Delay_4us();//��ʱ
    OLED_SDA_Set();//SDA��λ
}


//����Ӧ���ź�
void vIIC_Ack()
{
    OLED_SDA_Clr();
    IIC_Delay_4us();
    OLED_SCL_Set();
    IIC_Delay_4us();
    OLED_SCL_Clr();
    IIC_Delay_4us();
}


//������Ӧ��
void vIIC_NAck()
{
    OLED_SDA_Set();
    IIC_Delay_4us();
    OLED_SCL_Set();
    IIC_Delay_4us();
    OLED_SCL_Clr();
    IIC_Delay_4us();
}


#ifdef NEVER
u8 IIC_Wait_Ack()
{
    SDA_IN_PU();//GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_PU_IT);//��SDA��Ϊ����ģʽ��
    OLED_SCL_Set();               //����ʱ���ߡ�
    IIC_Delay_4us();
    if (OLED_SDA_READ() != 0) {
        //�� ��Ӧ��
        OLED_SCL_Clr();
        IIC_Delay_4us();
        SDA_OUT_PP();//GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);//SDA
        return 1;
    } else {
        //�� ��Ӧ��
        OLED_SCL_Clr();
        IIC_Delay_4us();
        SDA_OUT_PP();//GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);//SDA
        return 0;
    }
}
#else
void IIC_Wait_Ack()
{
    OLED_SDA_Set();//SDA��λ
    IIC_Delay_4us();//��ʱ
    OLED_SCL_Set();//SCL��λ
    IIC_Delay_4us();//��ʱ
    OLED_SCL_Clr();//SCL����
    IIC_Delay_4us();//��ʱ
}
#endif /* NEVER */

void Write_IIC_Byte(uint8_t Data)
{
    u8 i;
    for (i = 0; i < 8; i++) { //����8λ��1���ֽ�����
        OLED_SCL_Clr();//SCL����
        if (Data & 0x80) { //��data��8λ�����λ����д��
            OLED_SDA_Set();//����1
        } else {
            OLED_SDA_Clr();//����0
        }
        IIC_Delay_4us();//��ʱ
        OLED_SCL_Set();//SCL��λ
        IIC_Delay_4us();//��ʱ
        OLED_SCL_Clr();//SCL����
        Data <<= 1; //Ҫ������������λ
    }
}

//���պ���
uint8_t uIIC_RecvByte()
{
    uint8_t i, uReceiveByte = 0;
    SDA_IN_PU();//GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_PU_IT);
    for (i = 0; i < 8; i++) {

        uReceiveByte <<= 1;
        IIC_Delay_4us();
        OLED_SCL_Set();//��ʱ����ʱ��ȡ���ݵ�ƽ
        IIC_Delay_4us();
        if (OLED_SDA_READ() != 0) {
            uReceiveByte |= 0x01;
        }
        IIC_Delay_4us();
        OLED_SCL_Clr();
        IIC_Delay_4us();
    }

    SDA_OUT_PP();//GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);
    return uReceiveByte;
}


/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
    IIC_Start();
    Write_IIC_Byte(0x78);            //Slave address,SA0=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00);            //write command
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
    IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
    IIC_Start();
    Write_IIC_Byte(0x78);            //D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40);            //write data
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

void OLED_WR_Byte(u8 dat, u8 cmd)
{
    if (cmd) {
        Write_IIC_Data(dat);

    } else {
        Write_IIC_Command(dat);

    }
}

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f) | 0x01, OLED_CMD);
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);  //����ҳ��ַ��0~7��
        OLED_WR_Byte(0x00, OLED_CMD);      //������ʾλ�á��е͵�ַ
        OLED_WR_Byte(0x10, OLED_CMD);      //������ʾλ�á��иߵ�ַ
        for (n = 0; n < 128; n++)OLED_WR_Byte(0x00, OLED_DATA);
    } //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 16/12
void OLED_ShowChar(u8 x, u8 y, u8 chr)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //�õ�ƫ�ƺ��ֵ
    if (x > Max_Column - 1) {
        x = 0;
        y = y + 2;
    }
    if (SIZE == 16) {
#ifdef SIZE_16
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
#endif /* NEVER */
    } else {
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);

    }
}
//m^n����
u32 oled_pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)result *= m;
    return result;
}
//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ 0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size2 / 2)*t, y, ' ');
                continue;
            } else enshow = 1;

        }
        OLED_ShowChar(x + (size2 / 2)*t, y, temp + '0');
    }
}
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x, u8 y, u8 *chr)
{
    unsigned char j = 0;
    while (chr[j] != '\0') {
        OLED_ShowChar(x, y, chr[j]);
        x += 8;
        if (x > 120) {
            x = 0;
            y += 2;
        }
        j++;
    }
}


#ifdef NEVER
//��ʾ����
void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
    u8 t, adder = 0;
    OLED_Set_Pos(x, y);
    for (t = 0; t < 16; t++) {
        OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for (t = 0; t < 16; t++) {
        OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
        adder += 1;
    }
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for (y = y0; y < y1; y++) {
        OLED_Set_Pos(x0, y);
        for (x = x0; x < x1; x++) {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}
#endif /* NEVER */


//��ʼ��SSD1306
void OLED_Init(void)
{

    GPIO_Init(OLED_SCL_PORT, (GPIO_Pin_TypeDef)OLED_SCL_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(OLED_SDA_PORT, (GPIO_Pin_TypeDef)OLED_SDA_PINS, GPIO_MODE_OUT_OD_LOW_FAST);
    delay_ms(500);
    OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
    OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    OLED_WR_Byte(0xC8, OLED_CMD); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD); //-not offset
    OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
    OLED_WR_Byte(0x40, OLED_CMD); //Set VCOM Deselect Level
    OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD); //
    OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
    OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
    OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
    OLED_Clear();
    OLED_Set_Pos(0, 0);
}


void OLED_ON(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);//���õ�ɱ�
    OLED_WR_Byte(0X14, OLED_CMD);//������ɱ�
    OLED_WR_Byte(0XAF, OLED_CMD);//OLED����
}


void OLED_OFF(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);
    OLED_WR_Byte(0X10, OLED_CMD);
    OLED_WR_Byte(0xAE, OLED_CMD);
}
