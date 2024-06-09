#include "i2c.h"
#include "ds3231.h"

// Dirección del DS3231 en el bus I2C
#define DS3231_ADDRESS 0x68

// Función para convertir de BCD a decimal
uint8_t BCD_to_DEC(uint8_t val) {
	return ((val / 16 * 10) + (val % 16));
}

// Función para convertir de decimal a BCD
uint8_t DEC_to_BCD(uint8_t val) {
	return ((val / 10 * 16) + (val % 10));
}

// Función para establecer la hora en el DS3231
void DS3231_SetTime(DS3231_Time *time) {
	I2C_Start(); // Inicia la comunicación I2C
	I2C_Write((DS3231_ADDRESS << 1) | 0x00); // Dirección del DS3231 en modo escritura
	I2C_Write(0x00); // Empieza en el registro 0x00 (segundos)
	I2C_Write(DEC_to_BCD(time->seconds)); // Escribe los segundos en BCD
	I2C_Write(DEC_to_BCD(time->minutes)); // Escribe los minutos en BCD
	I2C_Write(DEC_to_BCD(time->hours));   // Escribe las horas en BCD
	I2C_Write(DEC_to_BCD(time->day));     // Escribe el día de la semana en BCD
	I2C_Write(DEC_to_BCD(time->date));    // Escribe el día del mes en BCD
	I2C_Write(DEC_to_BCD(time->month));   // Escribe el mes en BCD
	I2C_Write(DEC_to_BCD(time->year));    // Escribe el año en BCD
	I2C_Stop(); // Termina la comunicación I2C
}

// Función para obtener la hora del DS3231
void DS3231_GetTime(DS3231_Time *time) {
	I2C_Start(); // Inicia la comunicación I2C
	I2C_Write((DS3231_ADDRESS << 1) | 0x00); // Dirección del DS3231 en modo escritura
	I2C_Write(0x00); // Empieza en el registro 0x00 (segundos)
	I2C_Stop(); // Termina la comunicación I2C
	I2C_Start(); // Repite la condición de START para cambiar a modo lectura
	I2C_Write((DS3231_ADDRESS << 1) | 0x01); // Dirección del DS3231 en modo lectura
	
	// Lee los registros y convierte de BCD a decimal
	time->seconds = BCD_to_DEC(I2C_Read_ACK());
	time->minutes = BCD_to_DEC(I2C_Read_ACK());
	time->hours = BCD_to_DEC(I2C_Read_ACK());
	time->day = BCD_to_DEC(I2C_Read_ACK());
	time->date = BCD_to_DEC(I2C_Read_ACK());
	time->month = BCD_to_DEC(I2C_Read_ACK());
	time->year = BCD_to_DEC(I2C_Read_NACK()); // NACK en el último byte
	
	I2C_Stop(); // Termina la comunicación I2C
}
