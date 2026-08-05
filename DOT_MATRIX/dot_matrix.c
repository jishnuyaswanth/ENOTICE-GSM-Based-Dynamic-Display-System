#include "types.h"

#include <LPC21xx.H>

#include <string.h>


#include "delay.h"

#include "defines.h"

#include "sipo_74hc164.h"

#include "dot_matrix_def.h"

#include"dot_matrix.h"

#include "func.h"

extern u8 sms_flag;

void Init_dm_rows(void)
{
        //GPIO Configuration

        Init_SIPO_74HC164();
        WRITEBYTE(IODIR0,ROWS,0xFF);

}


//Displaying 4 different Characters in 4 8*8 dot Matrix Display
void display_str(u8 *str,u32 delay)

{

        u8 i;

        u32 d;

        for(d=0;d<delay;d++)
        {
                for(i=0;i<8;i++)
                {
                        if(sms_flag==1)
                      break;
                        WRITEBYTE(IOPIN0,ROWS,0x00);

                        SIPO_74HC164_1(font[str[0]-32][i]);

                        SIPO_74HC164_2(font[str[1]-32][i]);

                        SIPO_74HC164_3(font[str[2]-32][i]);

                        SIPO_74HC164_4(font[str[3]-32][i]);

                        WRITEBYTE(IOPIN0,ROWS,(1<<i));

                        delay_us(40);

                }
        }
}

//Sending String to display
void Display(u8*ptr)
{
   s32 j;
        u32 len;
        len=my_strlen(ptr);
    for(j=0;j<len-3;j++)
        {
           if(sms_flag==1)
              break;

           display_str(ptr+j,500);

          }
}

//String Length
u32 my_strlen(u8 * str)
{
  u32 i,cnt=0;
        for(i=0;str[i]!='\0';i++)
                 cnt++;
        return cnt;
}

