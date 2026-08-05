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

// Buffer to store unauthorized sender information
u8 Invalid[200]="";

// Default message displayed after power-up
u8 stored_msg[200]="   WAITING FOR INPUT   ";

// Buffer to store received SMS
u8 new_msg[300];

// Default authorized mobile number
u8 auth_number[] ="7032613467";

extern u8 sms_flag;
extern u32 index;
extern u8 f=0;

//---------------------------------------------------------
// Initialize all peripherals used in the project
//---------------------------------------------------------
void init_sym()
{
    UART0_Init();      // Initialize UART for GSM communication
    init_i2c();        // Initialize I2C for EEPROM
    Init_dm_rows();    // Initialize Dot Matrix Display
    gsm_init();        // Initialize GSM Module
}

int main(void)
{
    s32 N_Bytes=100;

    // Initialize all hardware modules
    init_sym();

    //-----------------------------------------------------
    // Restore previously stored message from EEPROM
    //-----------------------------------------------------
    if((i2c_eeprom_randomread(I2C_EEPROM_SA1,EE_MSGI))=='1')
    {
        memset(stored_msg,'\0',200);
        EEPROM_Read(I2C_EEPROM_SA1,EE_MSG,stored_msg,N_Bytes);
    }

    //-----------------------------------------------------
    // Restore authorized mobile number from EEPROM
    //-----------------------------------------------------
    if((i2c_eeprom_randomread(I2C_EEPROM_SA1,EE_MOBI))=='2')
        EEPROM_Read(I2C_EEPROM_SA1,EE_MOB,auth_number,11);

    while(1)
    {
        // Clear runtime variables before waiting for next SMS
        index=0;
        memset(rx_buf,'\0',300);
        f=0;

        //-------------------------------------------------
        // Continuously display stored message and monitor GSM
        //-------------------------------------------------
        do
        {
            Display(stored_msg);
            Check_gsm();

        }while(sms_flag==0);

        // New SMS received
        sms_flag=0;

        // Read SMS from GSM memory
        Read_sms(new_msg);

        //-------------------------------------------------
        // Verify sender and security command
        //-------------------------------------------------
        switch(verify_format(new_msg,auth_number))
        {

            //-------------------------------------------------
            // Case 1 : Authorized user updating display message
            //-------------------------------------------------
            case 1:

                // Extract actual message by removing security code
                extract_content(new_msg,stored_msg);

                // Calculate message length
                N_Bytes=my_strlen(stored_msg);

                // Set message valid flag in EEPROM
                i2c_eeprom_bytewrite(I2C_EEPROM_SA1, EE_MSGI, '1');

                // Store updated message permanently
                EEPROM_write(I2C_EEPROM_SA1,EE_MSG,stored_msg,N_Bytes);

                // Delete processed SMS
                delete_sms();
                break;

            //-------------------------------------------------
            // Case 2 : Authorized user updating mobile number
            //-------------------------------------------------
            case 2:

                // Extract new mobile number
                extract_num(new_msg,auth_number);

                if(f==1)
                {
                    // Invalid mobile number format
                    send_sms(auth_number,
                             "Invalid format\n",
                             "Only num 5665MNUM@");
                }
                else
                {
                    // Store authorization flag
                    i2c_eeprom_bytewrite(I2C_EEPROM_SA1,
                                         EE_MOBI,
                                         '2');

                    // Save new mobile number in EEPROM
                    EEPROM_write(I2C_EEPROM_SA1,
                                 EE_MOB,
                                 auth_number,
                                 11);

                    // Send confirmation SMS
                    send_sms(auth_number,
                             "You are ",
                             "Authorized");
                }

                delete_sms();
                break;

            //-------------------------------------------------
            // Case 3 : Authorized user requesting current message
            //-------------------------------------------------
            case 3:

                send_sms(auth_number,
                         "The displaying content is:",
                         stored_msg);

                delete_sms();
                break;

            //-------------------------------------------------
            // Case 4 : Authorized user entered wrong security code
            //-------------------------------------------------
            case 4:

                send_sms(auth_number,
                         "Invalid security code\n",
                         "Correct security code\nFor updating message is"
                         " 5665Ddata@\nFor Updating Mobile Number is 5665Mnum@\nFor Retrieving Displaying content is 5665I@ ");

                delete_sms();
                break;

            //-------------------------------------------------
            // Case 5 : SMS received from unauthorized sender
            //-------------------------------------------------
            case 5:

                // Extract sender number and received message
                Invalid_num(new_msg,Invalid);
                Invalid_msg(new_msg,Invalid);

                // Notify authorized user
                send_sms(auth_number,
                         "Invalid number\n",
                         Invalid);

                delete_sms();

                // Clear temporary buffer
                memset(Invalid,'\0',300);

                break;
        }
    }
}
