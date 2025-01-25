#include "localbmi160.h"
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

const char TAG[] = "BMI_DRIVER"; // Etiqueta para los mensajes de log

static spi_device_handle_t SpiHandle = 0; // Manejador del dispositivo SPI

int8_t bmi_read_spi(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t bmi_write_spi(uint8_t dev_addr, uint8_t reg_addr, uint8_t *read_data, uint16_t len);
void bmi_delay(uint32_t period);

/* Funcion para inicializar el BMI160 */
bmi160_dev bmi_init_Spi()
{
    // Configuración del bus SPI
    spi_bus_config_t busSpi = {0};
    spi_device_interface_config_t spiIf = {0};
    
    bmi160_dev dev; // Estructura del BMI160

    busSpi.mosi_io_num = GPIO_NUM_23;
    busSpi.miso_io_num = GPIO_NUM_19;
    busSpi.sclk_io_num = GPIO_NUM_18;
    busSpi.quadwp_io_num = -1;
    busSpi.quadhd_io_num = -1;
    busSpi.max_transfer_sz = 4092; // Tamaño maximo de la transferencia

    // Inicializar el bus SPI
    if (spi_bus_initialize(SPI3_HOST, &busSpi, SPI_DMA_DISABLED) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al inicializar el bus SPI");
        return dev;
    }
     else
    {
        ESP_LOGW(TAG, "SPI BUS añadido correctamente");

    }
    
    // Configuración del dispositivo SPI
    spiIf.spics_io_num = GPIO_NUM_21;       // GPIO que utilicemos para CS
    spiIf.clock_speed_hz = 1 * 1000 * 1000; // Velocidad comunicacion de 1Mhz
    spiIf.mode = 0;                         // Modo SPI 0: CPOL=0, CPHA=0
    spiIf.queue_size = 10;                   // Tamaño de la cola de transacciones
    spiIf.address_bits = 8;                 // la dirección de 8 bits si no lo pongo no funciona

    // Añadir device al SPI
    if (spi_bus_add_device(SPI3_HOST, &spiIf, &SpiHandle) != ESP_OK)
    {
        ESP_LOGE(TAG, "SPI Device cannot be added");
    }
    else
    {
        ESP_LOGW(TAG, "Dispositivo SPI añadido correctamente");

    }
        
    dev.intf = BMI160_SPI_INTF; // Interfaz SPI
    dev.read = bmi_read_spi;    // Función de lectura
    dev.write = bmi_write_spi;  // Función de escritura
    dev.delay_ms = bmi_delay;   // Función de delay

    return dev;
}

/*Lectura de un registro del BMI160, cogida de bmi160_defs.h
    * @param reg_addr Dirección del registro a leer
    * @param data Puntero a la variable donde se guardará el valor leído
    * @param len Longitud de los datos a leer
    * @return 0 si la lectura ha sido correcta, 1 si ha habido un error
*/
int8_t bmi_read_spi(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    spi_transaction_t Spi_Trans = {0};

    Spi_Trans.length = 8*len; // Transmitimos en 8 bits * len
    Spi_Trans.addr = (dev_addr);// Direción a leer 0x00 
    Spi_Trans.rxlength = 8*len; // Recibimos un byte, 8 bits
    Spi_Trans.rx_buffer = data; // Buffer de recepción se pone data porque es un puntero

    spi_device_acquire_bus(SpiHandle, portMAX_DELAY); // Adquirimos el bus SPI para poder transmitir (si no esta tambien funciona) 
    if (spi_device_polling_transmit(SpiHandle, &Spi_Trans) != ESP_OK) //Utilizo polling transmit porque no utilizo DMA
    {
        ESP_LOGE(TAG, "Error al transmitir");
        spi_device_release_bus(SpiHandle);
        return 1;
    }
    spi_device_release_bus(SpiHandle); // Liberamos el bus SPI

    return BMI160_OK;
}

/*Escritura de un registro del BMI160, cogida de bmi160_defs.h
    * @param reg_addr Dirección del registro a escribir
    * @param data Puntero a la variable donde se guardará el valor a escribir
    * @param len Longitud de los datos a escribir
    * @return 0 si la escritura ha sido correcta, 1 si ha habido un error
*/
int8_t bmi_write_spi(uint8_t dev_addr, uint8_t reg_addr, uint8_t *read_data, uint16_t len)
{
    spi_transaction_t Spi_Trans = {0};

    Spi_Trans.length = 8*len; // Transmitimos en 8 bits * len
    Spi_Trans.addr = (dev_addr);// Direción a leer 0x00 
    Spi_Trans.tx_buffer = read_data; // Buffer de recepción se pone read_data porque es un puntero
    
    spi_device_acquire_bus(SpiHandle, portMAX_DELAY); // Adquirimos el bus SPI para poder transmitir (si no esta tambien funciona) 
    if (spi_device_polling_transmit(SpiHandle, &Spi_Trans) != ESP_OK) //Utilizo polling transmit porque no utilizo DMA
    {
        ESP_LOGE(TAG, "Error al transmitir");
        spi_device_release_bus(SpiHandle);
        return 1;
    }
    spi_device_release_bus(SpiHandle); // Liberamos el bus SPI

    return BMI160_OK;
}

/*Delay para el BMI160, cogida de bmi160_defs.h
* @param period Periodo de tiempo en milisegundos
*/
void bmi_delay(uint32_t period)
{
    // 10ms delay es el minimo por frertos que es tick
    if (period < 10)
    {
        period = 10;
    }
    vTaskDelay(pdMS_TO_TICKS(period));
    return ;
}
