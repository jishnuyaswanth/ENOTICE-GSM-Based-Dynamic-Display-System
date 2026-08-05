#include<string.h>
#include"func.h"
#include "gsm.h"
extern u8 f;
extern u8 auth_number[];

//Verifying the Security Code
int verify_format(u8 *msg,u8 *Author_num)
{
  if(strstr((s8*)msg,(s8*)Author_num))
  {
        if((strstr((s8*)msg,"5665D"))&& (strstr((s8*)msg,"@")))
        {
                        return 1;
        }
        else if((strstr((s8*)msg,"5665M"))&& (strstr((s8*)msg,"@")))
        {
                        return 2;
        }
        else if((strstr((s8*)msg,"5665I@")))
        {
                        return 3;
        }
        else
        {
            return 4;
        }
  }
  else
   return 5;

}

//extracting original content removing security features
void extract_content(u8 *msg1,u8 *output)
{
    u32 i,j=0;
        s8* msg=NULL;
        memset(output,'\0',200);
        msg=strstr((s8*)msg1,"5665D");
        for(i=2;msg[i]!='@';i++)
        {
           if(i<5)
            output[j++]=' ';
           else if(msg[i]=='\r' && msg[i+1]=='\n')
           {
                  output[j++]=' ';
                  i++;
           }
           else if(msg[i]=='\r' || msg[i]=='\n')
           {
                        output[j++]=' ';
           }
           else
                output[j++]=msg[i];
        }
        output[j++]=' ';
        output[j++]=' ';
        output[j++]=' ';
        output[j]='\0';
}

//extracting Number
void extract_num(u8 *msg1,u8 *onum)
{
    u32 i,j=0;
        u8 num[11];
        s8*msg=(s8*)msg1;
        msg=strstr((s8*)msg1,"5665M");
        msg=msg+5;
        for(;*(msg)!='@';msg++)
        {
                if(((*msg)>='0' && (*msg)<='9'))
                {
                        num[j++]=*(msg);
                }
                else
                {
                        f=1;
                        break;
                }
        }
        num[j]='\0';
        if(f==0)
        {
                send_sms(auth_number,"Authorization of Number\n","SUCCESS");
                strcpy(onum,num);
        }
}

//Extracing Invalid Message By Unknown Sender
void Invalid_msg(u8* msg1,u8* invalid_msg)
{
   u32 newline=0,i=0,j=10;
   s8* msg=NULL;
   msg=(s8*)msg1;
   msg=strstr(msg,"UNREAD");
         invalid_msg[j++]='\n';
         invalid_msg[j++]='M';
         invalid_msg[j++]='s';
         invalid_msg[j++]='g';
         invalid_msg[j++]=':';
         msg=msg+52;
     while(*msg!=0x0D)
     {
        invalid_msg[j++]=*msg;
                msg++;
     }
           invalid_msg[j]='\0';
}

//Extracing Invalid Number By Unknown Sender
void Invalid_num(u8* msg,u8* invalid_msg)
{
   s8*ptr=NULL;
   u32 i;
   ptr=(s8*)msg;
   ptr=strstr((s8*)msg,"UNREAD");
   ptr=ptr+11;

   for(i=0;i<10;i++)
   {
     invalid_msg[i]=ptr[i];
   }
        invalid_msg[i]='\0';
}

