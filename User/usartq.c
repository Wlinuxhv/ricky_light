#include "sys.h"
#include "usart.h"

u8 usart_rx_buf[5][40];    //���ջ���,���40���ֽ�
u8 usart_rx_temporary[40]; //���ݱ����ݴ���,����ܹ�����40���ֽ�
u8 usartrxbuf_pagebuf = 0;   //��������ܻ����ҳ�루5������
u8 usart_rd_len = 0;       //������Ϣ�����ݳ���
u8 usart_rd_lentemp = 0;   //������¼�Ѷ�ȡ�����ݳ���
u8 usart_rx_enableflag = 0; //����״̬���
u8 usart_rx_lenrightflag = 0; //���ݳ���У��λ��ȷ��־
u8 usart_rx_successflag = 0; //�ɹ����յ�������Ϣ

void uart_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //ʹ��USART1��GPIOAʱ��
    USART_DeInit(USART1);                                                                       //��λ����1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   //PA.9//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                     //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��PA9

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                              //USART1_RX   PA.10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��PA10

    USART_InitStructure.USART_BaudRate = bound;                             //һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;             //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure);                               //��ʼ������
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;  //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;              //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                                                     //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                      //�����ж�
    USART_Cmd(USART1, ENABLE);                                          //ʹ�ܴ���
}

/******************************************************************************************/
/*                                                          �Զ��崮�������շ�����У��                                                              */
/*      ����ÿ�η���һ���ֽڣ�һ����λ������������û����һ���ֽڣ������ж�һ�Σ���ִ��һ�δ�*/
/*���жϺ�����                                                                                                                                                      */
/*��������У��              (��ͷ0xee)(���ݳ���)(...����...)(���ݳ��ȷ���)(��β0xef)                                        */
/*      ÿһ�����Ŵ���һ���ֽڣ����������Ŵ���N���ֽ�                                                                               */
/******************************************************************************************/
void USART1_IRQHandler(void)                                                    //����1�жϷ������
{
    u8 i, res, check_temp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {    //�����жϣ�ÿ����һ���ֽڣ�8λ���������ݣ����ж�һ��,ÿ���жϣ���ÿ����һ�����ݣ���ִ��һ�����г���
        res = USART_ReceiveData(USART1);                                            //��ȡ���յ�������
        if ((res == 0xee) && (usart_rx_enableflag == 0))                    //�����ܵ���ͷ(0xee)���ݲ��һ�û�гɹ�������������Ϣ
            usart_rx_enableflag = 1;                                                        //˵�����ǰ�ͷ�������������ݱ�־���������ݽ��ս׶�

        if (usart_rx_enableflag == 1) {                                                 //���������ݱ�־��λʱ����������
            res = USART_ReceiveData(USART1);                                    //��ȡ���ڱ�־
            if (usart_rd_lentemp == 0) {                                                //��ͷ���һ������Ϊ��Ҫ��������ݵĳ���
                usart_rd_len = res;                                                         //��ȡ���ݵĳ���
                if (usart_rd_len >= 40) {
                    usart_rx_lenrightflag = 0;                                      //���ݳ���У������
                    usart_rx_successflag = 0;                                       //���ݽ��ճɹ���־����
                    usart_rx_enableflag = 0;                                        //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len = 0;                                                       //���ݳ�������
                    usart_rd_lentemp = 0;                                               //���ݳ����ݴ�������
                }
            }

            else if (usart_rd_lentemp == usart_rd_len + 1) {        //����ȡ����usart_rd_lentemp+1������ʱ��У���Ƿ��ǳ�����Ϣ�ķ���
                check_temp = ~usart_rd_len;                                         //ȡ���ݳ���У��λ�ķ���
                if (res == check_temp)                                                  //�����ݳ���У����ȷʱ
                    usart_rx_lenrightflag = 1;                                      //���ݳ���У���־��һ
                else {
                    //�����ݳ���У�����ʱ
                    usart_rx_lenrightflag = 0;                                      //���ݳ���У������
                    usart_rx_successflag = 0;                                       //���ݽ��ճɹ���־����
                    usart_rx_enableflag = 0;                                        //�����ݳ���У�����ʱ�����ݽ���������־����
                    usart_rd_len = 0;                                                       //���ݳ�������
                    usart_rd_lentemp = 0;                                           //���ݳ����ݴ�������
                }
            } else if (usart_rd_lentemp == usart_rd_len + 2) {          //����ȡ����usart_rd_lentemp+2������ʱ��У���β�Ƿ���ȷ
                if ((res == 0xef) && (usart_rx_lenrightflag == 1)) { //�����β�����볤��У�鶼��ȷ
                    usart_rx_lenrightflag = 0;                                      //���ݳ���У������
                    usart_rx_successflag = 1;                                       //���ݽ��ճɹ���־��һ
                    usart_rx_enableflag = 0;                                        //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len = 0;                                                       //���ݳ�������
                    usart_rd_lentemp = 0;                                               //���ݳ����ݴ�������
                } else {
                    //����β����У�����ʱ
                    usart_rx_lenrightflag = 0;                                      //���ݳ���У������
                    usart_rx_successflag = 0;                                       //���ݽ��ճɹ���־����
                    usart_rx_enableflag = 0;                                        //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len = 0;                                                       //���ݳ�������
                    usart_rd_lentemp = 0;                                               //���ݳ����ݴ�������
                }
            } else usart_rx_temporary[usart_rd_lentemp - 1] = res; //��usart_rd_lentempΪ���ݶ�ʱ�������ݴ浽�������ݽ��ռĴ�����

            usart_rd_lentemp++;                                                                 //ÿ�μ�¼���ݣ����ݳ����ݴ����Լ�
            if (usart_rx_successflag == 1) {                                            //����ɹ����յ���Ϣ���ݣ�������usart_rx_temporary[]�ڵ����ݴ��ݸ�usart_rx_buf[][]
                for (i = 0; i < usart_rd_len; i++)
                    usart_rx_buf[usartrxbuf_pagebuf][i] = usart_rx_temporary[i];
                usartrxbuf_pagebuf++;                                                       //���ܻ����ҳ�뻺���Լ�
                if (usartrxbuf_pagebuf == 5) usartrxbuf_pagebuf = 0; //���ܻ����ҳ�뻺�����Ϊ5
                usartrxbuf_pagebuf++;
            }
        }
    }
}


#ifdef ITN9
void uart1_receive(void)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    u8 i, res, check_temp;
    //Uart1_Sendstr("[NOTICE]INTERRUPT_HANDLER in\n");
    //print("[NOTICE]INTERRUPT_HANDLER in\n");
    if (UART1_GetITStatus(UART1_IT_RXNE) != RESET) { //�����жϣ�ÿ����һ���ֽڣ�8λ���������ݣ����ж�һ��,ÿ���жϣ���ÿ����һ�����ݣ���ִ��һ�����г���
        UART1_ClearITPendingBit(UART1_IT_RXNE);
        res = 0x00ff & UART1_ReceiveData9();                 //��ȡ���յ�������
        if ((res == 0xee) && (usart_rx_enableflag == 0)) { //�����ܵ���ͷ(0xee)���ݲ��һ�û�гɹ�������������Ϣ
            /*head*/
            usart_rx_enableflag = 1;                      //˵�����ǰ�ͷ�������������ݱ�־���������ݽ��ս׶�
            printstr("[DEBUG] head :");
            printhex(res);
            printstr("\n");
        }

        if (usart_rx_enableflag == 1) {                   //���������ݱ�־��λʱ����������
            /*a frame*/
            //res=UART1_ReceiveData8();                //��ȡ���ڱ�־

            res = 0x00ff & UART1_ReceiveData9();            //��ȡ���ڱ�־
            printstr("[DEBUG] a Byte :");
            printhex(res);
            printstr("\n");
            if (usart_rd_lentemp == 0) {                  //��ͷ���һ������Ϊ��Ҫ��������ݵĳ���
                /*paylen*/
                usart_rd_len = res;                       //��ȡ���ݵĳ���
                printstr("[DEBUG] usart_rd_len :");
                printhex(usart_rd_len);
                printstr("\n");

                if (usart_rd_len >= UART_BUF_REG_LEN) {
                    usart_rx_lenrightflag = 0;  //���ݳ���У������
                    usart_rx_successflag = 0;   //���ݽ��ճɹ���־����
                    Uart1_Sendstr("[ERROR]Paylen too long receive failed\n");
                    usart_rx_enableflag = 0;    //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len = 0;           //���ݳ�������
                    usart_rd_lentemp = 0;       //���ݳ����ݴ�������
                }
            } else if (usart_rd_lentemp == usart_rd_len + 1) { //����ȡ����usart_rd_lentemp+1������ʱ��У���Ƿ��ǳ�����Ϣ�ķ���
                /*У�����ݳ��ȵ�ȡ��*/
                printstr("[DEBUGc] usart_rd_len :");
                printdec(usart_rd_len);
                printstr("\n");
                check_temp = ~usart_rd_len;               //ȡ���ݳ���У��λ�ķ���
                printstr("[DEBUG] check_temp :");
                printhex(check_temp);
                printstr("\n");
                printstr("[DEBUG] res :");
                printhex(res);
                printstr("\n");
                if (res == check_temp) {                  //�����ݳ���У����ȷʱ
                    usart_rx_lenrightflag = 1;            //���ݳ���У���־��һ
                } else {
                    //�����ݳ���У�����ʱ
                    usart_rx_lenrightflag = 0;             //���ݳ���У������
                    usart_rx_successflag = 0;              //���ݽ��ճɹ���־����
                    Uart1_Sendstr("[ERROR]Checksum failed receive failed\n");
                    usart_rx_enableflag = 0;                //�����ݳ���У�����ʱ�����ݽ���������־����
                    usart_rd_len = 0;                       //���ݳ�������
                    usart_rd_lentemp = 0;                   //���ݳ����ݴ�������
                }
            } else if (usart_rd_lentemp == usart_rd_len + 2) { //����ȡ����usart_rd_lentemp+2������ʱ��У���β�Ƿ���ȷ
                /*tail*/
                if ((res == 0xef) && (usart_rx_lenrightflag == 1)) { //�����β�����볤��У�鶼��ȷ
                    usart_rx_lenrightflag = 0;              //���ݳ���У������
                    usart_rx_successflag = 1;               //���ݽ��ճɹ���־��һ
                    Uart1_Sendstr("[NOTICE]receive sucess\n");
                    usart_rx_enableflag = 0;                //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len = 0;                       //���ݳ�������
                    usart_rd_lentemp = 0;                   //���ݳ����ݴ�������
                } else {
                    //����β����У�����ʱ
                    usart_rx_lenrightflag = 0;              //���ݳ���У������
                    usart_rx_successflag = 0;              //���ݽ��ճɹ���־����
                    Uart1_Sendstr("[ERROR]receive failed\n");
                    usart_rx_enableflag = 0;               //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len = 0;                      //���ݳ�������
                    usart_rd_lentemp = 0;                  //���ݳ����ݴ�������
                }
            } else usart_rx_temporary[usart_rd_lentemp - 1] = res; //��usart_rd_lentempΪ���ݶ�ʱ�������ݴ浽�������ݽ��ռĴ�����

            usart_rd_lentemp++;                            //ÿ�μ�¼���ݣ����ݳ����ݴ����Լ�
            if (usart_rx_successflag == 1) {               //����ɹ����յ���Ϣ���ݣ�������usart_rx_temporary[]�ڵ����ݴ��ݸ�usart_rx_buf[][]
                for (i = 0; i < usart_rd_len; i++)
                    usart_rx_buf[usartrxbuf_pagebuf][i] = usart_rx_temporary[i];
                usartrxbuf_pagebuf++;                              //���ܻ����ҳ�뻺���Լ�
                if (usartrxbuf_pagebuf == UART_BUF_REG_NUM) {
                    usartrxbuf_pagebuf = 0;  //���ܻ����ҳ�뻺�����Ϊ5
                }
                usartrxbuf_pagebuf++;
            }
        }
    }

}
#endif /* ITN9 */


