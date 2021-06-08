/**
 * @file BME280_conversion.cpp
 * @author Kacper Kowalski (kacper.s.kowalski@gmail.com)
 * @brief Defines the conversion from the raw data obtained from BME280 to the real values.
 * @date 2019-12-03
 */
#include "bme280_conversion.hpp"

#include <cinttypes>

namespace jungles
{

namespace BME280
{

// --------------------------------------------------------------------------------------------------------------------
// Declaration of private functions
// --------------------------------------------------------------------------------------------------------------------
static int32_t calculate_fine_temperature(int32_t temperature_raw, const BME280::CallibrationData&);
static float convert_temperature(int32_t fine_temperature);
static float convert_pressure(int32_t pressure, int32_t fine_temperature, const BME280::CallibrationData&);
static float convert_humidity(int32_t humidity, int32_t fine_temperature, const BME280::CallibrationData&);

// ---------------------------------------------------------------------------------------------------------------------
// Definition of public functions
// ---------------------------------------------------------------------------------------------------------------------
BME280Measurement to_real_values(const BME280::CallibrationData& callib_data, const BME280::RawData& regs)
{
    int32_t temperature_raw{(regs.temperature_msb << 12) | (regs.temperature_lsb << 4) | (regs.temperature_xlsb >> 4)};
    int32_t pressure_raw{(regs.pressure_msb << 12) | (regs.pressure_lsb << 4) | (regs.pressure_xlsb >> 4)};
    int32_t humidity_raw{(regs.humidity_msb << 8) | (regs.humidity_lsb)};

    auto t_fine{calculate_fine_temperature(temperature_raw, callib_data)};
    auto temperature{convert_temperature(t_fine)};
    auto pressure{convert_pressure(pressure_raw, t_fine, callib_data)};
    auto humidity{convert_humidity(humidity_raw, t_fine, callib_data)};

    return {temperature, pressure, humidity};
}

// --------------------------------------------------------------------------------------------------------------------
// Definition of private functions
// --------------------------------------------------------------------------------------------------------------------
static float convert_temperature(int32_t fine_temperature)
{
    auto t_fine = fine_temperature;
    float T = (t_fine * 5 + 128) >> 8;
    fine_temperature = t_fine;
    return T / 100.0;
}

static float
convert_pressure(int32_t pressure, int32_t fine_temperature, const BME280::CallibrationData& CallibrationData)
{
    int64_t var1, var2, p;
    int32_t adc_P = pressure;
    auto t_fine = fine_temperature;

    var1 = ((int64_t) t_fine) - 128000;
    var2 = var1 * var1 * (int64_t) CallibrationData.dig_P6;
    var2 = var2 + ((var1 * (int64_t) CallibrationData.dig_P5) << 17);
    var2 = var2 + (((int64_t) CallibrationData.dig_P4) << 35);
    var1 =
        ((var1 * var1 * (int64_t) CallibrationData.dig_P3) >> 8) + ((var1 * (int64_t) CallibrationData.dig_P2) << 12);
    var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) CallibrationData.dig_P1) >> 33;

    if (var1 == 0)
    {
        return 0.0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t) CallibrationData.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t) CallibrationData.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t) CallibrationData.dig_P7) << 4);
    return (float) p / 256.0;
}

static float
convert_humidity(int32_t humidity, int32_t fine_temperature, const BME280::CallibrationData& CallibrationData)
{
    int32_t v_x1_u32r;
    int32_t adc_H = humidity;
    auto t_fine = fine_temperature;
    v_x1_u32r = (t_fine - ((int32_t) 76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t) CallibrationData.dig_H4) << 20)
                    - (((int32_t) CallibrationData.dig_H5) * v_x1_u32r))
                   + ((int32_t) 16384))
                  >> 15)
                 * (((((((v_x1_u32r * ((int32_t) CallibrationData.dig_H6)) >> 10)
                        * (((v_x1_u32r * ((int32_t) CallibrationData.dig_H3)) >> 11) + ((int32_t) 32768)))
                       >> 10)
                      + ((int32_t) 2097152))
                         * ((int32_t) CallibrationData.dig_H2)
                     + 8192)
                    >> 14));

    v_x1_u32r =
        (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) CallibrationData.dig_H1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    float h = (v_x1_u32r >> 12);
    return h / 1024.0;
}

static int32_t calculate_fine_temperature(int32_t temperature_raw, const BME280::CallibrationData& CallibrationData)
{
    int32_t var1, var2;

    int32_t adc_T = temperature_raw;

    var1 = ((((adc_T >> 3) - ((int32_t) CallibrationData.dig_T1 << 1))) * ((int32_t) CallibrationData.dig_T2)) >> 11;

    var2 =
        (((((adc_T >> 4) - ((int32_t) CallibrationData.dig_T1)) * ((adc_T >> 4) - ((int32_t) CallibrationData.dig_T1)))
          >> 12)
         * ((int32_t) CallibrationData.dig_T3))
        >> 14;

    return (var1 + var2);
}

} // namespace BME280

} // namespace jungles
