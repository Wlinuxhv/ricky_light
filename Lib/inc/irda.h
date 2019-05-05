#ifndef WL_IRDA_H_
#define WL_IRDA_H_

#define CH_Mins 162
#define CH_Midl 98
#define CH_Plus 226
#define PREV_KEY 34
#define NEXT_KEY 2
#define PLAY_PAUSE 194
#define VOL_Plus 168
#define VOL_Mins 224
#define EQ_KEY 144
#define Plus100 152
#define Plus200 176
#define KEY_0 104
#define KEY_1 48
#define KEY_2 24
#define KEY_3 122
#define KEY_4 16
#define KEY_5 56
#define KEY_6 90
#define KEY_7 66
#define KEY_8 74
#define KEY_9 82

extern volatile u8  Ir_receive_ok;
extern volatile u16 time_interval;

extern void IR_Init(void);
extern  void TIM_IT_Updata_Handdle(void);
extern  void Ir_Receive_Handle(void);
extern u8 Ir_Process(void);
extern void display_key(uint16_t key_val);


#endif





