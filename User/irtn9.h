#ifndef __TH9_H__
#define __TH9_H__

#include "stm8s_gpio.h"

#define TN9_Port GPIOD
#define TN9_ACQ GPIO_PIN_1
#define TN9_CLK GPIO_PIN_2
#define TN9_DATA GPIO_PIN_3


//static const uint8_t IRTEMP_DATA_SIZE = 5;
#define IRTEMP_DATA_SIZE  (5)

static const long IRTEMP_TIMEOUT = 2000;  // milliseconds

#define IRTEMP_DATA_IR (0x4C)
#define IRTEMP_DATA_AMBIENT (0x66)
#define IRTEMP_DATA_JUNK (0x53)


// Each 5-byte data packet from the IRTemp is tagged with one of these
//static const long IRTEMP_DATA_AMBIENT = 0x66;
//static const long IRTEMP_DATA_IR =      0x4C;
//static const long IRTEMP_DATA_JUNK =    0x53; // ignored, contains version info perhaps?


void TN9_Init(void);


float getIRtemp(uint8_t dataType);




#endif
