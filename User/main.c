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


//����CPU�ڲ�ʱ��
#define  SYS_CLOCK    16
uint8_t debug = 0;


void CLOCK_Config(u8 SYS_CLK);
void All_Congfig(void);


void All_Congfig(void)
{
	disableInterrupts();
	CLOCK_Config(SYS_CLOCK);//ϵͳʱ�ӳ�ʼ��
	UART_Init(SYS_CLOCK,115200);
	LED_Init();
	PWM_Init();
	IR_Init();
	enableInterrupts();
	//TN9_Init();
}


/*********************************************
�������ܣ�ϵͳ�ڲ�ʱ������
���������SYS_CLK : 2��4��8��16
�����������
��    ע��ϵͳ����Ĭ���ڲ�2�ͣȣ�
*********************************************/
void CLOCK_Config(u8 SYS_CLK)
{
	//ʱ������Ϊ�ڲ�RC��16M
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
		if( Ir_receive_ok == 1 ) { //һ֡�������ݽ������
			key_val = Ir_Process();
			Ir_receive_ok=0;
			//��ͬ��ң��������Ӧ��ͬ�ļ�ֵ 0-9
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
	if(UART1_GetITStatus(UART1_IT_RXNE) != RESET) { //�����жϣ�ÿ����һ���ֽڣ�8λ���������ݣ����ж�һ��,ÿ���жϣ���ÿ����һ�����ݣ���ִ��һ�����г���
		UART1_ClearITPendingBit(UART1_IT_RXNE);
		res= 0x00ff&UART1_ReceiveData9();                    //��ȡ���յ�������
		if((res==0xee)&&(usart_rx_enableflag==0)) {       //�����ܵ���ͷ(0xee)���ݲ��һ�û�гɹ�������������Ϣ
			/*head*/
			usart_rx_enableflag=1;                        //˵�����ǰ�ͷ�������������ݱ�־���������ݽ��ս׶�
			printstr("[DEBUG] head :");
			printhex(res);
			printstr("\n");
		}

		if(usart_rx_enableflag==1) {                      //���������ݱ�־��λʱ����������
			/*a frame*/
			//res=UART1_ReceiveData8();                //��ȡ���ڱ�־

			res=0x00ff&UART1_ReceiveData9();                //��ȡ���ڱ�־
			printstr("[DEBUG] a Byte :");
			printhex(res);
			printstr("\n");
			if(usart_rd_lentemp==0) {                     //��ͷ���һ������Ϊ��Ҫ��������ݵĳ���
				/*paylen*/
				usart_rd_len=res;                         //��ȡ���ݵĳ���
				printstr("[DEBUG] usart_rd_len :");
				printhex(usart_rd_len);
				printstr("\n");

				if(usart_rd_len>=UART_BUF_REG_LEN) {
					usart_rx_lenrightflag=0;    //���ݳ���У������
					usart_rx_successflag=0;     //���ݽ��ճɹ���־����
					Uart1_Sendstr("[ERROR]Paylen too long receive failed\n");
					usart_rx_enableflag=0;      //���ݽ�����ɣ����ݽ���������־����
					usart_rd_len=0;             //���ݳ�������
					usart_rd_lentemp=0;         //���ݳ����ݴ�������
				}
			} else if(usart_rd_lentemp==usart_rd_len+1) { //����ȡ����usart_rd_lentemp+1������ʱ��У���Ƿ��ǳ�����Ϣ�ķ���
				/*У�����ݳ��ȵ�ȡ��*/
				printstr("[DEBUGc] usart_rd_len :");
				printdec(usart_rd_len);
				printstr("\n");
				check_temp=~usart_rd_len;                 //ȡ���ݳ���У��λ�ķ���
				printstr("[DEBUG] check_temp :");
				printhex(check_temp);
				printstr("\n");
				printstr("[DEBUG] res :");
				printhex(res);
				printstr("\n");
				if(res==check_temp) {                     //�����ݳ���У����ȷʱ
					usart_rx_lenrightflag=1;              //���ݳ���У���־��һ
				} else {
					//�����ݳ���У�����ʱ
					usart_rx_lenrightflag=0;               //���ݳ���У������
					usart_rx_successflag=0;                //���ݽ��ճɹ���־����
					Uart1_Sendstr("[ERROR]Checksum failed receive failed\n");
					usart_rx_enableflag=0;                  //�����ݳ���У�����ʱ�����ݽ���������־����
					usart_rd_len=0;                         //���ݳ�������
					usart_rd_lentemp=0;                     //���ݳ����ݴ�������
				}
			} else if(usart_rd_lentemp==usart_rd_len+2) {   //����ȡ����usart_rd_lentemp+2������ʱ��У���β�Ƿ���ȷ
				/*tail*/
				if((res==0xef)&&(usart_rx_lenrightflag==1)) { //�����β�����볤��У�鶼��ȷ
					usart_rx_lenrightflag=0;                //���ݳ���У������
					usart_rx_successflag=1;                 //���ݽ��ճɹ���־��һ
					Uart1_Sendstr("[NOTICE]receive sucess\n");
					usart_rx_enableflag=0;                  //���ݽ�����ɣ����ݽ���������־����
					usart_rd_len=0;                         //���ݳ�������
					usart_rd_lentemp=0;                     //���ݳ����ݴ�������
				} else {
					//����β����У�����ʱ
					usart_rx_lenrightflag=0;                //���ݳ���У������
					usart_rx_successflag=0;                //���ݽ��ճɹ���־����
					Uart1_Sendstr("[ERROR]receive failed\n");
					usart_rx_enableflag=0;                 //���ݽ�����ɣ����ݽ���������־����
					usart_rd_len=0;                        //���ݳ�������
					usart_rd_lentemp=0;                    //���ݳ����ݴ�������
				}
			} else usart_rx_temporary[usart_rd_lentemp-1]=res;  //��usart_rd_lentempΪ���ݶ�ʱ�������ݴ浽�������ݽ��ռĴ�����

			usart_rd_lentemp++;                            //ÿ�μ�¼���ݣ����ݳ����ݴ����Լ�
			if(usart_rx_successflag==1) {                  //����ɹ����յ���Ϣ���ݣ�������usart_rx_temporary[]�ڵ����ݴ��ݸ�usart_rx_buf[][]
				for(i=0; i<usart_rd_len; i++)
					usart_rx_buf[usartrxbuf_pagebuf][i]=usart_rx_temporary[i];
				usartrxbuf_pagebuf++;                              //���ܻ����ҳ�뻺���Լ�
				if(usartrxbuf_pagebuf==UART_BUF_REG_NUM) {
					usartrxbuf_pagebuf=0;    //���ܻ����ҳ�뻺�����Ϊ5
				}
				usartrxbuf_pagebuf++;
			}
		}
	}

}
#endif /* ITN9 */
