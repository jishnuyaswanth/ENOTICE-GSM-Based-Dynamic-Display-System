
void i2c_eeprom_bytewrite(u8 slaveAddr,u16 wBufAddr,u8 dat);
void EEPROM_write(u8 slaveAddr,u16 wBufStartAddr,u8 *p,u8 nBytes);
void EEPROM_Read(u8 slaveAddr,u16 rBufStartAddr,u8 *p,u8 nBytes);
u8 i2c_eeprom_randomread(u8 slaveAddr,u8 rBufAddr);
