#include "rtc.h"
#include <util/twi.h>

#define DS3231_ADDRESS 0x68

void I2C_init(void) {
	TWSR = 0x00; // Configurar prescaler a 1
	TWBR = ((16000000UL / 100000UL) - 16) / 2; // Configurar SCL a 100kHz
}

uint8_t BCD_to_decimal(uint8_t bcd) {
	return ((bcd / 16) * 10) + (bcd % 16);
}

void RTC_get_time(uint8_t* hour, uint8_t* minute, uint8_t* second) {
	// Iniciar comunicaci�n I2C
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	// Enviar direcci�n del DS3231 con bit de escritura
	TWDR = DS3231_ADDRESS << 1;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Enviar direcci�n del registro de segundos (0x00)
	TWDR = 0x00;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Repetir inicio
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	// Enviar direcci�n del DS3231 con bit de lectura
	TWDR = (DS3231_ADDRESS << 1) | 0x01;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Leer segundos
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*second = BCD_to_decimal(TWDR & 0x7F); // Solo necesitamos los �ltimos 7 bits

	// Leer minutos
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*minute = BCD_to_decimal(TWDR & 0x7F); // Solo necesitamos los �ltimos 7 bits

	// Leer horas
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*hour = BCD_to_decimal(TWDR & 0x3F); // Solo necesitamos los �ltimos 6 bits

	// Enviar se�al de parada
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}

void RTC_get_date(uint8_t* day, uint8_t* month, uint16_t* year) {
	// Iniciar comunicaci�n I2C
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	// Enviar direcci�n del DS3231 con bit de escritura
	TWDR = DS3231_ADDRESS << 1;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Enviar direcci�n del registro del d�a del mes (0x04)
	TWDR = 0x04;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Repetir inicio
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	// Enviar direcci�n del DS3231 con bit de lectura
	TWDR = (DS3231_ADDRESS << 1) | 0x01;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Leer d�a del mes
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*day = BCD_to_decimal(TWDR & 0x3F); // Solo necesitamos los �ltimos 6 bits

	// Leer mes
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*month = BCD_to_decimal(TWDR & 0x1F); // Solo necesitamos los �ltimos 5 bits

	// Leer a�o
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*year = BCD_to_decimal(TWDR);

	// Enviar se�al de parada
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}
