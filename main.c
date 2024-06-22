#define F_CPU 16000000UL  // Define la frecuencia de la CPU como 16 MHz
#include <avr/io.h>       
#include <util/delay.h>   
#include <stdio.h>        
#include <avr/interrupt.h>
#include "dht11.h"        
#include "timer.h"        
#include "serialPort.h"   
#include "i2c.h"          
#include "ds3231.h"       

#define BR9600 (0x67)  // 0x67=103 configura BAUDRATE=9600@16MHz

volatile uint8_t send_data = 1;  // Variable global para controlar el envío de datos
volatile char RX_Buffer = 0;     // Buffer para recepción de datos
extern volatile flagLibre;       // Variable externa para indicar el estado del buffer de transmisión
volatile uint8_t dht11_ready = 0;

int main(void) {
	// Inicialización del temporizador
	timer1_init();  // Llama a la función de inicialización del temporizador

	// Habilita interrupciones globales
	sei();  // Habilita todas las interrupciones globales

	// Inicialización de I2C y UART
	I2C_Init();  // Inicializa la comunicación I2C
	DS3231_Time currentTime;  // Crea una estructura para almacenar el tiempo del RTC
	currentTime.seconds = 0;  // Establece los segundos iniciales a 0
	currentTime.minutes = 0;  // Establece los minutos iniciales a 0
	currentTime.hours = 18;   // Establece las horas iniciales a 18 (6 PM)
	currentTime.day = 3;      // Establece el día de la semana inicial a 3
	currentTime.date = 9;     // Establece la fecha inicial al 9
	currentTime.month = 6;    // Establece el mes inicial a junio
	currentTime.year = 24;    // Establece el año inicial a 2024
	DS3231_SetTime(&currentTime);  // Establece el tiempo del RTC con la estructura inicializada

	SerialPort_Init(BR9600); // Inicializa el puerto serie con el baudrate 9600
	SerialPort_TX_Enable();  // Habilita la transmisión por el puerto serie
	SerialPort_RX_Enable();  // Habilita la recepción por el puerto serie
	SerialPort_TX_Interrupt_Enable();  // Habilita la interrupción de transmisión del puerto serie
	char msg[] = "Sistema Iniciado\n\r";  // Define un mensaje de inicio
	SerialPort_Send_String(msg);  // Envía el mensaje de inicio por el puerto serie
	SerialPort_RX_Interrupt_Enable();  // Habilita la interrupción de recepción del puerto serie

	uint8_t hour, minute, second;  // Variables para almacenar la hora, los minutos y los segundos
	uint8_t day, month;  // Variables para almacenar el día y el mes
	uint8_t year;  // Variable para almacenar el año

	while (1) {  // Bucle infinito
		 if (flag) {  // Comprueba si el flag para DHT11 está activado
			 flag = 0;  // Resetea el flag
			 dht11_ready = 1;
			 read_dht11();  // Lee los datos del sensor DHT11
		 }
		 
		 // Enviar datos si está habilitado y ambos sensores han proporcionado datos
		 if (send_data && dht11_ready) {
			 dht11_ready = 0;  // Resetea el flag de lectura del DHT11
			 
			 reiniciar_timer();
			 
			 DS3231_GetTime(&currentTime); 
			 
			 char buffer[128];  // Define un buffer para el mensaje
			 if (!fallo)
				 snprintf(buffer, sizeof(buffer), "TEMP: %02d°C HUM: %02d%% FECHA: %02d/%02d/%02d HORA: %02d:%02d:%02d\n\r", temperature, humidity, currentTime.date, currentTime.month, currentTime.year, currentTime.hours, currentTime.minutes, currentTime.seconds);
			 else
				 snprintf(buffer, sizeof(buffer), "Fallo del sensor DHT11!!\n\r");
			 SerialPort_Send_String(buffer);  // Envía el mensaje formateado por el puerto serie
		 }
	}

	return 1;  // Retorna 1 (aunque nunca llegará a este punto debido al bucle infinito)
}



// Interrupción de recepción UART
ISR(USART_RX_vect) {
	RX_Buffer = UDR0; // Lee el dato recibido y lo almacena en RX_Buffer (la lectura borra el flag RXC)
	if (RX_Buffer == 's' || RX_Buffer == 'S') {
		send_data = !send_data;  // Cambia el estado de la variable send_data
	}
}

// Interrupción de transmisión UART
ISR(USART_UDRE_vect) {
	flagLibre = 1;  // Indica que el buffer de transmisión está libre
	SerialPort_TX_Interrupt_Disable();  // Deshabilita la interrupción de transmisión UART
}
