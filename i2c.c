#include <avr/io.h>
#include "i2c.h"

void I2C_Init(void) {
	// Set SCL frequency to 400kHz with 16MHz clock
	TWSR = 0x00; // Configura el prescaler a 1
	TWBR = 152; // Configura el Bit Rate para 400kHz
	TWCR = (1<<TWEN); // Habilita el m�dulo TWI
}

void I2C_Start(void) {
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // Inicia la condici�n de START
	while (!(TWCR & (1<<TWINT))); // Espera a que se complete la condici�n de START
}

void I2C_Stop(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); // Genera una condici�n de STOP
	while(TWCR & (1<<TWSTO)); // Espera a que se complete la condici�n de STOP
}

void I2C_Write(uint8_t data) {
	TWDR = data; // Carga el dato en el registro de datos del TWI
	TWCR = (1<<TWINT) | (1<<TWEN); // Inicia la transmisi�n
	while (!(TWCR & (1<<TWINT))); // Espera a que se complete la transmisi�n
}

uint8_t I2C_Read_ACK(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // Inicia la recepci�n y env�a un ACK despu�s de recibir un byte
	while (!(TWCR & (1<<TWINT))); // Espera a que se complete la recepci�n
	return TWDR; // Devuelve el dato recibido
}

uint8_t I2C_Read_NACK(void) {
	TWCR = (1<<TWINT) | (1<<TWEN); // Inicia la recepci�n y no env�a un ACK despu�s de recibir un byte
	while (!(TWCR & (1<<TWINT))); // Espera a que se complete la recepci�n
	return TWDR; // Devuelve el dato recibido
}