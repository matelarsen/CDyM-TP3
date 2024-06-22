#ifndef TIMER_H_
#define TIMER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

extern volatile uint8_t flag ;

void timer1_init(void);
void reiniciar_timer();

#endif /* TIMER_H_ */