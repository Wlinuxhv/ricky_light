#include "stm8s.h"
#include "stm8s_tim1.h"
#include "stm8s_exti.h"
#include "irDA.h"
#include "ALL_Includes.h"

#define IRDA_PORT GPIOC
#define IRDA_PIN GPIO_PIN_7

volatile u8  Ir_Status=1;              //������մ���״̬
volatile u8  Ir_Receive_Count=0;       //�����������λ����
volatile u32 Ir_Receive_Data=0;        //32λ�ĺ����������
volatile u8  Ir_receive_ok=0;          //���������ɱ�־
volatile u16 time_interval = 0;
volatile u16 time1 = 0;
volatile u16 time2 = 0;
volatile u8 start_flag = 1;

/*
********************************************************************************
            ��ʱ������ж��ڴ���
********************************************************************************
*/
void TIM_IT_Updata_Handdle(void)
{
	TIM2_Cmd(DISABLE);
	Ir_Status=0;
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
	TIM2_SetCounter(0);
}


/*
********************************************************************************
                          ����������ݴ�����
********************************************************************************
*/
u8 Ir_Process(void)
{
	u8 Ir_num=0;                  //���մ����ļ�ֵ����ֵ
	u8 Address_H,Address_L;       //��ַ��,��ַ����
	u8 Data_H,Data_L;             //������,���ݷ���

	if(Ir_receive_ok==1) {        //�������
		Address_H=Ir_Receive_Data>>24;                //�õ���ַ��
		Address_L=(Ir_Receive_Data>>16)&0xff;         //�õ���ַ����
		//if((Address_H==(u8)~Address_L)&&(Address_H==REMOTE_ID))//����ң��ʶ����(ID)����ַ
		if((Address_H==(u8)~Address_L)) { //����ң��ʶ����(ID)����ַ
			Data_H=Ir_Receive_Data>>8;              //�õ�������
			Data_L=Ir_Receive_Data;                 //�õ����ݷ���
			if(Data_H==(u8)~Data_L) {               //������������ȷ
				Ir_num=Data_H;                      //��ȷ��ֵ
				Ir_Receive_Data=0;
			}
		}
	}
	return  Ir_num;      //���ؼ�ֵ
}

void TIM2_Init(void)
{
	TIM2_DeInit();
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, 0xFFFF);//��ʱ������1M�ļ���Ƶ�ʣ�1US�ķֱ��� ,��ʱ60ms
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_Cmd(ENABLE);
}


//���жϽ��պ�������
void EXIT_Init(void)
{
	EXTI_DeInit();
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC,EXTI_SENSITIVITY_FALL_ONLY);
}

void IR_Init(void)
{
	GPIO_Init(IRDA_PORT,IRDA_PIN,GPIO_MODE_IN_PU_IT);
	TIM2_Init();
	EXIT_Init();
}

/*
********************************************************************************
                          ��������жϴ�����
********************************************************************************
*/
void Ir_Receive_Handle(void)
{
	if( start_flag )    {
		time1 = TIM2_GetCounter();  //��ʼ��ʱ�򣬼�¼ʱ��1
		start_flag = 0;
		time_interval = 0;
		Ir_Status=1;
		return;
	} else {
		time2 = TIM2_GetCounter();//�ڶ��ε�ʱ��
		if(time2 >= time1) { //����ʱ��
			time_interval = time2 -time1;
		} else {
			time_interval = 0xFFFF - (time1 - time2) + 1;   //��ʱ�����ʱ������
		}
		time1 = TIM2_GetCounter();  //���µ�ǰʱ�̣�Ϊ��һ������׼��
	}

	switch(Ir_Status) {
	case 1://�ڶ����½��أ���ʱ���رգ���ȡ��ʱ������ֵ
		if( (time_interval>=12500)&&(time_interval<=14500) ) { //�ж��������Ƿ���ȷ9+4.5ms
			Ir_Status=2;                    //������һ״̬
		} else {                            //��������󣬴��½���
			Ir_Status=1;
			Ir_Receive_Count=0;
		}
		break;

	case 2://��ʼ32λ���ݵĽ���
		if( (time_interval>=1000)&&(time_interval<=1300) ) {      //���1.12ms ->0
			Ir_Receive_Data=Ir_Receive_Data<<1;
			Ir_Receive_Count++;
		} else if( (time_interval>=2000)&&(time_interval<=2600) ) { //���2.25ms ->1
			Ir_Receive_Data=Ir_Receive_Data<<1;
			Ir_Receive_Data=Ir_Receive_Data|0x0001;
			Ir_Receive_Count++;
		} else { //����0,1 ���մ��󣬴��½���
			Ir_Status=1;
			Ir_Receive_Data=0;
			Ir_Receive_Count=0;
		}

		//������������λ����������һ��
		while(Ir_Receive_Count==32) {
			Ir_receive_ok=1;//����������
			Ir_Status=1;
			Ir_Receive_Count=0;
			break;
		}
		break;

	default :
		break;
	}
}

void display_key(uint16_t key_val)
{
    switch( key_val ) {
        case KEY_0:printstr("key0_val:");printdec(key_val); printstr("\n");break;
        case KEY_1:printstr("key1_val:");printdec(key_val); printstr("\n");break;
        case KEY_2:printstr("key2_val:");printdec(key_val); printstr("\n");break;
        case KEY_3:printstr("key3_val:");printdec(key_val); printstr("\n");break;
        case KEY_4:printstr("key4_val:");printdec(key_val); printstr("\n");break;
        case KEY_5:printstr("key5_val:");printdec(key_val); printstr("\n");break;
        case KEY_6:printstr("key6_val:");printdec(key_val); printstr("\n");break;
        case KEY_7:printstr("key7_val:");printdec(key_val); printstr("\n");break;
        case KEY_8:printstr("key8_val:");printdec(key_val); printstr("\n");break;
        case KEY_9:printstr("key9_val:");printdec(key_val); printstr("\n");break;
        case CH_Mins:printstr("CH_Mins:");printdec(key_val); printstr("\n");break;
        case CH_Midl:printstr("CH_Midl:");printdec(key_val); printstr("\n");break;
        case CH_Plus:printstr("CH_Plus:");printdec(key_val); printstr("\n");break;
        case PREV_KEY:printstr("PREV_KEY:");printdec(key_val); printstr("\n");break;
        case NEXT_KEY:printstr("NEXT_KEY:");printdec(key_val); printstr("\n");break;
        case PLAY_PAUSE:printstr("PLAY_PAUSE:");printdec(key_val); printstr("\n");break;
        case VOL_Mins:printstr("VOL_Mins:");printdec(key_val); printstr("\n");break;
        case VOL_Plus:printstr("VOL_Plus:");printdec(key_val); printstr("\n");break;
        case EQ_KEY:printstr("EQ_KEY:");printdec(key_val); printstr("\n");break;
        case Plus100:printstr("Plus100:");printdec(key_val); printstr("\n");break;
        case Plus200:printstr("Plus200:");printdec(key_val); printstr("\n");break;
        break;
    }

}

