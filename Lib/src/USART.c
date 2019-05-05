#include "USART.h"
#include <stdio.h>
#include "ALL_Includes.h"

#ifdef ITN9
u8 usart_rx_buf[UART_BUF_REG_NUM][UART_BUF_REG_LEN];    //���ջ���,���40���ֽ�
u8 usart_rx_temporary[40]; //���ݱ����ݴ���,����ܹ�����40���ֽ�
u8 usartrxbuf_pagebuf=0;     //��������ܻ����ҳ�루5������
u8 usart_rd_len=0;         //������Ϣ�����ݳ���
u8 usart_rd_lentemp=0;     //������¼�Ѷ�ȡ�����ݳ���
u8 usart_rx_enableflag=0;  //����״̬���    
u8 usart_rx_lenrightflag=0;//���ݳ���У��λ��ȷ��־
u8 usart_rx_successflag=0; //�ɹ����յ�������Ϣ

#endif /* ITN9 */

/**************************
�������ܣ���ʼ��UART
���������SYS_Clk:ϵͳʱ�ӣ�2,4,8,16��
         baud��   ������
�����������
�� �� ֵ����
��    ע����
***************************/
void UART_Init(u8 SYS_Clk, u32 baud)
//{
//    UART1_DeInit();  
//    UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);  
//      
//    UART1_ITConfig(UART1_IT_RXNE, ENABLE);  
//      
//    UART1_Cmd(ENABLE);  
//    enableInterrupts(); 
//
//}
#if 1
{   
    u16 UART_Temp;
    UART_IOConfig();//UART IO���ų�ʼ�� 
    UART1->CR2 = 0;// ��ֹUART���ͺͽ���
    UART1->CR1 = 0;// b5 = 0,����UART  b2 = 0,��ֹУ��
    UART1->CR3 = 0;// b5,b4 = 00,1��ֹͣλ
                            
/************************************************** 
    ���ò����ʣ�����ע�����¼��㣺
    (1) ������дBRR2
    (2) BRR1��ŵ��Ƿ�Ƶϵ���ĵ�11λ����4λ��
    (3) BRR2��ŵ��Ƿ�Ƶϵ���ĵ�15λ����12λ���͵�3λ����0λ
    ������ڲ�����λ9600ʱ����Ƶϵ��=2000000/9600=208
    ��Ӧ��ʮ��������Ϊ00D0��BBR1=0D,BBR2=00
*************************************************/ 
    UART_Temp = SYS_Clk*1000000/baud;
    UART1->BRR2 = (u8)((UART_Temp&0x000F)|((UART_Temp&0xF000)>>8));
    UART1->BRR1 = (u8)((UART_Temp&0x0FF0)>>4);
                                    
#if 0
    UART1->CR2 = 0x2C; // b3 = 1,������
                       // b2 = 1,�������
                       // b5 = 1,������������ж� 

#else
    UART1->CR2 = 0x08; // b3 = 1,������
                       // b2 = 0,���������
                       // b5 = 0,��������������ж� 

#endif
    //enableInterrupts();

}
#endif

/**************************************
�������ܣ���UART3����һ���ַ�
���������ch -- Ҫ���͵��ַ�
�����������
�� �� ֵ����
��    ע����
***************************************/
void UART_SendChar(unsigned char ch)
{
     while((UART1->SR & 0x80) == 0x00);  // �����ͼĴ������գ���ȴ�
     UART1->DR = ch;                     // ��Ҫ���͵��ַ��͵����ݼĴ���  
}

/***********************************
�������ܣ�UART IO�ڳ�ʼ��
�����������
�����������
��   ע���ɣ������ģʽ�£���ͨ���ãң��Ĵ���
         �����������
***********************************/
void UART_IOConfig(void)
{ 
    TXPort->DDR |= TXPin;//���ģʽ
    TXPort->CR1 |= TXPin;//�������   
    
    RXPort->DDR &=~RXPin;//����ģʽ
    RXPort->CR1 &=~RXPin;//��������
}


/***********************************************
��������: �ض���fputc����
��   ע��ʹ��printf���ض���fputc����������
        �޸���General Options �е�Library Configuration
        ��Library Options
***********************************************/
//int fputc(int ch, FILE *f)
//{      
//    while((UART1->SR&0X40)==0);   
//
//    UART1->DR = (u8) ch;   
//    
//    return ch;
//}


int Strlen(unsigned char *pstr)
{
    int cnt= -1;
    while(*pstr != '\0')
    {
        pstr++;
        cnt++;
    }
    return cnt;
}

void Uart1_Sendstr(unsigned char *pstr,...)
{
    int strlen = Strlen(pstr);
    while(strlen-->=0)
    {
        UART_SendChar(*pstr);
        pstr++;
    }
}

#define printch(ch)    UART_SendChar(ch) 

/*
0~32767
*/
void    printdec(int dec)  
{  
    if(dec==0)  
    {  
        printch('0');
        return;  
    }  
    printdec(dec/10);  
    printch( (char)(dec%10 + '0'));  
}  

#ifdef FLOAT_SUPPORT
/*
    x.xxx
    0~32767.998
*/  
void    printflt(double flt)  
{  
    //int icnt = 0;  
    int tmpint = 0;  
      
    tmpint = (int)flt;  
    printdec(tmpint);  
    printch('.');  
    flt = flt - tmpint;  
    tmpint = (int)(flt * 1000);  
    printdec(tmpint);  
}  
#endif
  
void    printstr(char* str)  
{  
    while(*str)  
    {  
        printch(*str++);  
    }  
}  
  
void    printbin(int bin)  
{  
    if(bin == 0)  
    {  
        printstr("0b");  
        return;  
    }  
    printbin(bin/2);  
    printch( (char)(bin%2 + '0'));  
}  
  
void    printhex(int hex)  
{  
    if(hex==0)  
    {  
        printstr("0x");  
        return;  
    }  
    printhex(hex/16);  
    if(hex < 10)  
    {  
        printch((char)(hex%16 + '0'));  
    }  
    else  
    {  
        printch((char)(hex%16 - 10 + 'A' ));  
    }  
}  


void    print(char* fmt, ...)  
{  
#ifdef FLOAT_SUPPORT

    double vargflt = 0;  
#endif
    int  vargint = 0;  
    char* vargpch = NULL;  
    char vargch = 0;  
    char* pfmt = NULL;  
    va_list vp;  
  
    va_start(vp, fmt);  
    pfmt = fmt;  
  
    while(*pfmt)  
    {  
        if(*pfmt == '%')  
        {  
            switch(*(++pfmt))  
            {  
                  
                case 'c':  
                    vargch = va_arg(vp, int);   
                    /*    va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI 
                        mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */  
                    printch(vargch);  
                    break;  
                case 'd':  
                case 'i':  
                    vargint = va_arg(vp, int);  
                    printdec(vargint);  
                    break;  
#ifdef FLOAT_SUPPORT
                case 'f':  
                    vargflt = va_arg(vp, double);  
                    /*    va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI 
                        mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */  
                    printflt(vargflt);  
                    break;  
#endif
                case 's':  
                    vargpch = va_arg(vp, char*);  
                    printstr(vargpch);  
                    break;  
                case 'b':  
                case 'B':  
                    vargint = va_arg(vp, int);  
                    printbin(vargint);  
                    break;  
                case 'x':  
                case 'X':  
                    vargint = va_arg(vp, int);  
                    printhex(vargint);  
                    break;  
                case '%':  
                    printch('%');  
                    break;  
                default:  
                    break;  
            }  
            pfmt++;  
        }  
        else  
        {  
            printch(*pfmt++);  
        }  
    }  
    va_end(vp);  
}  
