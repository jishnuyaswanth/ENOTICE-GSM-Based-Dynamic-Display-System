#include "uart_def.h"
#include <LPC21xx.H>
#include "types.h"
#include <string.h>
#include "delay.h"
#include "func.h"

u8 rx_buf[400];
u8 new_var;
u32 index;
u8 sms_flag=0,t_flag,ch;

//UART Interrupt ISR
void UART0_interrupt(void) __irq
{
        new_var=U0IIR;
        if((new_var & 0x04))
        {
                ch=U0RBR;

                if(index<300)
                {
                        rx_buf[index++]=ch;

                        //Checks if new SMS is Received
                        if(strstr((s8*)rx_buf,"+CMTI"))
                        {
                           sms_flag=1;
                        }
                }
        }
        else if(new_var &0x02)
        {
                t_flag=1;
        }
        VICVectAddr=0;

}

//Initializing and Configuring the Settings for UART
void UART0_Init()
{
        PINSEL0 |= (TXD0_EN | RXD0_EN);

        U0LCR = (WORD_LEN_SEL | (1<<DLAB_BIT));

        U0DLL = 97;

        U0DLM = 0;

        U0LCR &= ~(1<<DLAB_BIT);

        VICVectAddr0 = (unsigned)UART0_interrupt;

        VICVectCntl0 = 0x20 | UART0_CHNO;

        VICIntEnable = 1<<UART0_CHNO;

        U0IER = RBR_INTERRUPT_EN|THRE_INTERRUPT_EN;
}

//Transmitting a Byte
void UART0_Tx(u8 ch)
{

        t_flag = 0;
        U0THR = ch;
        while(t_flag == 0);

}

//Transmitting a String Byte by Byte
void UART0_Str(u8 *str)
{
        while(*str)
                UART0_Tx(*str++);

}

//Receive Data Byte
char UART0_Rx(void)
{

  while (!(U0LSR & 0x01));

  return (U0RBR);

}
