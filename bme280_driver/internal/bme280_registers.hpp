/**
 * @file bme280_registers.hpp
 * @author Kacper Kowalski (kacper.s.kowalski@gmail.com)
 * @brief Defines all the registers in the internal BME280 memory.
 * @date 2020-07-31
 */
#ifndef __BME280_REGISTERS_HPP__
#define __BME280_REGISTERS_HPP__

#include <cinttypes>

namespace jungles
{

namespace BME280
{

static inline constexpr uint8_t address = 0xEC;

// --------------------------------------------------------------------------------------------------------------------
// Definitions of helper data types and helper structures
// --------------------------------------------------------------------------------------------------------------------
enum class RegisterAddress : uint8_t
{
    callibration_first_part_beg = 0x88,
    id = 0xD0,
    reset = 0xE0,
    callibration_second_part_beg = 0xE1,
    control_humidity = 0xF2,
    status = 0xF3,
    control_measurement = 0xF4,
    config = 0xF5,
    pressure_msb = 0xF7,
    pressure_lsb = 0xF8,
    pressure_xlsb = 0xF9,
    temperature_msb = 0xFA,
    temperature_lsb = 0xFB,
    temperature_xlsb = 0xFC,
    humidity_msb = 0xFD,
    humidity_lsb = 0xFE,
    data_beg = 0xF7,
};

namespace RegisterValues
{
enum : uint8_t
{
    id = 0x60
};
}

namespace ControlHumidity
{
enum : uint8_t
{
    oversampling_no = 0,
    oversampling_1 = 1,
    oversampling_2 = 2,
    oversampling_4 = 3,
    oversampling_8 = 4,
    oversampling_16 = 5
};
}

namespace ControlMeasurement
{
enum : uint8_t
{
    temperature_oversampling_no = 0b00000000,
    temperature_oversampling_1 = 0b00100000,
    temperature_oversampling_2 = 0b01000000,
    temperature_oversampling_4 = 0b01100000,
    temperature_oversampling_8 = 0b10000000,
    temperature_oversampling_16 = 0b10100000,

    pressure_oversampling_no = 0b00000000,
    pressure_oversampling_1 = 0b00000100,
    pressure_oversampling_2 = 0b00001000,
    pressure_oversampling_4 = 0b00001100,
    pressure_oversampling_8 = 0b00010000,
    pressure_oversampling_16 = 0b00010100,

    sleep_mode = 0,
    forced_mode = 0b00000001,
    normal_mode = 0b00000011,
};
}

namespace Configuration
{
enum : uint8_t
{
    stanby_time_normal_mode_ms_0_5 = 0b00000000,
    stanby_time_normal_mode_ms_62_5 = 0b00100000,
    stanby_time_normal_mode_ms_125 = 0b01000000,
    stanby_time_normal_mode_ms_250 = 0b01100000,
    stanby_time_normal_mode_ms_500 = 0b10000000,
    stanby_time_normal_mode_ms_1000 = 0b10100000,
    stanby_time_normal_mode_ms_10 = 0b11000000,
    stanby_time_normal_mode_ms_20 = 0b11100000,

    filter_coefficient_no = 0b00000000,
    filter_coefficient_2 = 0b00000100,
    filter_coefficient_4 = 0b00001000,
    filter_coefficient_8 = 0b00001100,
    filter_coefficient_16 = 0b00010000,

    enable_3wire_spi = 1
};
} // namespace configuration

namespace Status
{
enum : uint8_t
{
    measuring = 0b00001000,
    im_update = 0b00000001
};
}

union CallibrationDataSecondPart
{
    struct __attribute__((packed))
    {
        int16_t dig_H2;
        uint8_t dig_H3;
        uint8_t dig_H4_msb;
        uint8_t dig_H4_lsb_H5_lsb;
        uint8_t dig_H5_msb;
        uint8_t dig_H6;
    };
    char mapped_region[7];
};

} // namespace BME280

} // namespace jungles

#endif // __BME280_REGISTERS_HPP__
