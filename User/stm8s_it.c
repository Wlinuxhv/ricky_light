/**
  ******************************************************************************
  * @file    stm8s_it.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service 
  *          routine.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy Interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#include "ALL_Includes.h"

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  Ir_Receive_Handler(); 
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  //IR_rx();
  //disableInterrupts();
  //Ir_Receive_Handle(); 
  //enableInterrupts();

  g_debug = (g_debug + 1)%255;
}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x) 
/**
  * @brief External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8AF52Ax) */

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    if(TIM1_GetITStatus(TIM1_IT_UPDATE)!=RESET)
    {
        TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
        //IR_Time++;
        //TIM1_SetCounter(0);

    }
}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
 
/**
  * @brief Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
/**
  * @brief Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  //TIM_IT_Updata_Handdle();
 }

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
#include "USART.h"

 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
    if(UART1_GetITStatus(UART1_IT_RXNE) != RESET) //�����жϣ�ÿ����һ���ֽڣ�8λ���������ݣ����ж�һ��,ÿ���жϣ���ÿ����һ�����ݣ���ִ��һ�����г���
    {
        UART1_ClearITPendingBit(UART1_IT_RXNE);
    }
}
#ifdef WL_UART
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    u8 i,res,check_temp;
    if(UART1_GetITStatus(UART1_IT_RXNE) != RESET) //�����жϣ�ÿ����һ���ֽڣ�8λ���������ݣ����ж�һ��,ÿ���жϣ���ÿ����һ�����ݣ���ִ��һ�����г���
    {
        UART1_ClearITPendingBit(UART1_IT_RXNE);
        res= UART1_ReceiveData8();                   //��ȡ���յ�������
        //printstr("[DEBUG] raw :");
        //printhex(res);
        //printstr("\n----------------------------------\n");

        if((res==0xee)&&(usart_rx_enableflag==0))         //�����ܵ���ͷ(0xee)���ݲ��һ�û�гɹ�������������Ϣ
        {/*head*/
            usart_rx_enableflag=1;                        //˵�����ǰ�ͷ�������������ݱ�־���������ݽ��ս׶�
            //printstr("[DEBUG] head :");
            //printhex(res);
            usart_rx_lenrightflag=0;               //���ݳ���У������
            usart_rx_successflag=0;                //���ݽ��ճɹ���־����
            usart_rd_len=0;                         //���ݳ�������
            usart_rd_lentemp=0;
            //printstr("\n----------------------------------\n");
        }
        else if(usart_rx_enableflag==1)                        //���������ݱ�־��λʱ����������
        {/*a frame*/            
            //printstr("[DEBUG] usart_rd_lentemp :");
            //printhex(usart_rd_lentemp);
            //printstr("\n----------------------------------\n");

            if(usart_rd_lentemp==0)                       //��ͷ���һ������Ϊ��Ҫ��������ݵĳ���
            {/*paylen*/
                usart_rd_len=res;                         //��ȡ���ݵĳ��� 
                //printstr("[DEBUG] usart_rd_len :");
                //printdec(usart_rd_len);
                
                if(usart_rd_len >= UART_BUF_REG_LEN)
                {
                    usart_rx_lenrightflag=0;    //���ݳ���У������
                    usart_rx_successflag=0;     //���ݽ��ճɹ���־����
                    Uart1_Sendstr("[ERROR]Paylen too long receive failed\n");
                    usart_rx_enableflag=0;      //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len=0;             //���ݳ�������
                    usart_rd_lentemp=0;         //���ݳ����ݴ�������                                                                                
                }
                //printstr("\n----------------------------------\n");
            }
            else if(usart_rd_lentemp==usart_rd_len+1)     //����ȡ����usart_rd_lentemp+1������ʱ��У���Ƿ��ǳ�����Ϣ�ķ���
            {/*У�����ݳ��ȵ�ȡ��*/
                check_temp=~usart_rd_len;                 //ȡ���ݳ���У��λ�ķ���
                printstr("[DEBUGc] check_temp :");
                printdec(check_temp);
                printstr("\n");
                printstr("[DEBUGc] res :");
                printdec(res);
                
                if(res==check_temp)                       //�����ݳ���У����ȷʱ
                {
                    usart_rx_lenrightflag=1;              //���ݳ���У���־��һ
                }
                else                                                                                     
                {                                          //�����ݳ���У�����ʱ
                    usart_rx_lenrightflag=0;               //���ݳ���У������
                    usart_rx_successflag=0;                //���ݽ��ճɹ���־����
                    usart_rx_enableflag=0;                  //�����ݳ���У�����ʱ�����ݽ���������־����
                    usart_rd_len=0;                         //���ݳ�������
                    usart_rd_lentemp=0;                     //���ݳ����ݴ�������
                    Uart1_Sendstr("[ERROR]Checksum failed receive failed\n");
                }
                //printstr("\n----------------------------------\n");
            }
            else if(usart_rd_lentemp == usart_rd_len+2)       //����ȡ����usart_rd_lentemp+2������ʱ��У���β�Ƿ���ȷ
            {/*tail*/
                //printstr("the tail");
                if((res==0xef)&&(usart_rx_lenrightflag==1)) //�����β�����볤��У�鶼��ȷ
                {
                    usart_rx_lenrightflag=0;                //���ݳ���У������
                    usart_rx_successflag=1;                 //���ݽ��ճɹ���־��һ
                    usart_rx_enableflag=0;                  //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len=0;                         //���ݳ�������
                    usart_rd_lentemp=0;                     //���ݳ����ݴ�������                                        
                    Uart1_Sendstr("[NOTICE]receive sucess\n");
                    //printstr("\n----------------------------------\n");
                }
                else 
                {                                          //����β����У�����ʱ
                    usart_rx_lenrightflag=0;                //���ݳ���У������
                    usart_rx_successflag=0;                //���ݽ��ճɹ���־����

                    usart_rx_enableflag=0;                 //���ݽ�����ɣ����ݽ���������־����
                    usart_rd_len=0;                        //���ݳ�������
                    usart_rd_lentemp=0;                    //���ݳ����ݴ�������                                        
                    Uart1_Sendstr("[ERROR]receive failed\n");

                }
                //printstr("\n----------------------------------\n");
            }
            else
            {
                //printstr("the storing");
                //printstr("\n----------------------------------\n");
                usart_rx_temporary[usart_rd_lentemp-1] = res;    //��usart_rd_lentempΪ���ݶ�ʱ�������ݴ浽�������ݽ��ռĴ�����    
            }

            usart_rd_lentemp++; //ÿ�μ�¼���ݣ����ݳ����ݴ����Լ�    

            if(usart_rx_successflag==1)                    //����ɹ����յ���Ϣ���ݣ�������usart_rx_temporary[]�ڵ����ݴ��ݸ�usart_rx_buf[][]
            {
                for(i=0;i<usart_rd_len;i++)
                    usart_rx_buf[usartrxbuf_pagebuf][i]=usart_rx_temporary[i];
                usartrxbuf_pagebuf++;                              //���ܻ����ҳ�뻺���Լ�
                if(usartrxbuf_pagebuf==UART_BUF_REG_NUM) 
                {
                    usartrxbuf_pagebuf=0;    //���ܻ����ҳ�뻺�����Ϊ5
                }
                usartrxbuf_pagebuf++;
                usart_rd_lentemp = 0;
                usart_rx_successflag = 0;
            }            
        }
    }
}
#endif /* WL_UART */

#if 0
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    u8 i,res,check_temp;
    Uart1_Sendstr("[NOTICE]INTERRUPT_HANDLER in\n");
    if(UART1_GetITStatus(UART1_IT_RXNE) != RESET) //�����жϣ�ÿ����һ���ֽڣ�8λ���������ݣ����ж�һ��,ÿ���жϣ���ÿ����һ�����ݣ���ִ��һ�����г���
#if 1
    {
		res=UART1_ReceiveData8();                    //��ȡ���յ�������
		if((res==0xee)&&(usart_rx_enableflag==0))         //�����ܵ���ͷ(0xee)���ݲ��һ�û�гɹ�������������Ϣ
		{/*head*/
		    usart_rx_enableflag=1;                        //˵�����ǰ�ͷ�������������ݱ�־���������ݽ��ս׶�
        }
        
		if(usart_rx_enableflag==1)                        //���������ݱ�־��λʱ����������
		{/*a frame*/            
			res=UART1_ReceiveData8();                //��ȡ���ڱ�־
			if(usart_rd_lentemp==0)                       //��ͷ���һ������Ϊ��Ҫ��������ݵĳ���
			{/*paylen*/
				usart_rd_len=res;                         //��ȡ���ݵĳ��� 
				if(usart_rd_len>=UART_BUF_REG_LEN)
				{
					usart_rx_lenrightflag=0;    //���ݳ���У������
					usart_rx_successflag=0;     //���ݽ��ճɹ���־����
                    Uart1_Sendstr("[ERROR]receive failed\n");
					usart_rx_enableflag=0;      //���ݽ�����ɣ����ݽ���������־����
					usart_rd_len=0;             //���ݳ�������
					usart_rd_lentemp=0;         //���ݳ����ݴ�������                                                                                
				}
			}
			else if(usart_rd_lentemp==usart_rd_len+1)     //����ȡ����usart_rd_lentemp+1������ʱ��У���Ƿ��ǳ�����Ϣ�ķ���
			{/*У�����ݳ��ȵ�ȡ��*/
				check_temp=~usart_rd_len;                 //ȡ���ݳ���У��λ�ķ���
				if(res==check_temp)                       //�����ݳ���У����ȷʱ
				{
                    usart_rx_lenrightflag=1;              //���ݳ���У���־��һ
                }
				else                                                                                     
				{                                          //�����ݳ���У�����ʱ
					usart_rx_lenrightflag=0;               //���ݳ���У������
					usart_rx_successflag=0;                //���ݽ��ճɹ���־����
                    Uart1_Sendstr("[ERROR]receive failed\n");
                    usart_rx_enableflag=0;                  //�����ݳ���У�����ʱ�����ݽ���������־����
					usart_rd_len=0;                         //���ݳ�������
					usart_rd_lentemp=0;                     //���ݳ����ݴ�������
				}
			}
			else if(usart_rd_lentemp==usart_rd_len+2)       //����ȡ����usart_rd_lentemp+2������ʱ��У���β�Ƿ���ȷ
			{/*tail*/
				if((res==0xef)&&(usart_rx_lenrightflag==1)) //�����β�����볤��У�鶼��ȷ
				{
					usart_rx_lenrightflag=0;                //���ݳ���У������
					usart_rx_successflag=1;                 //���ݽ��ճɹ���־��һ
                    Uart1_Sendstr("[NOTICE]receive sucess\n");
					usart_rx_enableflag=0;                  //���ݽ�����ɣ����ݽ���������־����
					usart_rd_len=0;                         //���ݳ�������
					usart_rd_lentemp=0;                     //���ݳ����ݴ�������                                        
				}
				else 
				{                                          //����β����У�����ʱ
					usart_rx_lenrightflag=0;                //���ݳ���У������
					usart_rx_successflag=0;                //���ݽ��ճɹ���־����
                    Uart1_Sendstr("[ERROR]receive failed\n");
                    usart_rx_enableflag=0;                 //���ݽ�����ɣ����ݽ���������־����
					usart_rd_len=0;                        //���ݳ�������
					usart_rd_lentemp=0;                    //���ݳ����ݴ�������                                        
				}
			}
			else usart_rx_temporary[usart_rd_lentemp-1]=res;    //��usart_rd_lentempΪ���ݶ�ʱ�������ݴ浽�������ݽ��ռĴ�����    
				
			usart_rd_lentemp++;                            //ÿ�μ�¼���ݣ����ݳ����ݴ����Լ�    
			if(usart_rx_successflag==1)                    //����ɹ����յ���Ϣ���ݣ�������usart_rx_temporary[]�ڵ����ݴ��ݸ�usart_rx_buf[][]
			{
				for(i=0;i<usart_rd_len;i++)
					usart_rx_buf[usartrxbuf_pagebuf][i]=usart_rx_temporary[i];
				usartrxbuf_pagebuf++;                              //���ܻ����ҳ�뻺���Լ�
				if(usartrxbuf_pagebuf==UART_BUF_REG_NUM) 
                {
                    usartrxbuf_pagebuf=0;    //���ܻ����ҳ�뻺�����Ϊ5
                }
				usartrxbuf_pagebuf++;
			}            
		}
	}
#endif

}
#endif

 



#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S903) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8AF622x)
/**
  * @brief UART4 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART4 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8AF622x) */

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#else /* STM8S105 or STM8S103 or STM8S903 or STM8AF626x or STM8AF622x */
/**
  * @brief ADC1 interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer6 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#else /* STM8S208 or STM8S207 or STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
  * @brief Timer4 Update/Overflow Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x)*/

/**
  * @brief Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
