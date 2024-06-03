#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "dht11.h"
#include "timer.h"
#include "rtc.h"
#include "serialPort.h"
#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz


volatile uint8_t send_data = 1;
volatile char RX_Buffer = 0; // Buffer para recepción de datos
extern volatile uint8_t flagLibre;

int main(void) {
	// Inicialización del temporizador
	timer1_init();

	// Habilita interrupciones globales
	sei();
	
	// Inicialización de I2C y UART
	I2C_init();	
	SerialPort_Init(BR9600); // Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_TX_Enable();
	SerialPort_RX_Enable();
	_delay_ms(100);
	SerialPort_TX_Interrupt_Enable();

	char msg[] = "Sistema Iniciado\n\r";
	SerialPort_Send_String(msg);
	SerialPort_RX_Interrupt_Enable();
	
	

	uint8_t hour, minute, second;
	uint8_t day, month;
	uint16_t year;

	while (1) {
		if (flag) {
			flag = 0;
			read_dht11();

			if (send_data) {
				//RTC_get_time(&hour, &minute, &second);
				//RTC_get_date(&day, &month, &year);

				// Formatear y enviar datos
				char buffer[128];
				//snprintf(buffer, sizeof(buffer), "TEMP: %d°C HUM: %d%% FECHA: %d/%d/%d HORA: %d:%d:%d\n\r", temperature, humidity, day, month, year, hour, minute, second);
				snprintf(buffer, sizeof(buffer), "TEMP: %d°C HUM: %d%% \n\r", temperature, humidity);
				//char msg2[] = "Sistema Iniciado\n\r";
				SerialPort_Send_String(buffer);
			}
		}

		// Cambiar el estado de envío de datos al recibir 's' o 'S'
		if (RX_Buffer) {
			if (RX_Buffer == 's' || RX_Buffer == 'S') {
				send_data = !send_data;
			}
			RX_Buffer = 0;
		}
	}

	return 1;
}


ISR(USART_RX_vect) {
	RX_Buffer = UDR0; // La lectura del UDR borra el flag RXC
}

ISR(USART_UDRE_vect){
	flagLibre = 1;
	SerialPort_TX_Interrupt_Disable();
}