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
	// Iniciar comunicación I2C
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	// Enviar dirección del DS3231 con bit de escritura
	TWDR = DS3231_ADDRESS << 1;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Enviar dirección del registro de segundos (0x00)
	TWDR = 0x00;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Repetir inicio
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	// Enviar dirección del DS3231 con bit de lectura
	TWDR = (DS3231_ADDRESS << 1) | 0x01;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Leer segundos
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*second = BCD_to_decimal(TWDR & 0x7F); // Solo necesitamos los últimos 7 bits

	// Leer minutos
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*minute = BCD_to_decimal(TWDR & 0x7F); // Solo necesitamos los últimos 7 bits

	// Leer horas
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*hour = BCD_to_decimal(TWDR & 0x3F); // Solo necesitamos los últimos 6 bits

	// Enviar señal de parada
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}

void RTC_get_date(uint8_t* day, uint8_t* month, uint16_t* year) {
	// Iniciar comunicación I2C
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	// Enviar dirección del DS3231 con bit de escritura
	TWDR = DS3231_ADDRESS << 1;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Enviar dirección del registro del día del mes (0x04)
	TWDR = 0x04;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Repetir inicio
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	// Enviar dirección del DS3231 con bit de lectura
	TWDR = (DS3231_ADDRESS << 1) | 0x01;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	// Leer día del mes
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*day = BCD_to_decimal(TWDR & 0x3F); // Solo necesitamos los últimos 6 bits

	// Leer mes
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*month = BCD_to_decimal(TWDR & 0x1F); // Solo necesitamos los últimos 5 bits

	// Leer año
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*year = BCD_to_decimal(TWDR);

	// Enviar señal de parada
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}
