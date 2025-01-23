#include "bmi160.h"
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

const char TAG[] = "BMI_DRIVER";

// Registros del BMI160
#define BMI160_REG_CHIP_ID 0x00
#define BMI160_REG_CMD 0x7E
#define BMI160_REG_GYRO_X_LSB 0x0C
#define BMI160_REG_GYRO_X_MSB 0x0D
#define BMI160_REG_PWR_CONF 0x7C
#define BMI160_REG_PWR_CTRL 0x7D

spi_device_handle_t SpiHandle = 0; // Manejador del dispositivo SPI

/* Funcion para inicializar el BMI160 */
void bmi_initSpi()
{
    // Configuración del bus SPI
    spi_bus_config_t busSpi = {0};
    busSpi.mosi_io_num = GPIO_NUM_23;
    busSpi.miso_io_num = GPIO_NUM_19;
    busSpi.sclk_io_num = GPIO_NUM_18;
    busSpi.quadwp_io_num = -1;
    busSpi.quadhd_io_num = -1;
    busSpi.max_transfer_sz = 64; // Tamaño maximo de la transferencia

    // Inicializar el bus SPI
    if (spi_bus_initialize(SPI3_HOST, &busSpi, SPI_DMA_DISABLED) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al inicializar el bus SPI");
    }
    else
    {
        ESP_LOGW(TAG, "Bus SPI inicializado correctamente");
    }
        


    // Configuración del dispositivo SPI
    spi_device_interface_config_t SpicIf = {0};
    SpicIf.spics_io_num = GPIO_NUM_21;       // GPIO que utilicemos para CS
    SpicIf.clock_speed_hz = 1 * 1000 * 1000; // Velocidad comunicacion de 1Mhz
    SpicIf.mode = 0;                         // Modo SPI 0: CPOL=0, CPHA=0
    SpicIf.queue_size = 10;                   // Tamaño de la cola de transacciones
    SpicIf.address_bits = 8;                 // la dirección de 8 bits si no lo pongo no funciona

    

    // Añadir device al SPI
    if (spi_bus_add_device(SPI3_HOST, &SpicIf, &SpiHandle) != ESP_OK)
    {
        ESP_LOGE(TAG, "SPI Device cannot be added");
    }
    else
    {
        ESP_LOGW(TAG, "Dispositivo SPI añadido correctamente");

    }
}

/* Función para leer un registro del BMI160 */
uint8_t bmi160_read_register(uint8_t reg_addr, uint8_t *data)
{
    spi_transaction_t Spi_Trans = {0};
    Spi_Trans.flags = SPI_TRANS_USE_RXDATA ; // Seleccionamos la flag para usar el buffer de recepción
    Spi_Trans.length = 8; // Transmitimos en 8 bits
    Spi_Trans.rxlength = 8; // Recibimos un byte, 8 bits
    Spi_Trans.addr = (reg_addr | 0x80);// Direción a leer 0x00 y le añadimos el bit de lectura 0x80 para que funcione


    spi_device_acquire_bus(SpiHandle, portMAX_DELAY); // Adquirimos el bus SPI para poder transmitir (si no esta tambien funciona) 
    if (spi_device_polling_transmit(SpiHandle, &Spi_Trans) != ESP_OK) //Utilizo polling transmit porque no utilizo DMA
    {
        ESP_LOGE(TAG, "Error al transmitir");
        spi_device_release_bus(SpiHandle);
        return 1;
    }
    spi_device_release_bus(SpiHandle); // Liberamos el bus SPI

    // Copiamos el valor leído al puntero data
    *data = Spi_Trans.rx_data[0];

    ESP_LOGI(TAG, "Valor leído en el registro 0x%x: %x", reg_addr, *data);
    
    return 0;
}
