#include "uart_def.h"
#include <LPC21xx.H>
#include "types.h"
#include <string.h>
#include "delay.h"
#include "func.h"

u8 rx_buf[400];                    // UART receive buffer to store incoming data
u8 new_var;                        // Stores UART Interrupt Identification Register (U0IIR) value
u32 index;                         // Index for storing received bytes into rx_buf
u8 sms_flag = 0, t_flag, ch;       // sms_flag = New SMS received, t_flag = Transmission complete, ch = Received byte

// UART0 Interrupt Service Routine
void UART0_interrupt(void) __irq
{
    new_var = U0IIR;                           // Read UART Interrupt Identification Register

    if(new_var & 0x04)                         // Check whether Receive Data Available interrupt occurred
    {
        ch = U0RBR;                            // Read one byte from UART Receive Buffer Register

        if(index < 300)                        // Prevent receive buffer overflow
        {
            rx_buf[index++] = ch;              // Store received byte into receive buffer

            if(strstr((s8*)rx_buf, "+CMTI"))   // Check whether GSM reported a new SMS
            {
                sms_flag = 1;                  // Set SMS received flag
            }
        }
    }
    else if(new_var & 0x02)                    // Check whether THRE (Transmit Holding Register Empty) interrupt occurred
    {
        t_flag = 1;                            // Indicate previous transmission is complete
    }

    VICVectAddr = 0;                           // Notify VIC that interrupt servicing is complete
}


// Initialize UART0
void UART0_Init()
{
    PINSEL0 |= (TXD0_EN | RXD0_EN);            // Configure P0.0 and P0.1 as UART0 TXD and RXD pins

    U0LCR = (WORD_LEN_SEL | (1<<DLAB_BIT));    // Select 8-bit data and enable DLAB to access baud rate registers

    U0DLL = 97;                                // Load baud rate divisor low byte (9600 bps for selected PCLK)

    U0DLM = 0;                                 // Load baud rate divisor high byte

    U0LCR &= ~(1<<DLAB_BIT);                   // Disable DLAB and return to normal UART operation

    VICVectAddr0 = (unsigned)UART0_interrupt;  // Load UART0 ISR address into VIC vector slot

    VICVectCntl0 = 0x20 | UART0_CHNO;          // Enable vector slot and assign UART0 interrupt channel

    VICIntEnable = 1<<UART0_CHNO;              // Enable UART0 interrupt in VIC

    U0IER = RBR_INTERRUPT_EN | THRE_INTERRUPT_EN; // Enable Receive and Transmit interrupts
}


// Transmit one byte
void UART0_Tx(u8 ch)
{
    t_flag = 0;                                // Clear transmission complete flag

    U0THR = ch;                                // Load byte into UART Transmit Holding Register

    while(t_flag == 0);                        // Wait until transmission completes
}


// Transmit a string
void UART0_Str(u8 *str)
{
    while(*str)                                // Continue until NULL character is reached
    {
        UART0_Tx(*str++);                      // Send one character and move to the next
    }
}


// Receive one byte 
char UART0_Rx(void)
{
    while(!(U0LSR & 0x01));                    // Wait until Receive Data Ready bit becomes 1

    return U0RBR;                              // Return received byte
}