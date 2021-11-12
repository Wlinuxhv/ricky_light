#include "sys.h"
#include "usart.h"

u8 usart_rx_buf[5][40];    //接收缓冲,最大40个字节
u8 usart_rx_temporary[40]; //数据保存暂存器,最多能够缓存40个字节
u8 usartrxbuf_pagebuf = 0;   //最上面接受缓存的页码（5）缓存
u8 usart_rd_len = 0;       //有用信息的数据长度
u8 usart_rd_lentemp = 0;   //用来记录已读取的数据长度
u8 usart_rx_enableflag = 0; //接收状态标记
u8 usart_rx_lenrightflag = 0; //数据长度校验位正确标志
u8 usart_rx_successflag = 0; //成功接收到数据信息

void uart_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟
    USART_DeInit(USART1);                                                                       //复位串口1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   //PA.9//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                     //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化PA9

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                              //USART1_RX   PA.10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化PA10

    USART_InitStructure.USART_BaudRate = bound;                             //一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USART1, &USART_InitStructure);                               //初始化串口
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;  //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;              //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                                     //根据指定的参数初始化VIC寄存器

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                      //开启中断
    USART_Cmd(USART1, ENABLE);                                          //使能串口
}

/******************************************************************************************/
/*                                                          自定义串口数据收发数据校验                                                              */
/*      串口每次发送一个字节（一个八位二进制数），没接受一个字节，串口中断一次，就执行一次串*/
/*口中断函数。                                                                                                                                                      */
/*串口数据校验              (包头0xee)(数据长度)(...数据...)(数据长度反码)(包尾0xef)                                        */
/*      每一个括号代表一个字节，而数据括号待变N个字节                                                                               */
/******************************************************************************************/
void USART1_IRQHandler(void)                                                    //串口1中断服务程序
{
    u8 i, res, check_temp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {    //接收中断，每接收一个字节（8位二进制数据），中断一次,每次中断（即每接收一个数据）都执行一次下列程序
        res = USART_ReceiveData(USART1);                                            //读取接收到的数据
        if ((res == 0xee) && (usart_rx_enableflag == 0))                    //当接受到包头(0xee)数据并且还没有成功接收完数据信息
            usart_rx_enableflag = 1;                                                        //说明这是包头，启动接收数据标志，进入数据接收阶段

        if (usart_rx_enableflag == 1) {                                                 //到接受数据标志置位时，接受数据
            res = USART_ReceiveData(USART1);                                    //读取串口标志
            if (usart_rd_lentemp == 0) {                                                //包头后第一个数据为需要传输的数据的长度
                usart_rd_len = res;                                                         //读取数据的长度
                if (usart_rd_len >= 40) {
                    usart_rx_lenrightflag = 0;                                      //数据长度校验清零
                    usart_rx_successflag = 0;                                       //数据接收成功标志清零
                    usart_rx_enableflag = 0;                                        //数据接收完成，数据接收启动标志清零
                    usart_rd_len = 0;                                                       //数据长度清零
                    usart_rd_lentemp = 0;                                               //数据长度暂存器清零
                }
            }

            else if (usart_rd_lentemp == usart_rd_len + 1) {        //当读取到第usart_rd_lentemp+1个数据时，校验是否是长度信息的反码
                check_temp = ~usart_rd_len;                                         //取数据长度校验位的反码
                if (res == check_temp)                                                  //当数据长度校验正确时
                    usart_rx_lenrightflag = 1;                                      //数据长度校验标志置一
                else {
                    //当数据长度校验错误时
                    usart_rx_lenrightflag = 0;                                      //数据长度校验清零
                    usart_rx_successflag = 0;                                       //数据接收成功标志清零
                    usart_rx_enableflag = 0;                                        //当数据长度校验错误时，数据接收启动标志清零
                    usart_rd_len = 0;                                                       //数据长度清零
                    usart_rd_lentemp = 0;                                           //数据长度暂存器清零
                }
            } else if (usart_rd_lentemp == usart_rd_len + 2) {          //当读取到第usart_rd_lentemp+2个数据时，校验包尾是否正确
                if ((res == 0xef) && (usart_rx_lenrightflag == 1)) { //如果包尾数据与长度校验都正确
                    usart_rx_lenrightflag = 0;                                      //数据长度校验清零
                    usart_rx_successflag = 1;                                       //数据接收成功标志置一
                    usart_rx_enableflag = 0;                                        //数据接收完成，数据接收启动标志清零
                    usart_rd_len = 0;                                                       //数据长度清零
                    usart_rd_lentemp = 0;                                               //数据长度暂存器清零
                } else {
                    //当包尾数据校验错误时
                    usart_rx_lenrightflag = 0;                                      //数据长度校验清零
                    usart_rx_successflag = 0;                                       //数据接收成功标志清零
                    usart_rx_enableflag = 0;                                        //数据接收完成，数据接收启动标志清零
                    usart_rd_len = 0;                                                       //数据长度清零
                    usart_rd_lentemp = 0;                                               //数据长度暂存器清零
                }
            } else usart_rx_temporary[usart_rd_lentemp - 1] = res; //当usart_rd_lentemp为数据段时，将数据存到串口数据接收寄存器中

            usart_rd_lentemp++;                                                                 //每次记录数据，数据长度暂存器自加
            if (usart_rx_successflag == 1) {                                            //如果成功接收到信息数据，将缓存usart_rx_temporary[]内的数据传递给usart_rx_buf[][]
                for (i = 0; i < usart_rd_len; i++)
                    usart_rx_buf[usartrxbuf_pagebuf][i] = usart_rx_temporary[i];
                usartrxbuf_pagebuf++;                                                       //接受缓存的页码缓存自加
                if (usartrxbuf_pagebuf == 5) usartrxbuf_pagebuf = 0; //接受缓存的页码缓存最大为5
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
    if (UART1_GetITStatus(UART1_IT_RXNE) != RESET) { //接收中断，每接收一个字节（8位二进制数据），中断一次,每次中断（即每接收一个数据）都执行一次下列程序
        UART1_ClearITPendingBit(UART1_IT_RXNE);
        res = 0x00ff & UART1_ReceiveData9();                 //读取接收到的数据
        if ((res == 0xee) && (usart_rx_enableflag == 0)) { //当接受到包头(0xee)数据并且还没有成功接收完数据信息
            /*head*/
            usart_rx_enableflag = 1;                      //说明这是包头，启动接收数据标志，进入数据接收阶段
            printstr("[DEBUG] head :");
            printhex(res);
            printstr("\n");
        }

        if (usart_rx_enableflag == 1) {                   //到接受数据标志置位时，接受数据
            /*a frame*/
            //res=UART1_ReceiveData8();                //读取串口标志

            res = 0x00ff & UART1_ReceiveData9();            //读取串口标志
            printstr("[DEBUG] a Byte :");
            printhex(res);
            printstr("\n");
            if (usart_rd_lentemp == 0) {                  //包头后第一个数据为需要传输的数据的长度
                /*paylen*/
                usart_rd_len = res;                       //读取数据的长度
                printstr("[DEBUG] usart_rd_len :");
                printhex(usart_rd_len);
                printstr("\n");

                if (usart_rd_len >= UART_BUF_REG_LEN) {
                    usart_rx_lenrightflag = 0;  //数据长度校验清零
                    usart_rx_successflag = 0;   //数据接收成功标志清零
                    Uart1_Sendstr("[ERROR]Paylen too long receive failed\n");
                    usart_rx_enableflag = 0;    //数据接收完成，数据接收启动标志清零
                    usart_rd_len = 0;           //数据长度清零
                    usart_rd_lentemp = 0;       //数据长度暂存器清零
                }
            } else if (usart_rd_lentemp == usart_rd_len + 1) { //当读取到第usart_rd_lentemp+1个数据时，校验是否是长度信息的反码
                /*校验数据长度的取反*/
                printstr("[DEBUGc] usart_rd_len :");
                printdec(usart_rd_len);
                printstr("\n");
                check_temp = ~usart_rd_len;               //取数据长度校验位的反码
                printstr("[DEBUG] check_temp :");
                printhex(check_temp);
                printstr("\n");
                printstr("[DEBUG] res :");
                printhex(res);
                printstr("\n");
                if (res == check_temp) {                  //当数据长度校验正确时
                    usart_rx_lenrightflag = 1;            //数据长度校验标志置一
                } else {
                    //当数据长度校验错误时
                    usart_rx_lenrightflag = 0;             //数据长度校验清零
                    usart_rx_successflag = 0;              //数据接收成功标志清零
                    Uart1_Sendstr("[ERROR]Checksum failed receive failed\n");
                    usart_rx_enableflag = 0;                //当数据长度校验错误时，数据接收启动标志清零
                    usart_rd_len = 0;                       //数据长度清零
                    usart_rd_lentemp = 0;                   //数据长度暂存器清零
                }
            } else if (usart_rd_lentemp == usart_rd_len + 2) { //当读取到第usart_rd_lentemp+2个数据时，校验包尾是否正确
                /*tail*/
                if ((res == 0xef) && (usart_rx_lenrightflag == 1)) { //如果包尾数据与长度校验都正确
                    usart_rx_lenrightflag = 0;              //数据长度校验清零
                    usart_rx_successflag = 1;               //数据接收成功标志置一
                    Uart1_Sendstr("[NOTICE]receive sucess\n");
                    usart_rx_enableflag = 0;                //数据接收完成，数据接收启动标志清零
                    usart_rd_len = 0;                       //数据长度清零
                    usart_rd_lentemp = 0;                   //数据长度暂存器清零
                } else {
                    //当包尾数据校验错误时
                    usart_rx_lenrightflag = 0;              //数据长度校验清零
                    usart_rx_successflag = 0;              //数据接收成功标志清零
                    Uart1_Sendstr("[ERROR]receive failed\n");
                    usart_rx_enableflag = 0;               //数据接收完成，数据接收启动标志清零
                    usart_rd_len = 0;                      //数据长度清零
                    usart_rd_lentemp = 0;                  //数据长度暂存器清零
                }
            } else usart_rx_temporary[usart_rd_lentemp - 1] = res; //当usart_rd_lentemp为数据段时，将数据存到串口数据接收寄存器中

            usart_rd_lentemp++;                            //每次记录数据，数据长度暂存器自加
            if (usart_rx_successflag == 1) {               //如果成功接收到信息数据，将缓存usart_rx_temporary[]内的数据传递给usart_rx_buf[][]
                for (i = 0; i < usart_rd_len; i++)
                    usart_rx_buf[usartrxbuf_pagebuf][i] = usart_rx_temporary[i];
                usartrxbuf_pagebuf++;                              //接受缓存的页码缓存自加
                if (usartrxbuf_pagebuf == UART_BUF_REG_NUM) {
                    usartrxbuf_pagebuf = 0;  //接受缓存的页码缓存最大为5
                }
                usartrxbuf_pagebuf++;
            }
        }
    }

}
#endif /* ITN9 */


