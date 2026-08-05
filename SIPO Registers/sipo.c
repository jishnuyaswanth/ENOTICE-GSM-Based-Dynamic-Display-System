#include <LPC21xx.H>
#include "delay.h"
#include "defines.h"
#include "types.h"
//74HC164 SIPO shift register1 connections
#define SIN_1 16
#define CP_1  17

//74HC164 SIPO shift register2 connections
#define SIN_2 18
#define CP_2  19

//74HC164 SIPO shift register3 connections
#define SIN_3 20
#define CP_3  21

//74HC164 SIPO shift register4 connections
#define SIN_4 22
#define CP_4  23

void Init_SIPO_74HC164(void)
{

         //Output Configuration
        WRITEBYTE(IODIR0,SIN_1,0xff);
}

void SIPO_74HC164_1(u8 sDat)
{
        u8 i;

        //Writing one Byte of Data bit by bit
        for(i=0;i<8;i++)
        {
          WRITEBIT(IOPIN0,SIN_1,((sDat>>(7-i))&1));
          IOCLR0 = 1<<CP_1;
          delay_us(1);
          IOSET0 = 1<<CP_1;
          delay_us(1);
        }
}

void SIPO_74HC164_2(u8 sDat)
{
        u8 i;
        //Writing one Byte of Data bit by bit
        for(i=0;i<8;i++)
        {
          WRITEBIT(IOPIN0,SIN_2,((sDat>>(7-i))&1));
          IOCLR0 = 1<<CP_2;
          delay_us(1);
          IOSET0 = 1<<CP_2;
          delay_us(1);
        }
}

void SIPO_74HC164_3(u8 sDat)
{
        u8 i;
        //Writing one Byte of Data bit by bit
        for(i=0;i<8;i++)
        {
         WRITEBIT(IOPIN0,SIN_3,((sDat>>(7-i))&1));
         IOCLR0 = 1<<CP_3;
         delay_us(1);
         IOSET0 = 1<<CP_3;
         delay_us(1);
        }
}

void SIPO_74HC164_4(u8 sDat)
{
        u8 i;
        //Writing one Byte of Data bit by bit
        for(i=0;i<8;i++)
        {
         WRITEBIT(IOPIN0,SIN_4,((sDat>>(7-i))&1));
         IOCLR0 = 1<<CP_4;
         delay_us(1);
         IOSET0 = 1<<CP_4;
         delay_us(1);
        }
}
