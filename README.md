# Trabajo Practico 3 - Circuitos Digitales y Microcontroladores

Breve descripción del proyecto, su propósito y los problemas que resuelve.

Implementacion de un registrador de temperatura y humedad relativa ambiente utilizando el sensor DHT11, el
módulo RTC DS3231 y el kit del MCU conectado a una PC por medio de la interfaz USB.
El sensor DHT11 estará conectado al terminal PORTC0 del MCU, mientras que el módulo RTC se conectará
mediante la interfaz I2C del mismo. Para resolver el problema se implementan los drivers para el control del
sensor, para el control del módulo RTC y para la comunicación serie asincrónica por UART.

## Requerimientos que el sistema debe cumplir: 
a) El MCU deberá encuestar al sensor para obtener una medida de la temperatura y la humedad relativa cada
2seg.
b) Utilizando el módulo RTC el MCU completará el registro agregando la fecha y hora actual a cada una de las
medidas obtenidas con el sensor.
c) Por último realizará un formateo de los datos para transmitir el mensaje a una terminal serie en PC. Por
ejemplo, el formato puede ser “TEMP: 20 °C HUM: 40% FECHA: 10/06/24 HORA:15:30:56\r\n”
d) El envío de datos se podrá detener o reanudar desde la PC presionando la tecla ‘s’ o ‘S’ sucesivamente.
e) La comunicación serie asincrónica deberá implementarse utilizando interrupciones de recepción y
transmisión del periférico UART0.

## Tabla de Contenidos
- [Instalación](#instalación)
- [Uso](#uso)
- [Contribución](#contribución)
- [Licencia](#licencia)
- [Autores y Reconocimientos](#autores-y-reconocimientos)

## Instalación

### Prerrequisitos
- [Software o dependencias necesarias]

### Pasos
