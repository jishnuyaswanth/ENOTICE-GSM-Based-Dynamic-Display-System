#include "uart.h"
#include "types.h"
#include <string.h>
#include <LPC21xx.H>
#include "gsm.h"
#include "dot_matrix.h"
#include "delay.h"
#include "uart.h"

extern u8 auth_number[];           // Stores the authorized mobile number
extern u32 index;                  // UART receive buffer index
extern u8 rx_buf[400];             // UART receive buffer

// Initialize and configure the GSM module
void gsm_init(void)
{
    index = 0;                                     // Reset receive buffer index
    memset(rx_buf, '\0', 300);                     // Clear UART receive buffer
    display_str(" AT ",1000);                      // Display "AT" initialization message

    UART0_Str("AT\r\n");                           // Check GSM module communication
    delay_ms(1000);                               // Wait for GSM response
    rx_buf[index] = '\0';                          // Null terminate received data

    if(strstr((char*)rx_buf,"OK"))                // Check if GSM replied with "OK"
    {
        display_str(" OK ",1000);                  // Display successful response
        delay_ms(500);                             // Small delay

        index = 0;                                 // Reset receive index
        memset(rx_buf,'\0',300);                   // Clear receive buffer
        display_str("ATE0",1000);                  // Display ATE0 command

        UART0_Str("ATE0\r\n");                     // Disable GSM command echo
        delay_ms(1000);                            // Wait for response
        rx_buf[index]='\0';                        // Null terminate buffer

        if(strstr((s8*)rx_buf,"OK"))              // Verify ATE0 executed successfully
        {
            display_str(" OK ",1000);              // Display success
            delay_ms(500);

            index=0;                               // Reset receive index
            memset(rx_buf,'\0',300);               // Clear receive buffer
            display_str("CMGF",1000);              // Display CMGF command

            UART0_Str("AT+CMGF=1\r\n");            // Configure GSM in SMS text mode
            delay_ms(1000);                        // Wait for response
            rx_buf[index]='\0';                    // Null terminate buffer

            if(strstr((s8*)rx_buf,"OK"))          // Verify text mode enabled
            {
                index=0;                           // Reset receive index
                memset(rx_buf,0,300);             // Clear receive buffer
                display_str(" OK ",1000);          // Display success
                delay_ms(500);

                UART0_Str("AT+CMGD=1\r\n");        // Delete SMS stored in slot 1
                delay_ms(1000);                   // Wait for response
                rx_buf[index]='\0';               // Null terminate buffer

                if(strstr((s8*)rx_buf,"OK") || strstr((s8*)rx_buf,"ERROR")) // Continue even if slot is already empty
                {
                    index=0;                       // Reset receive index
                    memset(rx_buf,'\0',300);      // Clear receive buffer
                    display_str("CNMI",1000);     // Display CNMI command

                    UART0_Str("AT+CNMI=2,1,0,0,0\r\n"); // Enable new SMS indication through UART
                    delay_ms(1500);                    // Wait for GSM response
                    rx_buf[index]='\0';                // Null terminate received data

                    if(strstr((char*)rx_buf,"OK"))     // Verify CNMI command
                    {
                        display_str(" OK ",1000);      // Display success
                        delay_ms(500);                 // Small delay
                        Display("   GSM INIT SUCCESS  "); // Show GSM initialized successfully
                    }
                    else
                    {
                        Display("  CNMI ERROR  ");      // Display CNMI configuration error
                    }
                }
                else
                {
                    Display("  DELETE ERROR  ");        // SMS deletion failed
                }
            }
            else
            {
                Display("  CMGF ERROR  ");              // Failed to enter SMS text mode
            }
        }
        else
        {
            Display("  ATE0 ERROR  ");                  // Failed to disable echo
        }
    }
    else
    {
        Display(" AT ERROR ");                          // GSM module not responding
    }
}

// Send an SMS
void send_sms(u8 *num,u8 *info,u8 *msg)
{
    index=0;                                      // Reset receive buffer index
    memset(rx_buf,'\0',300);                      // Clear receive buffer

    UART0_Str("AT+CMGS=\"");                      // Send SMS command
    UART0_Str(num);                               // Send destination mobile number
    UART0_Str("\"\r\n");                          // Complete CMGS command

    index=0;                                      // Reset receive index
    memset(rx_buf,'\0',300);                      // Clear receive buffer

    UART0_Str(info);                              // Send SMS heading
    UART0_Str(msg);                               // Send SMS content

    delay_ms(100);                                // Wait before ending SMS

    UART0_Tx(0x1A);                               // Send Ctrl+Z to transmit SMS

    delay_ms(5000);                               // Wait for GSM to send SMS

    while(index<3);                               // Wait until response is received

    delay_ms(2000);                               // Additional delay

    rx_buf[index]='\0';                           // Null terminate response

    if(strstr((s8*)rx_buf,"OK"))                 // Check if SMS sent successfully
    {
        // SMS sent successfully
    }
    else
    {
        Display("   GSM SENT ERROR   ");          // Display SMS sending error
    }
}

// Delete SMS stored in slot 1
void delete_sms(void)
{
    index=0;                                      // Reset receive index
    memset(rx_buf,0,300);                         // Clear receive buffer

    UART0_Str("AT+CMGD=1\r\n");                   // Delete SMS from slot 1

    delay_ms(500);                                // Wait for response

    rx_buf[index]='\0';                           // Null terminate response

    if(strstr((s8*)rx_buf,"OK"));                // Check if deletion succeeded

    else
        Display("   GSM DELETE ERROR   ");        // Display deletion error
}

// Read SMS from slot 1
void Read_sms(u8 *sms)
{
    index=0;                                      // Reset receive index

    memset(rx_buf,'\0',300);                      // Clear receive buffer

    UART0_Str("AT+CMGR=1\r\n");                   // Read SMS from memory slot 1

    delay_ms(1000);                               // Wait for GSM response

    rx_buf[index]='\0';                           // Null terminate response

    if(strstr((s8*)rx_buf,"OK"))                 // Check whether SMS was read successfully
    {
        strcpy((char*)sms,(char*)rx_buf);         // Copy SMS into destination buffer

        memset(rx_buf,'\0',300);                  // Clear receive buffer
    }
    else
    {
        Display("   GSM READ ERROR   ");          // Display read failure
    }
}

// Check whether GSM module is connected
void Check_gsm()
{
    index=0;                                      // Reset receive index

    memset(rx_buf,'\0',300);                      // Clear receive buffer

    UART0_Str("AT\r\n");                          // Send basic AT command

    delay_ms(300);                                // Wait for response

    rx_buf[index]='\0';                           // Null terminate response

    if(strstr(rx_buf,"OK"))                      // GSM is responding
    {
        memset(rx_buf,'\0',300);                  // Clear receive buffer

        index=0;                                  // Reset receive index
    }
    else
    {
        Display("   GSM DISCONNECTED   ");        // GSM module is not responding
    }
}