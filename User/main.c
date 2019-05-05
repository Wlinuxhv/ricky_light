#include "ALL_Includes.h"
#include "USART.h"
//#include "irtn9.h"
#include "irDA.h"
#include "pwm.h"


#define PWM_CMP_MAX 10000
#define PWM_CMP_80  8000
#define PWM_CMP_70  7000
#define PWM_CMP_60  6000
#define PWM_CMP_50  5000
#define PWM_CMP_40  4000
#define PWM_CMP_30  3000
#define PWM_CMP_20  2000
#define PWM_CMP_10  1000

extern void PWM_SetValue(uint16_t pwm);
extern void PWM_Init(void);


//定义CPU内部时钟
#define  SYS_CLOCK    16
uint8_t debug = 0;


void CLOCK_Config(u8 SYS_CLK);
void All_Congfig(void);


void All_Congfig(void)
{
	disableInterrupts();
	CLOCK_Config(SYS_CLOCK);//系统时钟初始化
	UART_Init(SYS_CLOCK,115200);
	LED_Init();
	PWM_Init();
	IR_Init();
	enableInterrupts();
	//TN9_Init();
}


/*********************************************
函数功能：系统内部时钟配置
输入参数：SYS_CLK : 2、4、8、16
输出参数：无
备    注：系统启动默认内部2ＭＨＺ
*********************************************/
void CLOCK_Config(u8 SYS_CLK)
{
	//时钟配置为内部RC，16M
#ifdef NO_LIB
	CLK->CKDIVR &=~(BIT(4)|BIT(3));

	switch(SYS_CLK) {
	case 2:
		CLK->CKDIVR |=((1<<4)|(1<<3));
		break;
	case 4:
		CLK->CKDIVR |=(1<<4);
		break;
	case 8:
		CLK->CKDIVR |=(1<<3);
		break;
	case 16:
		break;
	}
#endif /* NO_LIB */

	CLK_HSICmd(ENABLE);//HSI = 16MHz
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //f_psc = 16MHz/1
}

int main(void)
{
	int16_t duty = 0;
	int16_t secs = 0;
	uint8_t auto_pwm = 0;
	uint8_t key_val = 0;
	All_Congfig();
	Uart1_Sendstr("+++++++++hardware init ok+++++++++\n");
	delay_ms(1000);
	Uart1_Sendstr("+++++++++single threads ok+++++++++\n");
	while(1) 
    {
		delay_ms(50);
        secs = (secs++  >= 1000 )? 0 : secs ;
        key_val = 0;
		if( Ir_receive_ok == 1 ) { //一帧红外数据接收完成
			key_val = Ir_Process();
			Ir_receive_ok=0;
			//不同的遥控器面板对应不同的键值 0-9
            //display_key(key_val); 
            switch( key_val ) {
                case VOL_Plus:
                duty += 500;
                duty =  duty >= PWM_CMP_MAX  ? PWM_CMP_MAX : duty;
                PWM_SetValue(duty);
                break;

                case VOL_Mins:
                duty -= 500;
                duty =  duty <= 0  ? 0 : duty;
                PWM_SetValue(duty);
                break;
                
                case KEY_0:
                duty = 0;
                auto_pwm = 0;
                PWM_SetValue(duty);
                break;

                case EQ_KEY:
                auto_pwm = auto_pwm == 1 ? 0 : 1;
                break;
            }
        }
        if(auto_pwm)
        {
            duty += 10;
            duty =  duty >= PWM_CMP_60 ? PWM_CMP_60 : duty;
            PWM_SetValue(duty);
        }
        if(!(secs%100)){
            printstr("duty:");
            printdec(duty);
            printstr("\n");
        }
	}
}


#ifdef ITN9
void uart1_receive(void)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
	u8 i,res,check_temp;
	//Uart1_Sendstr("[NOTICE]INTERRUPT_HANDLER in\n");
	//print("[NOTICE]INTERRUPT_HANDLER in\n");
	if(UART1_GetITStatus(UART1_IT_RXNE) != RESET) { //接收中断，每接收一个字节（8位二进制数据），中断一次,每次中断（即每接收一个数据）都执行一次下列程序
		UART1_ClearITPendingBit(UART1_IT_RXNE);
		res= 0x00ff&UART1_ReceiveData9();                    //读取接收到的数据
		if((res==0xee)&&(usart_rx_enableflag==0)) {       //当接受到包头(0xee)数据并且还没有成功接收完数据信息
			/*head*/
			usart_rx_enableflag=1;                        //说明这是包头，启动接收数据标志，进入数据接收阶段
			printstr("[DEBUG] head :");
			printhex(res);
			printstr("\n");
		}

		if(usart_rx_enableflag==1) {                      //到接受数据标志置位时，接受数据
			/*a frame*/
			//res=UART1_ReceiveData8();                //读取串口标志

			res=0x00ff&UART1_ReceiveData9();                //读取串口标志
			printstr("[DEBUG] a Byte :");
			printhex(res);
			printstr("\n");
			if(usart_rd_lentemp==0) {                     //包头后第一个数据为需要传输的数据的长度
				/*paylen*/
				usart_rd_len=res;                         //读取数据的长度
				printstr("[DEBUG] usart_rd_len :");
				printhex(usart_rd_len);
				printstr("\n");

				if(usart_rd_len>=UART_BUF_REG_LEN) {
					usart_rx_lenrightflag=0;    //数据长度校验清零
					usart_rx_successflag=0;     //数据接收成功标志清零
					Uart1_Sendstr("[ERROR]Paylen too long receive failed\n");
					usart_rx_enableflag=0;      //数据接收完成，数据接收启动标志清零
					usart_rd_len=0;             //数据长度清零
					usart_rd_lentemp=0;         //数据长度暂存器清零
				}
			} else if(usart_rd_lentemp==usart_rd_len+1) { //当读取到第usart_rd_lentemp+1个数据时，校验是否是长度信息的反码
				/*校验数据长度的取反*/
				printstr("[DEBUGc] usart_rd_len :");
				printdec(usart_rd_len);
				printstr("\n");
				check_temp=~usart_rd_len;                 //取数据长度校验位的反码
				printstr("[DEBUG] check_temp :");
				printhex(check_temp);
				printstr("\n");
				printstr("[DEBUG] res :");
				printhex(res);
				printstr("\n");
				if(res==check_temp) {                     //当数据长度校验正确时
					usart_rx_lenrightflag=1;              //数据长度校验标志置一
				} else {
					//当数据长度校验错误时
					usart_rx_lenrightflag=0;               //数据长度校验清零
					usart_rx_successflag=0;                //数据接收成功标志清零
					Uart1_Sendstr("[ERROR]Checksum failed receive failed\n");
					usart_rx_enableflag=0;                  //当数据长度校验错误时，数据接收启动标志清零
					usart_rd_len=0;                         //数据长度清零
					usart_rd_lentemp=0;                     //数据长度暂存器清零
				}
			} else if(usart_rd_lentemp==usart_rd_len+2) {   //当读取到第usart_rd_lentemp+2个数据时，校验包尾是否正确
				/*tail*/
				if((res==0xef)&&(usart_rx_lenrightflag==1)) { //如果包尾数据与长度校验都正确
					usart_rx_lenrightflag=0;                //数据长度校验清零
					usart_rx_successflag=1;                 //数据接收成功标志置一
					Uart1_Sendstr("[NOTICE]receive sucess\n");
					usart_rx_enableflag=0;                  //数据接收完成，数据接收启动标志清零
					usart_rd_len=0;                         //数据长度清零
					usart_rd_lentemp=0;                     //数据长度暂存器清零
				} else {
					//当包尾数据校验错误时
					usart_rx_lenrightflag=0;                //数据长度校验清零
					usart_rx_successflag=0;                //数据接收成功标志清零
					Uart1_Sendstr("[ERROR]receive failed\n");
					usart_rx_enableflag=0;                 //数据接收完成，数据接收启动标志清零
					usart_rd_len=0;                        //数据长度清零
					usart_rd_lentemp=0;                    //数据长度暂存器清零
				}
			} else usart_rx_temporary[usart_rd_lentemp-1]=res;  //当usart_rd_lentemp为数据段时，将数据存到串口数据接收寄存器中

			usart_rd_lentemp++;                            //每次记录数据，数据长度暂存器自加
			if(usart_rx_successflag==1) {                  //如果成功接收到信息数据，将缓存usart_rx_temporary[]内的数据传递给usart_rx_buf[][]
				for(i=0; i<usart_rd_len; i++)
					usart_rx_buf[usartrxbuf_pagebuf][i]=usart_rx_temporary[i];
				usartrxbuf_pagebuf++;                              //接受缓存的页码缓存自加
				if(usartrxbuf_pagebuf==UART_BUF_REG_NUM) {
					usartrxbuf_pagebuf=0;    //接受缓存的页码缓存最大为5
				}
				usartrxbuf_pagebuf++;
			}
		}
	}

}
#endif /* ITN9 */
