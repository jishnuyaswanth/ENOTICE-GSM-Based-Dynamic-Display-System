/*******************************************************

 * File: i2c_peripheral.c

 * Description: Smicrocontrollers using on-chip I2C hardware.

 ********************************************************/


#include "types.h"          // Defines data types like u8

#include "i2c_defines.h"    // Defines I2C register bit positions & macros

#include <LPC21xx.h>        // LPC21xx register definitions


void init_i2c(void) 

{ 

   // Configure P0.2 and P0.3 as I2C pins (SCL & SDA)

   // This uses PINSEL0 register to select alternate functions

   // P0.2 -> SCL, P0.3 -> SDA

   PINSEL0 |= SCL_0_2 | SDA_0_3;


   // Set the HIGH time duration of the I2C clock

   // Determines SCL clock speed

   I2SCLH = I2C_DIVIDER;


   // Set the LOW time duration of the I2C clock

   // Together with I2SCLH, controls I2C bit rate
   
   I2SCLL = I2C_DIVIDER;


   // Enable I2C peripheral by setting I2EN bit

   // Without this, I2C hardware will not function

   I2CONSET = 1 << I2EN_BIT;

}



void i2c_start(void)

{ 
        u32 c=5000;
   // Set START bit to request start condition

   I2CONSET = 1 << STA_BIT; 


   // Wait until SI (Serial Interrupt) flag is set

   // SI = 1 means START condition transmitted

   while ((((I2CONSET >> SI_BIT) & 1) == 0) && c) 
                c--;
                
        if(c==0)
                Display("  EEPROM DISCONNECTED   ");



   // Clear START bit after start condition is sent

   // Prevents repeated start unintentionally

   I2CONCLR = 1 << STAC_BIT; 

}




void i2c_restart(void) 

{ 

   // Set START bit again for repeated start

   I2CONSET = 1 << STA_BIT;

   // Clear SI bit to allow hardware to continue

   // This initiates the repeated start transmission

   I2CONCLR = 1 << SIC_BIT;


   // Wait until repeated START is transmitted

   while (((I2CONSET >> SI_BIT) & 1) == 0); 


   // Clear START bit after repeated start

   I2CONCLR = 1 << STAC_BIT; 

}




void i2c_stop(void) 
{ 

   // Set STOP bit to release I2C bus

   I2CONSET = 1 << STO_BIT; 


   // Clear SI bit to allow STOP condition to complete

   I2CONCLR = 1 << SIC_BIT;

}




void i2c_write(u8 dat) 

{ 

  // Load data byte into I2C data register

  // Hardware automatically shifts it out on SDA

  I2DAT = dat;   


  // Clear SI flag to start data transmission

  I2CONCLR = 1 << SIC_BIT; 


  // Wait until transmission completes

  // SI flag is set after byte is sent & ACK received

  while (((I2CONSET >> SI_BIT) & 1) == 0); 

}


u8 i2c_nack(void) 

{ 

  // Clear SI flag to begin data reception

  I2CONCLR = 1 << SIC_BIT; 


  // Wait until byte is received

  // SI is set when data is available in I2DAT

  while (((I2CONSET >> SI_BIT) & 1) == 0); 


  // Return received data byte

  return I2DAT;

}


u8 i2c_mack(void) 

{ 

  // Set AA (Assert Acknowledge) bit

  // Tells I2C hardware to send ACK after reception

  I2CONSET = 1 << AA_BIT;

  // Clear SI flag to start receiving data

  I2CONCLR = 1 << SIC_BIT; 

  // Wait until byte reception completes

  while (((I2CONSET >> SI_BIT) & 1) == 0); 

  // Clear AA bit after ACK is sent

  // Prepares for next operation

  I2CONCLR = 1 << AA_BIT;

  // Return received data byte

  return I2DAT;

}

