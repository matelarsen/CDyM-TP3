#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);
uint8_t I2C_Read_ACK(void);
uint8_t I2C_Read_NACK(void);

#endif /* I2C_H_ */
