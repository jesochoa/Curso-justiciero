#ifndef __BMI160_H__
#define __BMI160_H__

#include <stdint.h> // Define los uint8_t

/* Funcion para inicializar el BMI160 */
void bmi_initSpi();

/* Se recibe un entero de 8 bits*/
uint8_t bmi_getId();


#endif //__BMI160_H__