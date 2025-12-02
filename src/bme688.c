#include "bme688.h"
#include "esp_err.h"
#include "driver/i2c.h"
#include <stdint.h>

// ------ BME688 Initialization Function ------
uint8_t BME688_INITIALIZE(BME688 *dev, i2c_port_t port) {
    dev ->i2c_port          = port;
    dev ->address           = BME688_I2C_ADDR;
    dev ->humidity          = 0.0f;
    dev ->temp_c            = 0.0f;
    dev ->pressure          = 0.0f;

    uint8_t errNum = 0;
    esp_err_t status;
    uint8_t registerData;
    
    // Read the contents of the variant id register
    status = BME688_ReadRegister(dev, BME688_VARIANT_ID, &registerData);

    // Check Device ID
    errNum += (status !=ESP_OK);
    if (registerData != BME688_DEVICE_ID)
        return 255;

    // Set Device Polling Mode
    status = BME688_ReadRegister(dev, BME688_CTRL_MEAS, &registerData);
    errNum += (status !=ESP_OK);

    registerData &= 0xFC;               // clears bits in positions 1:0
    registerData |= 0x01;               // sets bits 1:0 to 01 (forced mode, pg. 35)
    status = BME688_WriteRegister(dev, BME688_CTRL_MEAS, &registerData);
    errNum += (status !=ESP_OK);

    // Set Oversampling for Humidity
    status = BME688_ReadRegister(dev, BME688_CTRL_HUM, &registerData);
    errNum += (status !=ESP_OK);
    registerData &= 0xF8;               // clears bits in positions 2:0
    registerData |= 0b100;              // sets bits 2:0 to b100 (8x)
    status = BME688_WriteRegister(dev, BME688_CTRL_HUM, &registerData);
    errNum += (status !=ESP_OK);

    // Set Oversampling for Temp/Pressure
    status = BME688_ReadRegister(dev, BME688_CTRL_MEAS, &registerData);
    errNum += (status !=ESP_OK);
    registerData &= 0x03;               // clears bits in positions 7:2
    registerData |= (0b100 << 5);       // sets bits 7:5 to 8x
    registerData |= (0b100 << 2);       // sets bits 4:2 to 8x
    status = BME688_WriteRegister(dev, BME688_CTRL_MEAS, &registerData);
    errNum += (status !=ESP_OK);

    // Set IIR Filter Coeffs. for Temp/Pressure
    status = BME688_ReadRegister(dev, BME688_CONFIG, &registerData);
    errNum += (status !=ESP_OK);

    registerData &= 0xE3;               // clears bits in positions 4:2
    registerData |= (0b010 << 2);       // sets bits 4:2 to b011 (order 3) 
    status = BME688_WriteRegister(dev, BME688_CONFIG, &registerData);
    errNum += (status !=ESP_OK);

    // ------ Read Calibration Variables ------
    uint8_t lsb, msb;

    // Initialize Temperature Variables
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_T1_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_T1_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_t1 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_T2_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_T2_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_t2 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_T3, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_t3 = lsb;
    
    // Initialize Pressure Variables
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P1_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P1_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_p1 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P2_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P2_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_p2 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P3, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_p3 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P4_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P4_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_p4 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P5_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P5_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_p5 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P6, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_p6 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P7, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_p7 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P8_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P8_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_p8 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P9_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P9_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_p9 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_P10, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_p10 = lsb;

    // Initialize Humidity Variables
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H1_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H1_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_h1 = (msb << 4) | (lsb & 0x0F);

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H2_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H2_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_h2 = (msb << 4) | (lsb & 0xF0);

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H3, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_h3 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H4, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_h4 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H5, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_h5 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H6, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_h6 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_H7, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_h7 = lsb;

    // Initialize Gas Variables
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_G1, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_g1 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_G2_LSB, &lsb);
    errNum += (status !=ESP_OK);
    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_G2_MSB, &msb);
    errNum += (status !=ESP_OK);
    dev->par_g2 = (msb << 8) | lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_PAR_G3, &lsb);
    errNum += (status !=ESP_OK);
    dev->par_g3 = lsb;

    status = BME688_ReadRegister(dev, BME688_CALIB_RES_HEAT_RANGE, &lsb);
    errNum += (status !=ESP_OK);
    dev->res_heat_range = (lsb & 0x30) >> 4;

    status = BME688_ReadRegister(dev, BME688_CALIB_RES_HEAT_VAL, &lsb);
    errNum += (status !=ESP_OK);
    dev->res_heat_val = lsb;
    

    // Return number of errors
    return errNum;
}

// ------ Low Level Functions ------
// Read Register
esp_err_t BME688_ReadRegister(BME688 *dev, uint8_t reg, uint8_t *data) {
    return i2c_master_write_read_device(
        dev->i2c_port,                  // I2C port (ex: I2C_NUM_0)
        dev->address,                   // sensor I2C address (0x76 or 0x77)
        &reg,                           // buffer containing register address
        1,                              // 1 byte (8 bits) size of register address
        data,                           // buffer to store read data
        1,                              // number of bytes to read
        1000 / portTICK_PERIOD_MS       // timeout in ticks
    );
}

// Write Register
esp_err_t BME688_WriteRegister(BME688 *dev, uint8_t reg, uint8_t *data) {
    uint8_t buffer[2] = {reg, *data};   // ESPIDF expects register writes to be two bytes
    
    return i2c_master_write_to_device(
        dev->i2c_port,                  // I2C port (ex:I2C_NUM_0)
        dev->address,                   // sensor I2C address (0x76 or 0x77)
        buffer,                         // buffer containing register address
        2,                              // 2 bytes (16 bits) 
        1000 / portTICK_PERIOD_MS       // timeout in ticks
    );
}


// ------ Trigger forced measurement ------
esp_err_t BME688_ForceMeasurement(BME688 *dev) {
    esp_err_t status;
    uint8_t registerData;

    status = BME688_ReadRegister(dev, BME688_CTRL_MEAS, &registerData);
    if (status != ESP_OK) return status;

    registerData &= 0xFC;               // Clear mode bits (bits 1:0)
    registerData |= 0x01;               // Set forced mode
    status = BME688_WriteRegister(dev, BME688_CTRL_MEAS, &registerData);
    if (status != ESP_OK) return status;
    vTaskDelay(pdMS_TO_TICKS(10));      // Small delay before polling data registers
    
    return ESP_OK;
}
// ------ Measure Temperature Data ------
esp_err_t BME688_ReadTemperature(BME688 *dev) {
    uint8_t regData[3];                  // 24 bit data
    esp_err_t status;

    float var1;
    float var2;
    float temp_comp;

    status  = BME688_ReadRegister(dev, BME688_TEMP_MSB_0, &regData[0]);
    status |= BME688_ReadRegister(dev, BME688_TEMP_LSB_0, &regData[1]);
    status |= BME688_ReadRegister(dev, BME688_TEMP_XLSB_0, &regData[2]);
    if (status != ESP_OK) return status;

    // Calculate raw temperature
    // MSB [19:12], LSB [11:4], XLSB [3:0]
    int32_t temp_raw = ((int32_t)regData[0] << 12) | 
                       ((int32_t)regData[1] << 4) | 
                       ((int32_t)(regData[2] >> 4) & 0x0F);

    // Convert raw temperature using calibration values
    var1 = (((double)temp_raw / 16384.0) - ((double)dev->par_t1 / 1024.0)) * (double)dev->par_t2;
    var2 = ((((double)temp_raw / 131072.0) - ((double)dev->par_t1 / 8192.0)) * (((double)temp_raw / 131072.0) - ((double)dev->par_t1 / 8192.0)) * ((double)dev->par_t3 * 16.0));
    int32_t t_fine = var1 + var2;
    temp_comp = t_fine / 5120.0;
    dev->temp_c = temp_comp;
    dev->t_fine = t_fine;

    return ESP_OK;
}


// ------ Measure Pressure Data ------
esp_err_t BME688_ReadPressure(BME688 *dev) {
    uint8_t regData[3];                  // 24 bit data
    esp_err_t status;

    // Calculate raw pressure
    // MSB [19:12], LSB [11:4], XLSB [3:0]
    status  = BME688_ReadRegister(dev, BME688_PRESS_MSB_0, &regData[0]);
    status |= BME688_ReadRegister(dev, BME688_PRESS_LSB_0, &regData[1]);
    status |= BME688_ReadRegister(dev, BME688_PRESS_XLSB_0, &regData[2]);
    if (status != ESP_OK) return status;

    int32_t press_raw = ((int32_t)regData[0] << 12) | 
                       ((int32_t)regData[1] << 4) | 
                       ((int32_t)(regData[2] >> 4) & 0x0F);

    // Convert raw pressure using calibration values (pg. 24)
    int64_t var1, var2, var3, press_comp;

    var1 = ((double)dev->t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)dev->par_p6 / 131072.0);
    var2 = var2 + (var1 * (double)dev->par_p5 * 2.0);
    var2 = (var2 / 4.0) + ((double)dev->par_p4 * 65536.0);
    var1 = ((((double)dev->par_p3 * var1 * var1) / 16384.0) + ((double)dev->par_p2 * var1)) / 524288.0;

    var1 = (1.0 + (var1 / 32768.0)) * (double)dev->par_p1;
    press_comp = 1048576.0 - (double)press_raw;
    press_comp = ((press_comp - (var2 / 4096.0)) * 6250.0) / var1; 
    var1 = ((double)dev->par_p9 * press_comp * press_comp) / 2147483648.0;
    var2 = press_comp * ((double)dev->par_p8 / 32768.0);
    var3 = (press_comp / 256.0) * (press_comp / 256.0) * (press_comp / 256.0) * (dev->par_p10 / 131072.0);
    press_comp = press_comp + (var1 + var2 + var3 + ((double)dev->par_p7 * 128.0)) / 16.0;
    //press_comp = press_comp / 1000.0;
    dev->pressure = press_comp;
    
    return ESP_OK;
}

// ------ Measure Humidity Data ------
esp_err_t BME688_ReadHumidity(BME688 *dev) {
    uint8_t regData[2];                  // 16 bit data
    esp_err_t status;

    status  = BME688_ReadRegister(dev, BME688_HUM_MSB_0, &regData[0]);
    status |= BME688_ReadRegister(dev, BME688_HUM_LSB_0, &regData[1]);

    uint16_t hum_raw = ((int32_t)regData[0] << 8) | 
                       ((int32_t)regData[1]);

    // Convert raw humidity using calibration values (pg. 25)
    float var1, var2, var3, var4, hum_comp, temp_comp;

    temp_comp = ((dev->t_fine) / 5120.0);

    var1 = hum_raw - (((double)dev->par_h1 * 16.0) + (((double)dev->par_h3 / 2.0) * temp_comp));
    var2 = var1 * (((double)dev->par_h2 / 262144.0) * (1.0 + (((double)dev->par_h4 / 16384.0) * temp_comp) + (((double)dev->par_h5 / 1048576.0) * temp_comp * temp_comp)));
    var3 = (double)dev->par_h6 / 16384.0;
    var4 = (double)dev->par_h7 / 2097152.0;
    hum_comp = var2 + ((var3 + (var4 * dev->temp_c)) * var2 *var2);
    dev->humidity = hum_comp;

    return ESP_OK;
}




esp_err_t BME688_WriteGas(BME688 *dev) {
    esp_err_t status;

    // Use calibration variables
    float var1;
    float var2;
    float var3;
    float var4;
    float var5;
    float target_temp = 250.0; // 250°C
    uint8_t gas_wait = 0b01110010; // corresponds to: 00 (1x),  110010 = 50
    uint8_t ctrl_gas_1 = 0x20; // 0b00100000. run gas: bit5, heater_step = 0
    uint8_t res_heat;

    // Calculate resistance value
    var1 = ((double)dev->par_g1 / 16.0) + 49.0;
    var2 = (((double)dev->par_g2 / 32768.0) * 0.0005) + 0.00235;
    var3 = (double)dev->par_g3 / 1024.0;
    var4 = var1 * (1.0 + (var2 * (double)target_temp));
    var5 = var4 + (var3 * (float)dev->temp_c);          // Heater resistance in ohms

    // res_heat should be written to res_heat_x register
    res_heat = (uint8_t)(3.4 * ((var5 * (4.0 / (4.0 + (float)dev->res_heat_range)) * (1.0 / (1.0 + ((float)dev->res_heat_val * 0.002)))) - 25));
    status = BME688_WriteRegister(dev, BME688_RES_HEAT_0, &res_heat);
    if (status != ESP_OK) return status;

    // Write wait time to gas_wait register
    status = BME688_WriteRegister(dev, BME688_GAS_WAIT_0, &gas_wait);
    if (status != ESP_OK) return status;

    // Turn on heater
    status = BME688_WriteRegister(dev, BME688_CTRL_GAS_1, &ctrl_gas_1);
    if (status != ESP_OK) return status;
    //printf("%d\n", (int)res_heat);
    vTaskDelay(pdMS_TO_TICKS(300));  // Delay 30 milliseconds

    return ESP_OK;
}


// ------ Measure Gas Data ------
esp_err_t BME688_ReadGas(BME688 *dev) {
    uint8_t regData[2];                  // 16 bit data
    esp_err_t status;

    // Extract raw gas data
    status  = BME688_ReadRegister(dev, BME688_GAS_R_MSB_0, &regData[0]);
    status |= BME688_ReadRegister(dev, BME688_GAS_R_LSB_0, &regData[1]);
    uint16_t gas_r_raw = ((uint16_t)regData[0] << 2) | 
                            ((regData[1] & 0xC0) >> 6); 
    uint8_t gas_range = regData[1] & 0x0F; // Positions 3:0 contain gas range

    // Convert raw gas readings using calibration values
    uint32_t var1_r;
    int32_t var2_r;
    uint32_t gas_res;

    var1_r = UINT32_C(262144) >> gas_range;
    var2_r = (int32_t)gas_r_raw - INT32_C(512);
    var2_r *= INT32_C(3);
    var2_r = INT32_C(4096) + var2_r;
    gas_res = 1000000.0 * (float)var1_r / (float)var2_r;
    dev->gas_res = gas_res;

    return ESP_OK;
}