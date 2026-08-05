#include "types.h"
#include <LPC21xx.H>
#include <string.h>

#include "delay.h"
#include "defines.h"
#include "sipo_74hc164.h"
#include "dot_matrix_def.h"
#include "dot_matrix.h"
#include "func.h"

extern u8 sms_flag;                    // Flag indicating arrival of a new SMS

// Initialize GPIO pins required for the Dot Matrix Display
void Init_dm_rows(void)
{
    Init_SIPO_74HC164();               // Initialize all four 74HC164 shift registers

    WRITEBYTE(IODIR0,ROWS,0xFF);       // Configure all row pins as output pins
}


// Display four characters simultaneously on four 8x8 dot matrix displays
void display_str(u8 *str,u32 delay)
{
    u8 i;                              // Row counter

    u32 d;                             // Display refresh counter

    for(d=0; d<delay; d++)             // Refresh the display for the specified duration
    {
        for(i=0; i<8; i++)             // Scan each of the 8 rows
        {
            if(sms_flag==1)            // Stop displaying immediately if a new SMS is received
                break;

            WRITEBYTE(IOPIN0,ROWS,0x00);      // Turn OFF all rows before updating display

            SIPO_74HC164_1(font[str[0]-32][i]); // Send row pattern of first character to Display-1

            SIPO_74HC164_2(font[str[1]-32][i]); // Send row pattern of second character to Display-2

            SIPO_74HC164_3(font[str[2]-32][i]); // Send row pattern of third character to Display-3

            SIPO_74HC164_4(font[str[3]-32][i]); // Send row pattern of fourth character to Display-4

            WRITEBYTE(IOPIN0,ROWS,(1<<i));     // Enable only the current row

            delay_us(40);                      // Hold the row ON briefly for persistence of vision
        }
    }
}


// Scroll the complete message across the dot matrix display
void Display(u8 *ptr)
{
    s32 j;                            // Scroll position index

    u32 len;                          // Stores length of the message

    len = my_strlen(ptr);             // Calculate the total message length

    for(j=0; j<len-3; j++)            // Move one character at a time for scrolling
    {
        if(sms_flag==1)               // Stop scrolling if a new SMS arrives
            break;

        display_str(ptr+j,500);       // Display four consecutive characters starting from position j
    }
}


// Calculate the length of a string
u32 my_strlen(u8 *str)
{
    u32 i, cnt=0;                     // i = loop index, cnt = character counter

    for(i=0; str[i]!='\0'; i++)       // Traverse until the null terminator is found
        cnt++;                        // Increment string length

    return cnt;                       // Return total number of characters
}