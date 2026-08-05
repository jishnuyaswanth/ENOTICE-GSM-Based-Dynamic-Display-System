#include "uart.h"
#include "types.h"
#include <string.h>
#include <LPC21xx.H>
#include "gsm.h"
#include "dot_matrix.h"
#include "delay.h"
#include "uart.h"
//#include "func.h"
extern u8 auth_number[];
extern u32 index;
extern u8 rx_buf[400];

//Initializing the GSM Module
void gsm_init(void)
{
                index=0;
                memset(rx_buf,'\0',300);
                display_str(" AT ",1000);

                //Checks for reply
                UART0_Str("AT\r\n");
                //while(index<3);
                delay_ms(1000);
                rx_buf[index]='\0';
                if(strstr((char*)rx_buf,"OK"))
                {
                        display_str(" OK ",1000);
                        delay_ms(500);
                        index=0;
                        memset(rx_buf,'\0',300);
                        display_str("ATE0",1000);

                        //Removing Echo
                        UART0_Str("ATE0\r\n");
                //      while(index<3);
                        delay_ms(1000);
                         rx_buf[index]='\0';
			                 if(strstr((s8*)rx_buf,"OK"))
                        {
                                display_str(" OK ",1000);
                            delay_ms(500);
                                index=0;
                                memset(rx_buf,'\0',300);
                                display_str("CMGF",1000);

                                //Set to SMS Mode
                                UART0_Str("AT+CMGF=1\r\n");
                        //      while(index<3);
                                delay_ms(1000);
                                rx_buf[index]='\0';
                                if(strstr((s8*)rx_buf,"OK"))
                                {
                                        index=0;
                                        memset(rx_buf,0,300);
                                        display_str(" OK ",1000);
                                        delay_ms(500);

                                        //Deleting the SMS in slot 1
                                UART0_Str("AT+CMGD=1\r\n");
                                //       while(index<3);
                                        delay_ms(1000);
                                        rx_buf[index]='\0';
                                        if(strstr((s8*)rx_buf,"OK") || strstr((s8*)rx_buf,"ERROR"))
                                        {
                                index=0;
                                        memset(rx_buf,'\0',300);
                                        display_str("CNMI",1000);


                                        //Set to Text Mode
                                        UART0_Str("AT+CNMI=2,1,0,0,0\r\n");
                                //      while(index<3);
                                        delay_ms(1500);
                                        rx_buf[index]='\0';
					                                 if(strstr((char*)rx_buf,"OK"))
                                          {
                                                display_str(" OK ",1000);
                                                delay_ms(500);
                                            Display("   GSM INIT SUCCESS  ");

                                          }
                                           else
                                             Display("  CNMI ERROR  ");

                        }
                        else
                                 Display("  DELETE ERROR  ");

             }
                else
                    Display("  CMGF ERROR  ");

           }
            else
               Display("  ATE0 ERROR  ");

           }
           else
                Display(" AT ERROR ");
}

//Send SMS to auth_number
void send_sms(u8 *num, u8 *info,u8 *msg)
{
            index=0;
                        memset(rx_buf,'\0',300);
                        UART0_Str("AT+CMGS=\"");
                        UART0_Str(num);
                        UART0_Str("\"\r\n");
                        index=0;
                        memset(rx_buf,'\0',300);
                        UART0_Str(info);
                        UART0_Str(msg);
                        delay_ms(100);
                        UART0_Tx(0x1A);
                        delay_ms(5000);

                        while(index<3);
                  delay_ms(2000);

                        rx_buf[index]='\0';
                        if(strstr((s8*)rx_buf,"OK"))
                        {
                                //msg sent success
                        }
                        else
                                Display("   GSM SENT ERROR   ");

}


//Deleting the SMS in slot1
void delete_sms(void)
{
    index=0;
        memset(rx_buf,0,300);
    UART0_Str("AT+CMGD=1\r\n");
    //while(index<3);
        delay_ms(500);
        rx_buf[index]='\0';
        if(strstr((s8*)rx_buf,"OK"));
        else
                Display("   GSM DELETE ERROR   ");

}


//Reading the SMS in slot 1 and storing in buffer
void Read_sms(u8 *sms)
{

   index=0;
   memset(rx_buf,'\0',300);
   UART0_Str("AT+CMGR=1\r\n");
   delay_ms(1000);
  // while(index<3);
   rx_buf[index]='\0';
   if(strstr((s8*)rx_buf,"OK"))
   {
        strcpy((char*)sms,(char*)rx_buf);
        memset(rx_buf,'\0',300);
   }
   else
        Display("   GSM READ ERROR   ");
}

//Sending AT command frequently to check if it is connected
void Check_gsm()
{
        index=0;
        memset(rx_buf,'\0',300);
        UART0_Str("AT\r\n");
        delay_ms(300);
//      while(index<2);
        rx_buf[index]='\0';
        if(strstr(rx_buf,"OK"))
        {
                memset(rx_buf,'\0',300);
                index=0;
        }
        else
        {
                Display("   GSM DISCONNECTED   ");
	}
}
