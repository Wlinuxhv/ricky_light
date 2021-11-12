#include "ALL_Includes.h"
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)


//定义CPU内部时钟
#define  SYS_CLOCK    16
uint8_t g_debug = 0;

/*********************************************
函数功能：系统内部时钟配置
输入参数：SYS_CLK : 2、4、8、16
输出参数：无
备    注：系统启动默认内部2ＭＨＺ
*********************************************/
void CLOCK_Config(u8 SYS_CLK)
{
    //时钟配置为内部RC，16M
    CLK_HSICmd(ENABLE);//HSI = 16MHz
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //f_psc = 16MHz/1
}

void All_Congfig(void)
{
    disableInterrupts();
    CLOCK_Config(SYS_CLOCK);//系统时钟初始化
    UART_Init(SYS_CLOCK, 115200);
    LED_Init();
    PWM_Init();
    IR_Init();
    drv_spi_init();
    //NRF24L01_Gpio_Init();
    //NRF24L01_check();     //检测nRF24L01
    //RF24L01_Init();
    OLED_Init();            //OLED
    OLED_ON();
    enableInterrupts();
    //TN9_Init();
}

int main(void)
{
    int16_t duty = 0;
    int16_t secs = 0;
    uint16_t key_val = 0;
    All_Congfig();
    Uart1_Sendstr("+++++++++hardware init ok+++++++++\n");
    delay_ms(1000);
    Uart1_Sendstr("+++++++++single thread ok+++++++++\n");
    OLED_ShowString(0, 3, "oled is ok");

    while (1) {
        delay_ms(10);
        secs++;
        secs  %= 100;
        key_val = Ir_Process();
        display_key(key_val);
        ui_loop_routine(key_val);
        if (!(secs % 100)) {
            printstr("duty:");
            printdec(duty++);
            printstr("\n");
            OLED_ON();
            OLED_ShowNum(90, 3, duty, 3, 16);
        }

        /*
        检测端
        1. 每隔5秒检测当前浮力开是否闭合
        2. 发送当前的开关状态到控制端

        */
    }
}


