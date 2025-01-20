#include "bmi160.h"
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

const char TAG[] = "BMI_DRIVER";

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

    spi_device_interface_config_t SpicIf = {0};
    SpicIf.spics_io_num = GPIO_NUM_21;       // GPIO que utilicemos para CS
    SpicIf.clock_speed_hz = 1 * 1000 * 1000; // Velocidad comunicacion
    SpicIf.mode = 0;                         // Modo SPI 0: CPOL=0, CPHA=0
    SpicIf.queue_size = 1;                   // Tamaño de la cola de transacciones
    SpicIf.address_bits = 8;                 // la dirección de 8 bits si no lo pongo no funciona

    spi_device_handle_t SpiHandle = 0;

    // Inicializar el bus SPI
    if (spi_bus_initialize(SPI3_HOST, &busSpi, SPI_DMA_DISABLED) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al inicializar el bus SPI");
    }
    else
        ESP_LOGW(TAG, "Bus SPI inicializado correctamente");

    // Añadir el dispositivo SPI
    if (spi_bus_add_device(SPI3_HOST, &SpicIf, &SpiHandle) != ESP_OK)
    {
        ESP_LOGE(TAG, "SPI Device cannot be added");
    }
    else
        ESP_LOGW(TAG, "Dispositivo SPI añadido correctamente");


    /*Codigo para transmitir y recibir codigo*/
    uint8_t data; // Utilizamos una variable para los datos transmitir y recibir
    
    spi_transaction_t Spi_Trans = {0}; // Lo inicializo a cero por si hay algun valor en memoria
    Spi_Trans.length = 8; // Transmitimos en 8 Bits
    Spi_Trans.rxlength = 8; // Recibimos
    Spi_Trans.rx_buffer = &data; // Guardamos el valor recibido en la variable data
    Spi_Trans.flags = SPI_TRANS_USE_RXDATA ; // Lo selecionamos con las flags

    // Esta lectura en 0x7F es solo para que se ponga en modo SPI
    Spi_Trans.addr = (0x7F | 0X80); // Direción a leer 0x7F y le añadimos el bit de lectura 0x80

    spi_device_acquire_bus(SpiHandle, portMAX_DELAY); // Adquirimos el bus SPI para poder transmitir
    if (spi_device_polling_transmit(SpiHandle, &Spi_Trans) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al transmitir");
        return;
    }
    spi_device_release_bus(SpiHandle); // Liberamos el bus SPI

    ESP_LOGI(TAG, "Valor leido en el registro 0x7F: %x", Spi_Trans.rx_data[0]);
    //////////////////////////////////////////////////////////////////////////////////

    // Leemos el valor del registro 0x00 del BMI160 que nos tiene que devolver 0xD1
    Spi_Trans.addr = (0x00 | 0X80); // Direción a leer 0x00 y le añadimos el bit de lectura 0x80

    spi_device_acquire_bus(SpiHandle, portMAX_DELAY); // Adquirimos el bus SPI para poder transmitir
    if (spi_device_polling_transmit(SpiHandle, &Spi_Trans) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al transmitir");
        return;
    }
    spi_device_release_bus(SpiHandle); // Liberamos el bus SPI

    ESP_LOGI(TAG, "Valor leido en el registro 0x00: %x", Spi_Trans.rx_data[0]);

    Spi_Trans.addr = (0x1B | 0X80);

    spi_device_acquire_bus(SpiHandle, portMAX_DELAY); // Adquirimos el bus SPI para poder transmitir
    if (spi_device_polling_transmit(SpiHandle, &Spi_Trans) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al transmitir");
        return;
    }
    spi_device_release_bus(SpiHandle); // Liberamos el bus SPI

    ESP_LOGI(TAG, "Valor leido en el registro 0x1B : %x ", Spi_Trans.rx_data[0]);
}

/* Se recibe un entero de 8 bits*/
uint8_t bmi_getId()
{
    return 0;
}