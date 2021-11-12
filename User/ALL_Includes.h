#ifndef  _ALL_Includes_H
#define  _ALL_Includes_H

#include "stm8s.h"
#include "mytype.h"
#include "led.h"
#include "delay.h"

#include "USART.h"
#include "oled.h"
//#include "bmp.h"
#include "spi.h"
#include "RF24L01.h"
#include "ui.h"

//#include "irtn9.h"
#include "irDA.h"
#include "pwm.h"
/* #define STM8S103 */      /*!< STM8S Low density devices */
//USART1_RX PD5
//USART1_TX PD6
//LED PB5
//PWM PC4
//IRDA PA1

extern uint8_t g_debug;

//#define IRDA     C7
//#define LED      B5  //FIXED
//#define PWM      C4
//#define TN9_ACQ  D1
//#define TN9_CLK  D2
//#define TN9_DATA D3

//#define OLED_SCL  D1
//#define OLED_SDA  A2

//#define UART1 TX D5
//#define UART1 RX D6
/*    
	线路连接：  RF24L01： VCC        3.3V
			GND        GND
			CE         D3
			IRQ        D2
            CSN        A3
			SCK        C5 //FIXED
			MOSI       C6 //FIXED
			MISO       C7 //FIXED

*/

#endif

