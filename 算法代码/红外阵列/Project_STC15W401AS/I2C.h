#ifndef _I2C_H_
#define _I2C_H_

//extern void I2C_Read(unsigned char *buf, unsigned char addr, unsigned char len);
extern bit I2C_Write(unsigned char *buf, unsigned char addr, unsigned char len);

#endif