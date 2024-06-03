#include "timer.h"

volatile uint8_t flag = 0;

void timer1_init(void) {
	// Configura el Timer1 para generar una interrupción cada 2 segundos
	TCCR1B |= (1 << WGM12);  // Modo CTC (Clear Timer on Compare Match)
	TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
	OCR1A = 31249;  // Valor de comparación para 2 segundos
	TIMSK1 |= (1 << OCIE1A);  // Habilita la interrupción por comparación
}

ISR(TIMER1_COMPA_vect) {
	flag = 1;
}
