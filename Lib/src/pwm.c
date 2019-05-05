#include "ALL_Includes.h"
#include "stm8s_tim1.h"
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

void PWM_IO_Init(void)
{
      //设置IO口为输出
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);   //输出推拉，低电平，10MHz
    //GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);
}

void PWM_TIM_Init(void)
{
#if 0
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_2, 1000); //2分频， 1MHZ 1000 = 1ms F= 1KHZ
    TIM2_OC2Init(TIM2_OCMODE_PWM1,\
        TIM2_OUTPUTSTATE_ENABLE,\
        0,\
        TIM2_OCPOLARITY_HIGH);
     TIM2_Cmd(ENABLE); //使能定时器2
#else
  TIM1_DeInit(); 
  TIM1_TimeBaseInit(0x0f, TIM1_COUNTERMODE_UP, PWM_CMP_MAX, 0x00);//  
  TIM1_OC4Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, 0, TIM1_OCPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET);//50%占空比 
  TIM1_CCxCmd(TIM1_CHANNEL_4, ENABLE); 
  TIM1_OC4PreloadConfig(ENABLE); 
  TIM1_CtrlPWMOutputs(ENABLE); 
  TIM1_Cmd(ENABLE); 
#endif
}

void PWM_SetValue(uint16_t pwm)
{
    TIM1_SetCompare4(pwm);    
}

void PWM_Init(void)
{
    PWM_IO_Init();
    PWM_TIM_Init();
}
