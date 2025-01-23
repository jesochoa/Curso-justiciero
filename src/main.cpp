
#include "bmi160.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

extern "C" void app_main();



void app_main() {
    vTaskDelay(pdMS_TO_TICKS(1000));

    bmi_initSpi();
   
   
    // Leer el ID del dispositivo BMI160

    uint8_t data; // Utilizamos una variable para los datos transmitir y recibir


    if (bmi160_read_register( 0x00, &data) == 0)
    {
        ESP_LOGI("MAIN", " BMI160 devuelve en el registro 0x00 : %x", data);
    }
    else
    {
        ESP_LOGE("MAIN", "Error al leer el ID del dispositivo BMI160");
    }

    bmi160_read_register( 0x00, &data);

    for(;;) // es como while (1) pero mas rapido
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    

}