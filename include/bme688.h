#ifdef __cplusplus
extern "C" {
#endif

#include "driver/i2c.h"

// ------ BME688 I2C Driver ------
// Written by Dorian Yeh

#define BME688_I2C_ADDR 0x76                            // SDO -> GND = 0x76, SDO -> VDDIO = 0x77

// ------ BME688 Datasheet ------ || Pg. 36
#define BME688_DEVICE_ID                    0x01
#define BME688_VARIANT_ID                   0XF0        // value should be 0x01 (hex)
#define BME688_CHIP_ID                      0XD0        //

#define BME688_CTRL_HUM                     0x72
#define BME688_CTRL_MEAS                    0x74
#define BME688_CONFIG                       0x75        // IIR Filter control

// ------ Gas Control Registers ------ || Pg. 38
#define BME688_IDAC_HEAT_0                  0x50
#define BME688_IDAC_HEAT_1                  0x51
#define BME688_IDAC_HEAT_2                  0x52
#define BME688_IDAC_HEAT_3                  0x53
#define BME688_IDAC_HEAT_4                  0x54
#define BME688_IDAC_HEAT_5                  0x55
#define BME688_IDAC_HEAT_6                  0x56
#define BME688_IDAC_HEAT_7                  0x57
#define BME688_IDAC_HEAT_8                  0x58
#define BME688_IDAC_HEAT_9                  0x59

#define BME688_RES_HEAT_0                   0x5A
#define BME688_RES_HEAT_1                   0x5B
#define BME688_RES_HEAT_2                   0x5C
#define BME688_RES_HEAT_3                   0x5D
#define BME688_RES_HEAT_4                   0x5E
#define BME688_RES_HEAT_5                   0x5F
#define BME688_RES_HEAT_6                   0x60
#define BME688_RES_HEAT_7                   0x61
#define BME688_RES_HEAT_8                   0x62
#define BME688_RES_HEAT_9                   0x63

#define BME688_GAS_WAIT_0                   0x64
#define BME688_GAS_WAIT_1                   0x65
#define BME688_GAS_WAIT_2                   0x66
#define BME688_GAS_WAIT_3                   0x67
#define BME688_GAS_WAIT_4                   0x68
#define BME688_GAS_WAIT_5                   0x69
#define BME688_GAS_WAIT_6                   0x6A
#define BME688_GAS_WAIT_7                   0x6B
#define BME688_GAS_WAIT_8                   0x6C
#define BME688_GAS_WAIT_9                   0x6D
#define BME688_GAS_WAIT_SHARED              0x6E

#define BME688_CTRL_GAS_0                   0x70
#define BME688_CTRL_GAS_1                   0x71

// ------ Data registers ------ || Pg. 41
// Pressure
#define BME688_PRESS_MSB_0                  0x1F        // MSB [19:12]
#define BME688_PRESS_MSB_1                  0x30
#define BME688_PRESS_MSB_2                  0x41

#define BME688_PRESS_LSB_0                  0x20        // LSB [11:4]
#define BME688_PRESS_LSB_1                  0x31
#define BME688_PRESS_LSB_2                  0x42

#define BME688_PRESS_XLSB_0                 0x21        // XLSB [3:0]
#define BME688_PRESS_XLSB_1                 0x32
#define BME688_PRESS_XLSB_2                 0x43

// Temperature
#define BME688_TEMP_MSB_0                   0x22        // MSB [19:12]
#define BME688_TEMP_MSB_1                   0x33
#define BME688_TEMP_MSB_2                   0x44

#define BME688_TEMP_LSB_0                   0x23        // LSB [11:4]
#define BME688_TEMP_LSB_1                   0x34
#define BME688_TEMP_LSB_2                   0x45

#define BME688_TEMP_XLSB_0                  0x24        // XLSB [3:0]
#define BME688_TEMP_XLSB_1                  0x35
#define BME688_TEMP_XLSB_2                  0x46

// Humidity
#define BME688_HUM_MSB_0                    0x25        // MSB [15:8]
#define BME688_HUM_MSB_1                    0x36
#define BME688_HUM_MSB_2                    0x47

#define BME688_HUM_LSB_0                    0x26        // LSB [7:0]
#define BME688_HUM_LSB_1                    0x37
#define BME688_HUM_LSB_2                    0x48

// Gas Resistance
#define BME688_GAS_R_MSB_0                  0x2C        // MSB [9:2]
#define BME688_GAS_R_MSB_1                  0x3D        // 7:0 contains data
#define BME688_GAS_R_MSB_2                  0x4E

#define BME688_GAS_R_LSB_0                  0x2D        // LSB [1:0]
#define BME688_GAS_R_LSB_1                  0x3E        // 7:6 contains data for gas res.
#define BME688_GAS_R_LSB_2                  0x4F        // 3:0 contains ADC range of measured gas res.

// ------ Status Registers ------ || Pg. 43
// Measuring Status
#define BME688_MEAS_STATUS_0                0x1D
#define BME688_MEAS_STATUS_1                0x2E
#define BME688_MEAS_STATUS_2                0x3F

// Sub Measurement Index
#define BME688_SUB_MEAS_INDEX_0             0x1E 
#define BME688_SUB_MEAS_INDEX_1             0x2F 
#define BME688_SUB_MEAS_INDEX_2             0x40

// ------ Calibration Variable Names ------
// Temperature
#define BME688_CALIB_PAR_T1_LSB             0xE9
#define BME688_CALIB_PAR_T1_MSB             0xEA
#define BME688_CALIB_PAR_T2_LSB             0x8A
#define BME688_CALIB_PAR_T2_MSB             0x8B 
#define BME688_CALIB_PAR_T3                 0x8C

// Humidity
#define BME688_CALIB_PAR_H1_LSB             0xE2         //3:0
#define BME688_CALIB_PAR_H1_MSB             0xE3
#define BME688_CALIB_PAR_H2_LSB             0xE2         //7:4
#define BME688_CALIB_PAR_H2_MSB             0xE1
#define BME688_CALIB_PAR_H3                 0xE4
#define BME688_CALIB_PAR_H4                 0xE5
#define BME688_CALIB_PAR_H5                 0xE6
#define BME688_CALIB_PAR_H6                 0xE7
#define BME688_CALIB_PAR_H7                 0xE8

// Pressure
#define BME688_CALIB_PAR_P1_LSB             0x8E
#define BME688_CALIB_PAR_P1_MSB             0x8F
#define BME688_CALIB_PAR_P2_LSB             0x90
#define BME688_CALIB_PAR_P2_MSB             0x91
#define BME688_CALIB_PAR_P3                 0x92
#define BME688_CALIB_PAR_P4_LSB             0x94
#define BME688_CALIB_PAR_P4_MSB             0x95
#define BME688_CALIB_PAR_P5_LSB             0x96
#define BME688_CALIB_PAR_P5_MSB             0x97
#define BME688_CALIB_PAR_P6                 0x99
#define BME688_CALIB_PAR_P7                 0x98
#define BME688_CALIB_PAR_P8_LSB             0x9C
#define BME688_CALIB_PAR_P8_MSB             0x9D
#define BME688_CALIB_PAR_P9_LSB             0x9E
#define BME688_CALIB_PAR_P9_MSB             0x9F
#define BME688_CALIB_PAR_P10                0xA0

// Gas
#define BME688_CALIB_PAR_G1                 0xED
#define BME688_CALIB_PAR_G2_LSB             0xEB
#define BME688_CALIB_PAR_G2_MSB             0xEC
#define BME688_CALIB_PAR_G3                 0xEE
#define BME688_CALIB_RES_HEAT_RANGE         0x02         //5:4
#define BME688_CALIB_RES_HEAT_VAL           0x00

// ------ Data Storage ------
typedef struct {
    i2c_port_t i2c_port;
    uint8_t address;

    float temp_c;               // Degrees C
    float pressure;             // Pascals
    float humidity;             // %Humidity
    int32_t gas_res;            // ohms


    int32_t t_fine;             // Store this value from temperature for pressure/humidity compensation

    // Add gas calculations later


    // ------ Calibration parameters ------
    // Coefficients for temp
    uint16_t    par_t1;
    int16_t     par_t2;
    int8_t      par_t3;

    // Coefficients for humidity
    uint16_t    par_h1;
    uint16_t    par_h2;
    int8_t      par_h3;
    int8_t      par_h4;
    int8_t      par_h5;
    uint8_t     par_h6;
    int8_t      par_h7;

    // Coefficients for pressure
    uint16_t    par_p1;
    int16_t     par_p2;
    int8_t      par_p3;
    int16_t     par_p4;
    int16_t     par_p5;
    int8_t      par_p6;
    int8_t      par_p7;
    int16_t     par_p8;
    int16_t     par_p9;
    uint8_t     par_p10; 

    // Coefficients for gas
    uint8_t     par_g1;
    int16_t     par_g2;
    int8_t      par_g3;
    uint8_t     res_heat_range;
    int8_t      res_heat_val;

} BME688;


// ------ Initialization ------
uint8_t BME688_INITIALIZE (
    BME688 *dev, 
    i2c_port_t port
);

// ------ Data Acquisition Functions ------
esp_err_t BME688_ForceMeasurement(BME688 *dev);
esp_err_t BME688_ReadTemperature(BME688 *dev);
esp_err_t BME688_ReadPressure(BME688 *dev);
esp_err_t BME688_ReadHumidity(BME688 *dev);

esp_err_t BME688_WriteGas(BME688 *dev);
esp_err_t BME688_ReadGas(BME688 *dev);

// ------ Low Level Functions ------
esp_err_t BME688_ReadRegister(
    BME688 *dev, 
    uint8_t reg, 
    uint8_t *data
);

esp_err_t BME688_WriteRegister(
    BME688 *dev, 
    uint8_t reg, 
    uint8_t *data
);

#ifdef __cplusplus
}
#endif