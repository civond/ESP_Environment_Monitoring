#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "driver/gpio.h"

//#define DEVICE_ADDR     0x76  // BME688 Sensor


// Custom functions
//#include "i2c_helper.h"
#include "bme688.h"

#define I2C_PORT        I2C_NUM_0
#define I2C_SDA_IO      2
#define I2C_SCL_IO      1
#define I2C_FREQ_HZ 100000  // 100 kHz

void i2c_master_init(void) {
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SDA_IO;
    conf.scl_io_num = I2C_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_FREQ_HZ;

    i2c_param_config(I2C_PORT, &conf);
    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
}

extern "C" void app_main(void)
{
    i2c_master_init();
    BME688 sensor;
    esp_err_t status;
    uint8_t err = BME688_INITIALIZE(&sensor, I2C_PORT);
    if (err == 0) {
            printf("Initialization completed with 0 errors!\n");
            printf("Running loop\n");
        } else {
            printf("Number of errors: %d\n", err);
        }
   
    while (1) {
        // Force sensor to take new measurement
        status = BME688_ForceMeasurement(&sensor);
        printf("Status: %d\n", status);

        // Read Temp, Press, Hum Data
        status = BME688_ReadTemperature(&sensor);
        printf("\tTemperature: %.2f °C\n", sensor.temp_c);
        status = BME688_ReadPressure(&sensor);
        printf("\tPressure: %.2f Pa\n", sensor.pressure);
        status = BME688_ReadHumidity(&sensor);
        printf("\tHumidity: %.2f \n", sensor.humidity);

        // Read and write gas
        status = BME688_WriteGas(&sensor);
        status = BME688_ReadGas(&sensor);
        printf("\tGas Resistance: %lu ohms\n", sensor.gas_res);

    
        vTaskDelay(pdMS_TO_TICKS(2000)); // Wait 2 seconds before next measurement
    }
}
