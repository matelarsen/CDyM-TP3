#ifndef DS3231_H_
#define DS3231_H_

#include <avr/io.h>

// Estructura que define la hora y fecha para el DS3231
typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} DS3231_Time;

void DS3231_SetTime(DS3231_Time *time);
void DS3231_GetTime(DS3231_Time *time);

#endif /* DS3231_H_ */
