//i2c.h

#ifndef I2C_H
#define I2C_H


#include "types.h"


void init_i2c(void);

void i2c_start(void) ;

void i2c_restart(void) ;

void i2c_stop(void); 

void i2c_write(u8 dat);

u8 i2c_nack(void); 

u8 i2c_mack(void);


#endif
