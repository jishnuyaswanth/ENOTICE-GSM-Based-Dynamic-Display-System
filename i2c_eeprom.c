#include "i2c.h"
#include "delay.h"

// Function prototype for writing a single byte into EEPROM
void i2c_eeprom_bytewrite(u8 slaveAddr, u16 wBufAddr, u8 dat);

// Write multiple bytes into EEPROM
void EEPROM_write(u8 slaveAddr, u16 wBufStartAddr, u8 *p, u8 nBytes)
{
    int i = 0;                                      // Loop variable

    i2c_start();                                    // Generate I2C START condition

    i2c_write(slaveAddr << 1);                      // Send EEPROM slave address with WRITE bit

    i2c_write(wBufStartAddr >> 8);                  // Send higher byte of EEPROM memory address

    i2c_write(wBufStartAddr);                       // Send lower byte of EEPROM memory address

    for(i = 0; p[i]; i++)                          // Write data bytes until NULL character
    {
        i2c_write(p[i]);                            // Write one byte into EEPROM
    }

    i2c_write('@');                                 // Store '@' as end-of-message marker

    i2c_stop();                                     // Generate I2C STOP condition

    delay_ms(10);                                   // Wait for EEPROM internal write cycle to complete
}


// Read multiple bytes from EEPROM
void EEPROM_Read(u8 slaveAddr, u16 rBufStartAddr, u8 *p, u8 nBytes)
{
    u8 i;                                           // Loop variable

    i2c_start();                                    // Generate START condition

    i2c_write(slaveAddr << 1);                      // Send slave address with WRITE bit

    i2c_write(rBufStartAddr >> 8);                  // Send higher byte of EEPROM memory address

    i2c_write(rBufStartAddr);                       // Send lower byte of EEPROM memory address

    i2c_restart();                                  // Generate REPEATED START condition

    i2c_write((slaveAddr << 1) | 1);                // Send slave address with READ bit

    for(i = 0; i < nBytes; i++)                     // Read specified number of bytes
    {
        p[i] = i2c_mack();                          // Read one byte and send ACK

        if(p[i] == '@')                             // Check for end-of-message marker
        {
            break;                                  // Stop reading when '@' is encountered
        }
    }

    p[i] = '\0';                                    // Append NULL terminator to make it a C string

    i2c_nack();                                     // Read last byte with NACK to end reception

    i2c_stop();                                     // Generate STOP condition
}


// Write a single byte into EEPROM
void i2c_eeprom_bytewrite(u8 slaveAddr, u16 wBufAddr, u8 dat)
{
    i2c_start();                                    // Generate START condition

    i2c_write(slaveAddr << 1);                      // Send slave address with WRITE bit

    i2c_write(wBufAddr >> 8);                       // Send higher byte of EEPROM address

    i2c_write(wBufAddr);                            // Send lower byte of EEPROM address

    i2c_write(dat);                                 // Write one byte into EEPROM

    i2c_stop();                                     // Generate STOP condition

    delay_ms(10);                                   // Wait until EEPROM completes write operation
}


// Read a single byte from EEPROM
u8 i2c_eeprom_randomread(u8 slaveAddr, u8 rBufAddr)
{
    u8 dat;                                         // Variable to store received data

    i2c_start();                                    // Generate START condition

    i2c_write(slaveAddr << 1);                      // Send slave address with WRITE bit

    i2c_write(rBufAddr >> 8);                       // Send higher byte of EEPROM address

    i2c_write(rBufAddr);                            // Send lower byte of EEPROM address

    i2c_restart();                                  // Generate REPEATED START condition

    i2c_write((slaveAddr << 1) | 1);                // Send slave address with READ bit

    dat = i2c_nack();                               // Read the final byte and send NACK

    i2c_stop();                                     // Generate STOP condition

    return dat;                                     // Return the received byte
}