/**
 * @file bme280_conversion.hpp
 * @author Kacper Kowalski (kacper.s.kowalski@gmail.com)
 * @brief Declares function which converts raw data to real values and structures which define the conversion input.
 * @date 2019-12-03
 */
#ifndef __BME280_CONVERSION_HPP__
#define __BME280_CONVERSION_HPP__

#include <cinttypes>

#include "../bme280_measurement.hpp"

namespace jungles
{

namespace BME280
{

union CallibrationData
{
    struct __attribute__((packed))
    {
        // Here the mapped region starts
        uint16_t dig_T1; ///< Temperature compensation value
        int16_t dig_T2;  ///< Temperature compensation value
        int16_t dig_T3;  ///< Temperature compensation value

        uint16_t dig_P1; ///< Pressure compensation value
        int16_t dig_P2;  ///< Pressure compensation value
        int16_t dig_P3;  ///< Pressure compensation value
        int16_t dig_P4;  ///< Pressure compensation value
        int16_t dig_P5;  ///< Pressure compensation value
        int16_t dig_P6;  ///< Pressure compensation value
        int16_t dig_P7;  ///< Pressure compensation value
        int16_t dig_P8;  ///< Pressure compensation value
        int16_t dig_P9;  ///< Pressure compensation value

        uint8_t r1;     ///< Unused 1
        uint8_t dig_H1; ///< Humidity compensation value
        // Here the mapped region ends. The memory region below is not mapped as it uses halfbytes :(

        int16_t dig_H2; ///< Humidity compensation value
        uint8_t dig_H3; ///< Humidity compensation value
        int16_t dig_H4; ///< Humidity compensation value
        int16_t dig_H5; ///< Humidity compensation value
        int8_t dig_H6;  ///< Humidity compensation value
    };
    char mapped_region[26];
};

union RawData
{
    struct __attribute__((packed))
    {
        uint8_t pressure_msb;
        uint8_t pressure_lsb;
        uint8_t pressure_xlsb;
        uint8_t temperature_msb;
        uint8_t temperature_lsb;
        uint8_t temperature_xlsb;
        uint8_t humidity_msb;
        uint8_t humidity_lsb;
    };
    char mapped_region[8];
};

BME280Measurement to_real_values(const BME280::CallibrationData&, const BME280::RawData&);

} // namespace BME280

} // namespace jungles

#endif // __BME280_CONVERSION_HPP__
