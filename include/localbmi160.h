#ifndef __BMI160_H__
#define __BMI160_H__

#include <stdint.h> // Define los uint8_t ...
#include <freertos/FreeRTOS.h> // Para las funciones de FreeRTOS
#include "bmi160.h" // Libreria del BMI160 de Bosch

/* Funcion para inicializar el BMI160 */
bmi160_dev bmi_init_Spi();

//Lectura de un registro del BMI160, cogida de bmi160_defs.h
int8_t bmi_read_spi(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

//Escritura de un registro del BMI160, cogida de bmi160_defs.h
int8_t bmi_write_spi(uint8_t dev_addr, uint8_t reg_addr, uint8_t *read_data, uint16_t len);

//Delay para el BMI160, cogida de bmi160_defs.h
void bmi_delay(uint32_t period);


#endif //__BMI160_H__