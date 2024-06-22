# Trabajo Practico 3 - Circuitos Digitales y Microcontroladores
# Registrador de Temperatura y Humedad con MCU

Este proyecto implementa un registrador de temperatura y humedad relativa utilizando el sensor DHT11, el módulo RTC DS3231 y un kit de MCU conectado a una PC a través de la interfaz USB. El proyecto ha sido desarrollado como parte del curso "Circuitos Digitales y Microcontroladores (E305)" en la Facultad de Ingeniería de la UNLP.

## Descripción

El sistema encuestará al sensor DHT11 cada 2 segundos para obtener medidas de temperatura y humedad relativa. Utilizando el módulo RTC DS3231, el MCU agregará la fecha y hora actual a cada registro y los enviará a una terminal serie en una PC mediante la interfaz UART.

## Requerimientos del Sistema

El sistema debe cumplir con los siguientes requerimientos:

1. **Encuesta del Sensor**: El MCU encuestará al sensor DHT11 cada 2 segundos.
2. **Registro de Fecha y Hora**: Utilizando el módulo RTC, el MCU añadirá la fecha y hora actual a cada registro.
3. **Formato de Datos**: Los datos se formatearán como "TEMP: 20 °C HUM: 40% FECHA: 10/06/24 HORA:15:30:56\r\n".
4. **Control de Envío de Datos**: El envío de datos puede detenerse o reanudarse desde la PC presionando la tecla ‘s’ o ‘S’.
5. **Comunicación Serie Asincrónica**: Implementada utilizando interrupciones de recepción y transmisión del periférico UART0.
6. **Comunicacion Serie Sincrónica**: Implementada utilizando la comunicacion I2C.

### Requisitos
- Kit de MCU compatible
- Sensor DHT11
- Módulo RTC DS3231
- Conexión a una PC vía USB
- Software para terminal serie

### Autores
- Melina Caciani Toniolo
- Franco Niderhaus
- Mateo Larsen
