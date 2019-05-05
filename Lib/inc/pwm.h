#ifdef  __WL_PWM__
#define __WL_PWM__

#include "ALL_Includes.h"

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

#endif
