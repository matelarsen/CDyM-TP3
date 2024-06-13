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

volatile uint8_t send_data = 1;  // Variable global para controlar el env�o de datos
volatile char RX_Buffer = 0;     // Buffer para recepci�n de datos
extern volatile flagLibre;       // Variable externa para indicar el estado del buffer de transmisi�n

int main(void) {
	// Inicializaci�n del temporizador
	timer1_init();  // Llama a la funci�n de inicializaci�n del temporizador

	// Habilita interrupciones globales
	sei();  // Habilita todas las interrupciones globales

	// Inicializaci�n de I2C y UART
	I2C_Init();  // Inicializa la comunicaci�n I2C
	DS3231_Time currentTime;  // Crea una estructura para almacenar el tiempo del RTC
	currentTime.seconds = 0;  // Establece los segundos iniciales a 0
	currentTime.minutes = 0;  // Establece los minutos iniciales a 0
	currentTime.hours = 18;   // Establece las horas iniciales a 18 (6 PM)
	currentTime.day = 3;      // Establece el d�a de la semana inicial a 3
	currentTime.date = 9;     // Establece la fecha inicial al 9
	currentTime.month = 6;    // Establece el mes inicial a junio
	currentTime.year = 24;    // Establece el a�o inicial a 2024
	DS3231_SetTime(&currentTime);  // Establece el tiempo del RTC con la estructura inicializada

	SerialPort_Init(BR9600); // Inicializa el puerto serie con el baudrate 9600
	SerialPort_TX_Enable();  // Habilita la transmisi�n por el puerto serie
	SerialPort_RX_Enable();  // Habilita la recepci�n por el puerto serie
	SerialPort_TX_Interrupt_Enable();  // Habilita la interrupci�n de transmisi�n del puerto serie
	char msg[] = "Sistema Iniciado\n\r";  // Define un mensaje de inicio
	SerialPort_Send_String(msg);  // Env�a el mensaje de inicio por el puerto serie
	SerialPort_RX_Interrupt_Enable();  // Habilita la interrupci�n de recepci�n del puerto serie

	uint8_t hour, minute, second;  // Variables para almacenar la hora, los minutos y los segundos
	uint8_t day, month;  // Variables para almacenar el d�a y el mes
	uint8_t year;  // Variable para almacenar el a�o

	while (1) {  // Bucle infinito
		if (flag) {  // Comprueba si el flag est� activado
			flag = 0;  // Resetea el flag

			if (send_data) {  // Si el env�o de datos est� habilitado
				DS3231_GetTime(&currentTime);  // Obtiene la hora actual del RTC
				read_dht11();  // Lee los datos del sensor DHT11
				// Formatea y env�a los datos
				char buffer[128];  // Define un buffer para el mensaje
				if (!fallo)
				  snprintf(buffer, sizeof(buffer), "TEMP: %02d�C HUM: %02d%% FECHA: %02d/%02d/%02d HORA: %02d:%02d:%02d\n\r", temperature, humidity, currentTime.date, currentTime.month, currentTime.year, currentTime.hours, currentTime.minutes, currentTime.seconds);
				else
				  snprintf(buffer, sizeof(buffer), "Fallo del sensor DHT11!!\n\r");
				SerialPort_Send_String(buffer);  // Env�a el mensaje formateado por el puerto serie
			}
		}

		// Cambia el estado de env�o de datos al recibir 's' o 'S'
		if (RX_Buffer) {  // Comprueba si hay un dato recibido en el buffer
			if (RX_Buffer == 's' || RX_Buffer == 'S') {  // Si el dato recibido es 's' o 'S'
				send_data = !send_data;  // Cambia el estado de la variable send_data
			}
			RX_Buffer = 0;  // Resetea el buffer de recepci�n
		}
	}

	return 1;  // Retorna 1 (aunque nunca llegar� a este punto debido al bucle infinito)
}

// Interrupci�n de recepci�n UART
ISR(USART_RX_vect) {
	RX_Buffer = UDR0; // Lee el dato recibido y lo almacena en RX_Buffer (la lectura borra el flag RXC)
}

// Interrupci�n de transmisi�n UART
ISR(USART_UDRE_vect) {
	flagLibre = 1;  // Indica que el buffer de transmisi�n est� libre
	SerialPort_TX_Interrupt_Disable();  // Deshabilita la interrupci�n de transmisi�n UART
}
