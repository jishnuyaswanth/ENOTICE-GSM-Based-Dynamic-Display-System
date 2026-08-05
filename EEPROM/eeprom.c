#include "i2c.h"
#include "delay.h"
void i2c_eeprom_bytewrite(u8 slaveAddr,u16 wBufAddr,u8 dat);

void EEPROM_write(u8 slaveAddr,u16 wBufStartAddr,u8 *p,u8 nBytes)
{
  int i=0;

        //start condition
        i2c_start();

        //writing Slave Address with Write Bit
        i2c_write(slaveAddr<<1);

        //Writing Memory Location MSB Byte
    i2c_write(wBufStartAddr>>8);

        //Writing Memory Location LSB Byte
    i2c_write(wBufStartAddr);

        //Writing N no of Bytes
         for(i=0;p[i];i++)
  {
                i2c_write(p[i]);
  }

        //Writing last Byte
        i2c_write('@');

        //stop condition
        i2c_stop();

        //delay for processing data
        delay_ms(10);
}

void EEPROM_Read(u8 slaveAddr,u16 rBufStartAddr,u8 *p,u8 nBytes)
{
   u8 i;

   //start condition
   i2c_start();

   //writing Slave Address with Write Bit
   i2c_write(slaveAddr<<1);

        //Writing Memory Location MSB Byte
   i2c_write(rBufStartAddr>>8);

        //Writing Memory Location LSB Byte
   i2c_write(rBufStartAddr);

        //Restart Condition
    i2c_restart();

        ////writing Slave Address with Read Bit
    i2c_write(slaveAddr<<1|1);

   //Reading N no of Data
   for(i=0;i<nBytes;i++)
   {

    p[i]=i2c_mack();

         if(p[i]=='@')
         {
               break;
     }

   }

   p[i]='\0';

     //Read one Byte of Data
        i2c_nack();

        //Stop Condition
    i2c_stop();

}

void i2c_eeprom_bytewrite(u8 slaveAddr,u16 wBufAddr,u8 dat)
{
        //start condition
    i2c_start();

        //writing Slave Address with Write Bit
    i2c_write(slaveAddr<<1);

        //Writing Memory Location MSB Byte
    i2c_write(wBufAddr>>8);

        //Writing Memory Location LSB Byte
        i2c_write(wBufAddr);

        //Writing one byte
    i2c_write(dat);

        //Stop Condition
    i2c_stop();

        //delay for processing data
    delay_ms(10);
}

u8 i2c_eeprom_randomread(u8 slaveAddr,u8 rBufAddr)
{
    u8 dat;

        //start condition
    i2c_start();

        //writing Slave Address with Write Bit
    i2c_write(slaveAddr<<1);

        //Writing Memory Location MSB Byte
    i2c_write(rBufAddr>>8);

        //Writing Memory Location LSB Byte
        i2c_write(rBufAddr);

        //Restart Condition
    i2c_restart();

        //writing Slave Address with Read Bit
    i2c_write(slaveAddr<<1|1);

        //Read one byte for data
    dat=i2c_nack();

        //Stop Condition
    i2c_stop();

    return dat;
}
          
