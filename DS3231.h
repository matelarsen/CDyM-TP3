// ds3231.h
#ifndef DS3231_H_
#define DS3231_H_

struct rtc_time {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
};

void ds3231_init(void);
void ds3231_get_time(struct rtc_time *t);

#endif /* DS3231_H_ */

// ds3231.c
#include "ds3231.h"
#include <util/twi.h>

void ds3231_init(void) {
	// Inicialización del módulo I2C
}

void ds3231_get_time(struct rtc_time *t) {
	// Lectura de tiempo desde DS3231
}
