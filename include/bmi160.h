#ifndef __BMI160_H__
#define __BMI160_H__

#include <stdint.h> // Define los uint8_t

/* Funcion para inicializar el BMI160 */
void bmi_initSpi();



// Lee un byte de un registro del BMI160
uint8_t  bmi160_read_register(uint8_t reg_addr, uint8_t *data);


#endif //__BMI160_H__