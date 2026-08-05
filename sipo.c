#include <LPC21xx.H>
#include "delay.h"
#include "defines.h"
#include "types.h"

// 74HC164 Shift Register-1 Connections
#define SIN_1 16                     // Serial Data Input pin of Shift Register-1
#define CP_1  17                     // Clock Pulse pin of Shift Register-1

// 74HC164 Shift Register-2 Connections
#define SIN_2 18                     // Serial Data Input pin of Shift Register-2
#define CP_2  19                     // Clock Pulse pin of Shift Register-2

// 74HC164 Shift Register-3 Connections
#define SIN_3 20                     // Serial Data Input pin of Shift Register-3
#define CP_3  21                     // Clock Pulse pin of Shift Register-3

// 74HC164 Shift Register-4 Connections
#define SIN_4 22                     // Serial Data Input pin of Shift Register-4
#define CP_4  23                     // Clock Pulse pin of Shift Register-4

// Initialize all GPIO pins connected to the 74HC164 shift registers
void Init_SIPO_74HC164(void)
{
    WRITEBYTE(IODIR0, SIN_1, 0xFF);  // Configure P0.16 to P0.23 as output pins
}


// Send one byte of data to Shift Register-1
void SIPO_74HC164_1(u8 sDat)
{
    u8 i;                                            // Loop variable

    for(i=0; i<8; i++)                               // Transmit all 8 bits
    {
        WRITEBIT(IOPIN0, SIN_1, ((sDat>>(7-i))&1));  // Send one data bit (MSB first)

        IOCLR0 = 1<<CP_1;                            // Make clock LOW

        delay_us(1);                                // Small delay for signal stabilization

        IOSET0 = 1<<CP_1;                            // Generate clock HIGH (rising edge shifts data)

        delay_us(1);                                // Small delay before next bit
    }
}


// Send one byte of data to Shift Register-2
void SIPO_74HC164_2(u8 sDat)
{
    u8 i;                                            // Loop variable

    for(i=0; i<8; i++)                               // Transmit all 8 bits
    {
        WRITEBIT(IOPIN0, SIN_2, ((sDat>>(7-i))&1));  // Send one data bit (MSB first)

        IOCLR0 = 1<<CP_2;                            // Make clock LOW

        delay_us(1);                                // Small delay

        IOSET0 = 1<<CP_2;                            // Generate rising edge of clock

        delay_us(1);                                // Small delay
    }
}


// Send one byte of data to Shift Register-3
void SIPO_74HC164_3(u8 sDat)
{
    u8 i;                                            // Loop variable

    for(i=0; i<8; i++)                               // Transmit all 8 bits
    {
        WRITEBIT(IOPIN0, SIN_3, ((sDat>>(7-i))&1));  // Send one data bit (MSB first)

        IOCLR0 = 1<<CP_3;                            // Make clock LOW

        delay_us(1);                                // Small delay

        IOSET0 = 1<<CP_3;                            // Generate rising edge of clock

        delay_us(1);                                // Small delay
    }
}


// Send one byte of data to Shift Register-4
void SIPO_74HC164_4(u8 sDat)
{
    u8 i;                                            // Loop variable

    for(i=0; i<8; i++)                               // Transmit all 8 bits
    {
        WRITEBIT(IOPIN0, SIN_4, ((sDat>>(7-i))&1));  // Send one data bit (MSB first)

        IOCLR0 = 1<<CP_4;                            // Make clock LOW

        delay_us(1);                                // Small delay

        IOSET0 = 1<<CP_4;                            // Generate rising edge of clock

        delay_us(1);                                // Small delay
    }
}