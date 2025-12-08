#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include <string.h>
#include "driver/gpio.h"

//#define DEVICE_ADDR     0x76  // BME688 Sensor


// Drivers
#include "bme688.h"
#include "ssd1306.h"

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

void i2c_scan() {
    printf("Scanning I2C bus...\n");
    for (uint8_t addr = 1; addr < 127; addr++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret == ESP_OK) {
            printf("Found device at 0x%02X\n", addr);
        }
    }
    printf("Scan done.\n");
}

// ------ Main ------
extern "C" void app_main(void)
{   
    // Initialize I2C bus
    i2c_master_init();

    // Initialize sensor/screen
    BME688 sensor;
    SSD1306_t screen;
    memset(&screen, 0, sizeof(screen));
    screen._address = 0x3C;         // REQUIRED
    screen._i2c_num = I2C_PORT;     // REQUIRED (0 or 1)

    esp_err_t status;
    uint8_t err = BME688_INITIALIZE(&sensor, I2C_PORT);

    ssd1306_init(&screen, 128, 64);             // for 128x64 panel
    ssd1306_clear_screen(&screen, false);       // clear, with default background (black)
    ssd1306_contrast(&screen, 0xff); 

    if (err == 0) {
            printf("Initialization completed with 0 errors!\n");
            printf("Running loop\n");
        } else {
            printf("Number of errors: %d\n", err);
        }
   
    // Main loop
    while (1) {
        //printf("Loop running\n");
        //i2c_scan();
        //vTaskDelay(2000 / portTICK_PERIOD_MS);

        // Force sensor to take new measurement
        status = BME688_ForceMeasurement(&sensor);
        //printf("Status: %d\n", status);

        // Read Temp, Press, Hum Data
        status = BME688_ReadTemperature(&sensor);
        //printf("\tTemperature: %.2f °C\n", sensor.temp_c);

        status = BME688_ReadPressure(&sensor);
        float pressure_kpa = sensor.pressure_pa / 1000.0f;
        //printf("\tPressure: %.2f Pa\n", pressure_kpa);
        
        status = BME688_ReadHumidity(&sensor);
        //printf("\tHumidity: %.2f \n", sensor.humidity);

        // Read and write gas
        status = BME688_WriteGas(&sensor);
        status = BME688_ReadGas(&sensor);
        float gas_res_kohm = sensor.gas_res_ohm / 1000.0f;
        //printf("\tGas Resistance: %.2f ohms\n", gas_res_kohm);

        printf(
        "%.2f,%.2f,%.2f,%lu\n",
            sensor.temp_c,
            sensor.pressure_pa,
            sensor.humidity,
            sensor.gas_res_ohm
        );

        // Format data for SSD1306
        char line0[20], line1[20], line2[20], line3[20];
        snprintf(line0, sizeof(line0), "Temp: %.1f C", sensor.temp_c);
        snprintf(line1, sizeof(line1), "Press: %.1f kPa", pressure_kpa);
        snprintf(line2, sizeof(line2), "Hum: %.1f%%", sensor.humidity);
        snprintf(line3, sizeof(line3), "GasR: %.1f kOhms", gas_res_kohm);
        ssd1306_clear_screen(&screen, false);

        // Display Text
        ssd1306_display_text(&screen, 0, line0, strlen(line0), false);
        ssd1306_display_text(&screen, 1, line1, strlen(line1), false);
        ssd1306_display_text(&screen, 2, line2, strlen(line2), false);
        ssd1306_display_text(&screen, 3, line3, strlen(line3), false);

        vTaskDelay(pdMS_TO_TICKS(10000)); // Wait 10 seconds before next measurement
        
        }
}
