#ifndef RTC_H
#define RTC_H

#include <avr/io.h>

void I2C_init(void);
void RTC_get_time(uint8_t* hour, uint8_t* minute, uint8_t* second);
void RTC_get_date(uint8_t* day, uint8_t* month, uint16_t* year);

#endif // RTC_H
