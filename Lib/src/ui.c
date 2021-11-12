#include "ui.h"


#define PWM_CMP_MAX 10000
#define PWM_CMP_80  8000
#define PWM_CMP_70  7000
#define PWM_CMP_60  6000
#define PWM_CMP_50  5000
#define PWM_CMP_40  4000
#define PWM_CMP_30  3000
#define PWM_CMP_20  2000
#define PWM_CMP_10  1000



typedef enum manaul_mode {
    M_MODE_0 = 0,
    M_MODE_1,
    M_MODE_2,
    M_MODE_3,
    M_MODE_4,
    M_MODE_5,
    M_MODE_MAX = M_MODE_5,
} e_Mode_t;


typedef struct ui_data {
    uint8_t mode;
    uint8_t is_pwm_enable;
    uint8_t pwm_auto_mode;
    uint16_t pwm_duty;
} ui_data_t;

ui_data_t UI_Data = {
    .mode = M_MODE_0,
    .is_pwm_enable = 0,
    .pwm_auto_mode = 0,
    .pwm_duty = 0,
};

void EQ_callback_mode_switch(void *op)
{
    ui_data_t *p = (ui_data_t*)op;
    if (op) {
      p->mode++;
        p->mode %=  M_MODE_MAX;
    }
}

void VOL_Plus_callback(void *op)
{
    ui_data_t *p = (ui_data_t*)op;
    if (op) {
        switch (p->mode) {
            case M_MODE_0:
                if (p->is_pwm_enable) {
                    p->pwm_duty += 500;
                    p->pwm_duty =  p->pwm_duty >= PWM_CMP_MAX  ? PWM_CMP_MAX : p->pwm_duty;
                    PWM_SetValue(p->pwm_duty);
                }
                break;
            case M_MODE_1:
                break;

            case M_MODE_2:
                break;
            case M_MODE_3:
                break;
            case M_MODE_4:
                break;
            case M_MODE_5:
                break;
        }
    }
}
void VOL_Mins_callback(void *op)
{
    ui_data_t *p = (ui_data_t*)op;
    if (op) {
        switch (p->mode) {
            case M_MODE_0:
                if (p->is_pwm_enable) {
                    p->pwm_duty -= 500;
                    p->pwm_duty =  p->pwm_duty >= PWM_CMP_MAX  ? PWM_CMP_MAX : p->pwm_duty;
                    PWM_SetValue(p->pwm_duty);
                }
                break;
            case M_MODE_1:
                break;
            case M_MODE_2:
                break;
            case M_MODE_3:
                break;
            case M_MODE_4:
                break;
            case M_MODE_5:
                break;
        }
    }
}
void PLAY_PAUSE_callback(void *op)
{
    ui_data_t *p = (ui_data_t*)op;
    if (op) {
        switch (p->mode) {
            case M_MODE_0:
                if (p->is_pwm_enable) {
                    p->pwm_duty = 0;
                    p->is_pwm_enable = 0;
                    PWM_SetValue(p->pwm_duty);
                }
                break;
            case M_MODE_1:
                break;
            case M_MODE_2:
                break;
            case M_MODE_3:
                break;
            case M_MODE_4:
                break;
            case M_MODE_5:
                break;
        }
    }
}
void CH_Midl_callback(void *op)
{
    ui_data_t *p = (ui_data_t*)op;
    if (op) {
        switch (p->mode) {
            case M_MODE_0:
                if (p->is_pwm_enable) {
                    p->pwm_auto_mode = !p->pwm_auto_mode;
                }
                break;
            case M_MODE_1:
                break;
            case M_MODE_2:
                break;
            case M_MODE_3:
                break;
            case M_MODE_4:
                break;
            case M_MODE_5:
                break;
        }
    }
}
void ui_init(void)
{
    key_callback_register(EQ_KEY, EQ_callback_mode_switch);
    key_callback_register(PLAY_PAUSE, PLAY_PAUSE_callback);
    key_callback_register(VOL_Plus, VOL_Plus_callback);
    key_callback_register(VOL_Mins, VOL_Mins_callback);
    key_callback_register(CH_Midl, CH_Midl_callback);
}
void ui_loop_routine(uint16_t key_val)
{
    key_callback_do(key_val, (void *)&UI_Data);
    if (M_MODE_0 == UI_Data.mode) {
        if (UI_Data.pwm_auto_mode) {
            UI_Data.pwm_duty += 10;
            UI_Data.pwm_duty =  UI_Data.pwm_duty >= PWM_CMP_60 ? PWM_CMP_60 : UI_Data.pwm_duty;
            PWM_SetValue(UI_Data.pwm_duty);
        }
    } else if (M_MODE_1 == UI_Data.mode) {
    } else if (M_MODE_2 == UI_Data.mode) {
    } else if (M_MODE_3 == UI_Data.mode) {
    } else if (M_MODE_4 == UI_Data.mode) {
    } else if (M_MODE_5 == UI_Data.mode) {
    }
}

