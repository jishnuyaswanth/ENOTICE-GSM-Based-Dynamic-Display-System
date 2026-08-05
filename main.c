#include <string.h>
#include "uart.h"
#include "types.h"
#include "delay.h"
#include "i2c_eeprom.h"
#include "gsm.h"
#include "func.h"
#include "i2c_eeprom_defines.h"
#include "dot_matrix.h"
#include "i2c.h"

u8 Invalid[200]="";     // Buffer to store unauthorized sender information


u8 stored_msg[200]="   WAITING FOR INPUT   ";    // Default message displayed after power-up


u8 new_msg[300];    // Buffer to store received SMS


u8 auth_number[] ="7032613467";   //Default authorized mobile number

extern u8 sms_flag;
extern u32 index;
extern u8 f=0;

//---------------------------------------------------------
// Initialize all peripherals used in the project
//---------------------------------------------------------
void init_sym()
{
    UART0_Init();      // Initialize UART0 for GSM communication
    init_i2c();        // Initialize I2C peripheral for EEPROM communication
    Init_dm_rows();    // Configure GPIOs and initialize 8x8 dot matrix display
    gsm_init();        // Initialize GSM module using AT commands
}
int main(void)
{
    s32 N_Bytes = 100;                         // Number of bytes to read/write from EEPROM

    init_sym();                               // Initialize all peripherals

    if(i2c_eeprom_randomread(I2C_EEPROM_SA1, EE_MSGI) == '1')   // Check whether a message is stored
    {
        memset(stored_msg,'\0',200);          // Clear message buffer
        EEPROM_Read(I2C_EEPROM_SA1, EE_MSG, stored_msg, N_Bytes); // Read stored display message
    }

    if(i2c_eeprom_randomread(I2C_EEPROM_SA1, EE_MOBI) == '2')   // Check whether an authorized number exists
        EEPROM_Read(I2C_EEPROM_SA1, EE_MOB, auth_number, 11);   // Read authorized mobile number

    while(1)                                 // Run forever
    {
        index = 0;                           // Reset UART receive index
        memset(rx_buf,'\0',300);             // Clear UART receive buffer
        f = 0;                               // Clear invalid number flag

        do
        {
            Display(stored_msg);             // Continuously scroll stored message
            Check_gsm();                     // Verify GSM module connectivity

        } while(sms_flag == 0);              // Wait until a new SMS arrives

        sms_flag = 0;                        // Clear SMS received flag
        Read_sms(new_msg);                   // Read received SMS from GSM memory

        switch(verify_format(new_msg, auth_number)) // Verify sender and command
        {

            case 1:

   	  extract_content(new_msg,stored_msg);                 // Extract the actual display message by removing the security code (5665D) and '@'

    N_Bytes = my_strlen(stored_msg);                    // Calculate the length of the extracted message	

    i2c_eeprom_bytewrite(I2C_EEPROM_SA1, EE_MSGI, '1');  // Store a flag in EEPROM indicating that a valid message exists

    EEPROM_write(I2C_EEPROM_SA1, EE_MSG, stored_msg, N_Bytes); // Save the new display message permanently in EEPROM

    delete_sms();                                        // Delete the processed SMS from GSM memory

    break;                                               // Exit the switch statement

               case 2:

    extract_num(new_msg, auth_number);                   // Extract the new authorized mobile number from the received SMS

    if(f == 1)                                           // Check whether the extracted number format is invalid
    {
        send_sms(auth_number,                            // Send an error SMS to the authorized user
                 "Invalid format\n",
                 "Only num 5665MNUM@");
    }
    else                                                 // Mobile number format is valid
    {
        i2c_eeprom_bytewrite(I2C_EEPROM_SA1, EE_MOBI, '2'); // Store a flag indicating that an authorized number is saved

        EEPROM_write(I2C_EEPROM_SA1, EE_MOB, auth_number, 11); // Save the new authorized mobile number into EEPROM

        send_sms(auth_number,                            // Send confirmation that authorization is successful
                 "You are ",
                 "Authorized");
    }

    delete_sms();                                        // Delete the processed SMS from GSM memory

    break;                                               // Exit the switch statement
                case 3:

    send_sms(auth_number,                                // Send the currently displayed message to the authorized user
             "The displaying content is:",
             stored_msg);

    delete_sms();                                        // Delete the processed SMS from GSM memory

    break;                                               // Exit the switch statement

           case 4:

    send_sms(auth_number,                                // Inform the user that an invalid security code was received
             "Invalid security code\n",
             "Correct security code\nFor updating message is"
             " 5665Ddata@\nFor Updating Mobile Number is 5665Mnum@\nFor Retrieving Displaying content is 5665I@ ");

    delete_sms();                                        // Delete the processed SMS from GSM memory

    break;                                               // Exit the switch statement

         case 5:

    Invalid_num(new_msg, Invalid);                       // Extract the sender's unauthorized mobile number

    Invalid_msg(new_msg, Invalid);                       // Append the unauthorized SMS content to the Invalid buffer

    send_sms(auth_number,                                // Notify the authorized user about the unauthorized SMS
             "Invalid number\n",
             Invalid);

    delete_sms();                                        // Delete the unauthorized SMS from GSM memory

    memset(Invalid, '\0', 300);                          // Clear the Invalid buffer for the next unauthorized message

    break;                                               // Exit the switch statement        }
    }
}