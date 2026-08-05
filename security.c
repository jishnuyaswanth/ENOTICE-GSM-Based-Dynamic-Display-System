#include <string.h>
#include "func.h"
#include "gsm.h"

extern u8 f;                         // Flag to indicate invalid mobile number format
extern u8 auth_number[];             // Stores the currently authorized mobile number

// Verify whether the received SMS has a valid sender and security command
int verify_format(u8 *msg, u8 *Author_num)
{
    if(strstr((s8*)msg, (s8*)Author_num))      // Check whether SMS is from the authorized mobile number
    {
        if((strstr((s8*)msg,"5665D")) && (strstr((s8*)msg,"@"))) // Check for Display Update command
        {
            return 1;                          // Valid display message command
        }
        else if((strstr((s8*)msg,"5665M")) && (strstr((s8*)msg,"@"))) // Check for Mobile Number Update command
        {
            return 2;                          // Valid mobile number update command
        }
        else if(strstr((s8*)msg,"5665I@"))     // Check for Display Message Read command
        {
            return 3;                          // Valid information request command
        }
        else
        {
            return 4;                          // Authorized sender but invalid security code
        }
    }
    else
    {
        return 5;                              // Unauthorized sender
    }
}


// Extract the actual display message by removing the security code
void extract_content(u8 *msg1, u8 *output)
{
    u32 i, j=0;                                // i = source index, j = destination index

    s8 *msg = NULL;                            // Pointer to beginning of security code

    memset(output, '\0', 200);                 // Clear output buffer

    msg = strstr((s8*)msg1, "5665D");          // Locate "5665D" security code

    for(i=2; msg[i]!='@'; i++)                 // Copy characters until '@' terminator
    {
        if(i<5)                                // Skip the remaining security characters
        {
            output[j++]=' ';                   // Replace them with spaces
        }
        else if(msg[i]=='\r' && msg[i+1]=='\n') // Check for CR+LF sequence
        {
            output[j++]=' ';                   // Replace new line with a space
            i++;                               // Skip the '\n' character
        }
        else if(msg[i]=='\r' || msg[i]=='\n')  // Check for individual CR or LF
        {
            output[j++]=' ';                   // Replace with space
        }
        else
        {
            output[j++] = msg[i];              // Copy actual display character
        }
    }

    output[j++]=' ';                           // Add trailing space for smooth scrolling
    output[j++]=' ';                           // Add trailing space
    output[j++]=' ';                           // Add trailing space

    output[j]='\0';                            // Append NULL terminator
}


// Extract the new authorized mobile number
void extract_num(u8 *msg1, u8 *onum)
{
    u32 i, j=0;                                // Loop variables

    u8 num[11];                                // Temporary buffer for 10-digit mobile number

    s8 *msg = (s8*)msg1;                       // Point to received SMS

    msg = strstr((s8*)msg1,"5665M");           // Locate mobile number command

    msg = msg + 5;                             // Skip "5665M" security code

    for(; *msg!='@'; msg++)                    // Read digits until '@'
    {
        if((*msg>='0') && (*msg<='9'))         // Verify each character is a digit
        {
            num[j++] = *msg;                   // Store digit
        }
        else
        {
            f = 1;                             // Invalid character found

            break;                             // Stop processing
        }
    }

    num[j]='\0';                               // Append NULL terminator

    if(f==0)                                   // Check whether number format is valid
    {
        send_sms(auth_number,                  // Notify current authorized user
                 "Authorization of Number\n",
                 "SUCCESS");

        strcpy(onum, num);                     // Copy new number into authorized number buffer
    }
}


// Extract message received from an unauthorized sender
void Invalid_msg(u8 *msg1, u8 *invalid_msg)
{
    u32 newline=0, i=0, j=10;                  // j starts after the mobile number

    s8 *msg = NULL;                            // Pointer to SMS buffer

    msg = (s8*)msg1;                           // Point to received SMS

    msg = strstr(msg,"UNREAD");                // Locate SMS header

    invalid_msg[j++] = '\n';                   // Insert newline

    invalid_msg[j++] = 'M';                    // Append 'M'

    invalid_msg[j++] = 's';                    // Append 's'

    invalid_msg[j++] = 'g';                    // Append 'g'

    invalid_msg[j++] = ':';                    // Append ':'

    msg = msg + 52;                            // Skip GSM header and point to SMS body

    while(*msg != 0x0D)                        // Read until carriage return
    {
        invalid_msg[j++] = *msg;               // Copy SMS text

        msg++;                                 // Move to next character
    }

    invalid_msg[j]='\0';                       // Append NULL terminator
}


// Extract unauthorized sender's mobile number
void Invalid_num(u8 *msg, u8 *invalid_msg)
{
    s8 *ptr = NULL;                            // Pointer to SMS header

    u32 i;                                     // Loop variable

    ptr = (s8*)msg;                            // Point to received SMS

    ptr = strstr((s8*)msg,"UNREAD");           // Locate "UNREAD" header

    ptr = ptr + 11;                            // Move pointer to sender's mobile number

    for(i=0; i<10; i++)                        // Copy 10-digit mobile number
    {
        invalid_msg[i] = ptr[i];               // Store one digit
    }

    invalid_msg[i] = '\0';                     // Append NULL terminator
}