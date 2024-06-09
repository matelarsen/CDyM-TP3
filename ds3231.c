#include "i2c.h"
#include "ds3231.h"

// Direcci�n del DS3231 en el bus I2C
#define DS3231_ADDRESS 0x68

// Funci�n para convertir de BCD a decimal
uint8_t BCD_to_DEC(uint8_t val) {
	return ((val / 16 * 10) + (val % 16));
}

// Funci�n para convertir de decimal a BCD
uint8_t DEC_to_BCD(uint8_t val) {
	return ((val / 10 * 16) + (val % 10));
}

// Funci�n para establecer la hora en el DS3231
void DS3231_SetTime(DS3231_Time *time) {
	I2C_Start(); // Inicia la comunicaci�n I2C
	I2C_Write((DS3231_ADDRESS << 1) | 0x00); // Direcci�n del DS3231 en modo escritura
	I2C_Write(0x00); // Empieza en el registro 0x00 (segundos)
	I2C_Write(DEC_to_BCD(time->seconds)); // Escribe los segundos en BCD
	I2C_Write(DEC_to_BCD(time->minutes)); // Escribe los minutos en BCD
	I2C_Write(DEC_to_BCD(time->hours));   // Escribe las horas en BCD
	I2C_Write(DEC_to_BCD(time->day));     // Escribe el d�a de la semana en BCD
	I2C_Write(DEC_to_BCD(time->date));    // Escribe el d�a del mes en BCD
	I2C_Write(DEC_to_BCD(time->month));   // Escribe el mes en BCD
	I2C_Write(DEC_to_BCD(time->year));    // Escribe el a�o en BCD
	I2C_Stop(); // Termina la comunicaci�n I2C
}

// Funci�n para obtener la hora del DS3231
void DS3231_GetTime(DS3231_Time *time) {
	I2C_Start(); // Inicia la comunicaci�n I2C
	I2C_Write((DS3231_ADDRESS << 1) | 0x00); // Direcci�n del DS3231 en modo escritura
	I2C_Write(0x00); // Empieza en el registro 0x00 (segundos)
	I2C_Stop(); // Termina la comunicaci�n I2C
	I2C_Start(); // Repite la condici�n de START para cambiar a modo lectura
	I2C_Write((DS3231_ADDRESS << 1) | 0x01); // Direcci�n del DS3231 en modo lectura
	
	// Lee los registros y convierte de BCD a decimal
	time->seconds = BCD_to_DEC(I2C_Read_ACK());
	time->minutes = BCD_to_DEC(I2C_Read_ACK());
	time->hours = BCD_to_DEC(I2C_Read_ACK());
	time->day = BCD_to_DEC(I2C_Read_ACK());
	time->date = BCD_to_DEC(I2C_Read_ACK());
	time->month = BCD_to_DEC(I2C_Read_ACK());
	time->year = BCD_to_DEC(I2C_Read_NACK()); // NACK en el �ltimo byte
	
	I2C_Stop(); // Termina la comunicaci�n I2C
}
