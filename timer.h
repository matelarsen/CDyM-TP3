#ifndef TIMER_H_
#define TIMER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

extern volatile uint8_t flag_ds3231,flag_dht11 ;

void timer1_init(void);

#endif /* TIMER_H_ */