#include "dht11.h"
#define DHT11_PIN 0 // PORTC0
#define DHT11_DDR DDRC
#define DHT11_PORT PORTC
#define DHT11_PIN_INPUT PINC

volatile uint8_t temperature = 0;
volatile uint8_t humidity = 0;
volatile uint8_t check_sum = 0;

void read_dht11(void) {
	uint8_t i;
	uint8_t data[5] = {0};

	// Inicia la comunicación con el sensor
	DHT11_DDR |= (1 << DHT11_PIN);  // Configura el pin del DHT11 como salida
	DHT11_PORT &= ~(1 << DHT11_PIN); // Lleva el pin a LOW
	_delay_ms(18); // Mantiene LOW por al menos 18 ms
	DHT11_PORT |= (1 << DHT11_PIN);  // Lleva el pin a HIGH
	_delay_us(20);  // Espera 20-40 us
	
	DHT11_DDR &= ~(1 << DHT11_PIN);  // Configura el pin como entrada

	// Espera la respuesta del sensor
	while(DHT11_PIN_INPUT & (1 << DHT11_PIN)); // Espera a que el pin se ponga en LOW
	while(!(DHT11_PIN_INPUT & (1 << DHT11_PIN))); // Espera a que el pin se ponga en HIGH
	while(DHT11_PIN_INPUT & (1 << DHT11_PIN)); // Espera a que el pin se ponga en LOW

	// Lee los 5 bytes de datos
	for (i = 0; i < 5; i++) {
		data[i] = read_byte();
	}

	// Asignar valores leídos a variables
	humidity = data[0];
	temperature = data[2];
	check_sum = data[4];

}

uint8_t read_byte(void) {
	uint8_t result = 0;
	for (uint8_t i = 0; i < 8; i++) {
		while(!(DHT11_PIN_INPUT & (1 << DHT11_PIN))); // Espera a que el pin se ponga en HIGH
		_delay_us(30);
		if(DHT11_PIN_INPUT & (1 << DHT11_PIN)) {
			result |= (1 << (7 - i)); // Si el pin sigue en HIGH, es un 1
		}
		while(DHT11_PIN_INPUT & (1 << DHT11_PIN)); // Espera a que el pin se ponga en LOW
	}
	return result;
}