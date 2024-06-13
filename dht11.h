// dht11.h
#ifndef DHT11_H_
#define DHT11_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

extern volatile uint8_t temperature;
extern volatile uint8_t humidity ;
extern volatile uint8_t check_sum;
extern volatile uint8_t fallo;

void init_dht11(void);
void read_dht11(void);
uint8_t read_byte(void);

#endif /* DHT11_H_ */
