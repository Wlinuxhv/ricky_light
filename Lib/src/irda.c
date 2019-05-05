#include "stm8s.h"
#include "stm8s_tim1.h"
#include "stm8s_exti.h"
#include "irDA.h"
#include "ALL_Includes.h"

#define IRDA_PORT GPIOC
#define IRDA_PIN GPIO_PIN_7

volatile u8  Ir_Status=1;              //红外接收处理状态
volatile u8  Ir_Receive_Count=0;       //红外接收数据位计数
volatile u32 Ir_Receive_Data=0;        //32位的红外接收数据
volatile u8  Ir_receive_ok=0;          //红外接收完成标志
volatile u16 time_interval = 0;
volatile u16 time1 = 0;
volatile u16 time2 = 0;
volatile u8 start_flag = 1;

/*
********************************************************************************
            定时器溢出中断内处理
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
                          红外接收数据处理函数
********************************************************************************
*/
u8 Ir_Process(void)
{
	u8 Ir_num=0;                  //最终处理后的键值返回值
	u8 Address_H,Address_L;       //地址码,地址反码
	u8 Data_H,Data_L;             //数据码,数据反码

	if(Ir_receive_ok==1) {        //接收完成
		Address_H=Ir_Receive_Data>>24;                //得到地址码
		Address_L=(Ir_Receive_Data>>16)&0xff;         //得到地址反码
		//if((Address_H==(u8)~Address_L)&&(Address_H==REMOTE_ID))//检验遥控识别码(ID)及地址
		if((Address_H==(u8)~Address_L)) { //检验遥控识别码(ID)及地址
			Data_H=Ir_Receive_Data>>8;              //得到数据码
			Data_L=Ir_Receive_Data;                 //得到数据反码
			if(Data_H==(u8)~Data_L) {               //接收数据码正确
				Ir_num=Data_H;                      //正确键值
				Ir_Receive_Data=0;
			}
		}
	}
	return  Ir_num;      //返回键值
}

void TIM2_Init(void)
{
	TIM2_DeInit();
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, 0xFFFF);//定时器设置1M的计数频率，1US的分辨率 ,计时60ms
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_Cmd(ENABLE);
}


//外中断接收红外数据
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
                          红外接收中断处理函数
********************************************************************************
*/
void Ir_Receive_Handle(void)
{
	if( start_flag )    {
		time1 = TIM2_GetCounter();  //开始的时候，记录时刻1
		start_flag = 0;
		time_interval = 0;
		Ir_Status=1;
		return;
	} else {
		time2 = TIM2_GetCounter();//第二次的时刻
		if(time2 >= time1) { //计算时间
			time_interval = time2 -time1;
		} else {
			time_interval = 0xFFFF - (time1 - time2) + 1;   //定时器溢出时这样算
		}
		time1 = TIM2_GetCounter();  //更新当前时刻，为下一个码做准备
	}

	switch(Ir_Status) {
	case 1://第二个下降沿，定时器关闭，读取定时器计数值
		if( (time_interval>=12500)&&(time_interval<=14500) ) { //判断引导码是否正确9+4.5ms
			Ir_Status=2;                    //进入下一状态
		} else {                            //引导码错误，从新接收
			Ir_Status=1;
			Ir_Receive_Count=0;
		}
		break;

	case 2://开始32位数据的接收
		if( (time_interval>=1000)&&(time_interval<=1300) ) {      //间隔1.12ms ->0
			Ir_Receive_Data=Ir_Receive_Data<<1;
			Ir_Receive_Count++;
		} else if( (time_interval>=2000)&&(time_interval<=2600) ) { //间隔2.25ms ->1
			Ir_Receive_Data=Ir_Receive_Data<<1;
			Ir_Receive_Data=Ir_Receive_Data|0x0001;
			Ir_Receive_Count++;
		} else { //不是0,1 接收错误，从新接收
			Ir_Status=1;
			Ir_Receive_Data=0;
			Ir_Receive_Count=0;
		}

		//超出接收数据位数，接收下一个
		while(Ir_Receive_Count==32) {
			Ir_receive_ok=1;//红外接收完成
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

