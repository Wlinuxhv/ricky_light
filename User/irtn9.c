#include "irtn9.h"
#include "USART.h"
#include "stdio.h"


static float decodeTemperature(volatile uint8_t data[]) 
{
  int msb = data[1] << 8;
  int lsb = data[2];

  return((msb + lsb) / 16.0 - 273.15);
}

static void sensorEnable(void) 
{
    if(GPIO_ReadInputPin(TN9_Port,TN9_ACQ)!= RESET)
        GPIO_WriteLow(TN9_Port,TN9_ACQ);
}

static void sensorDisable(void) 
{
    if(GPIO_ReadInputPin(TN9_Port,TN9_ACQ)!= SET)
        GPIO_WriteHigh(TN9_Port,TN9_ACQ);
}



static uint8_t validData(uint8_t data[]) 
{
  uint8_t checksum = (data[0] + data[1] + data[2]) & 0xff;
  return (data[3] == checksum  &&  data[4] == '\r');
}

void TN9_Init(void)
{
    GPIO_DeInit(TN9_Port);
    GPIO_Init(TN9_Port,TN9_ACQ, GPIO_MODE_OUT_OD_LOW_SLOW);
    GPIO_Init(TN9_Port,TN9_CLK, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(TN9_Port,TN9_DATA,GPIO_MODE_IN_FL_NO_IT);
    sensorDisable();
}


float getIRtemp(uint8_t dataType)
{

  long timeout = IRTEMP_TIMEOUT;
  int i;

  sensorEnable();

  while(1) 
  {
    uint8_t data[5] = { 0 };
    for(uint8_t data_byte = 0; data_byte < 5; data_byte++) 
    {
        for(int8_t data_bit = 7; data_bit >= 0; data_bit--) 
        {
            // Clock idles high, data changes on falling edge, sample on rising edge
            while(GPIO_ReadInputPin(TN9_Port,TN9_CLK)) 
            {
                for (i=0;i<10;i++)
                    delay_us();
//                timeout--;
//                printstr("1x\n");
            } // Wait for falling edge

            while(!GPIO_ReadInputPin(TN9_Port,TN9_CLK)) 
            {
                for (i=0;i<10;i++)
                    delay_us();
//                timeout--;
//                printstr("2x\n");
            } // Wait for rising edge to sample
            
            if(GPIO_ReadInputPin(TN9_Port,TN9_DATA))
            {
                print("data_bit:");
                printdec(data_bit);
                print("\n");
                data[data_byte] |= 1<<data_bit;
            }  
        }
        print("data:");
        printhex(data[data_byte]);
        print("\n");
        
    }

    print("------------------------------------------------\n");
    sensorDisable();

    if(timeout <= 0) {
      printstr("timeout sensorDisable\n");
      return 0;
    }

        print("dataType:");
        printhex(data[0]);
        print("\n");

    if (data[0] == dataType && validData(data)) 
    {
      printstr("decodeTemperature sensorDisable\n");
      float temperature = decodeTemperature(data);
      return temperature;
    }
    print("------------------------------------------------\n");
  }
}


