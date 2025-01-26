
#include "localbmi160.h"

#include <freertos/FreeRTOS.h>

extern "C" void app_main();

bmi160_dev dev; // Estructura del BMI160

void app_main() {

    vTaskDelay(pdMS_TO_TICKS(1000));

    dev = bmi_init_Spi(); // Inicializar el bus SPI y el dispositivo BMI160

    int8_t rslt = bmi160_init(&dev); // Inicializar el BMI160

    if (rslt != BMI160_OK)
    {
        printf("Error al inicializar el BMI160!\n");
    }
    else
    {
        printf("BMI160 inicializado correctamente\n");
        printf("Chip ID 0x%X\n", dev.chip_id);
    }



    /* Select the Output data rate, range of accelerometer sensor */
    dev.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
    dev.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
    dev.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

    /* Select the power mode of accelerometer sensor */
    dev.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    /* Select the Output data rate, range of Gyroscope sensor */
    dev.gyro_cfg.odr = BMI160_GYRO_ODR_3200HZ;
    dev.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    dev.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    /* Select the power mode of Gyroscope sensor */
    dev.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

    /* Set the sensor configuration */
    rslt = bmi160_set_sens_conf(&dev);

    for(;;) // es como while lo pongo porque es mas rapido
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    

}