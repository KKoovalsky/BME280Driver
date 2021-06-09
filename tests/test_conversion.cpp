/**
 * @file        test_conversion.cpp
 * @brief       Tests whether the conversion of BME280 measurement works properly.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"

#include "bme280_driver.hpp"

#include <iostream>
#include <vector>

struct I2CMasterMock : jungles::I2CMaster
{
    virtual Bytes read(unsigned char, unsigned char register_address, unsigned) override
    {
        if (register_address == 0x88)
            return callibration_data_first_block;
        else if (register_address == 0xE1)
            return callibration_data_second_block;
        else if (register_address == 0xF7)
            return raw_measurement_data;
        return {};
    }

    virtual unsigned char read_byte(unsigned char, unsigned char register_address) override
    {
        if (register_address == 0xD0)
            return 0x60;
        return {};
    }

    virtual void write(unsigned char, unsigned char, std::string_view) override
    {
    }

    virtual void write_byte(unsigned char, unsigned char, unsigned char) override
    {
    }

    std::vector<unsigned char> callibration_data_first_block;
    std::vector<unsigned char> callibration_data_second_block;
    std::vector<unsigned char> raw_measurement_data;
};

TEST_CASE("BME280 measurements are converted", "[bme280]")
{

    SECTION("Positive test with room-like conditions")
    {
        I2CMasterMock i2c_master_mock;
        i2c_master_mock.callibration_data_first_block = {0x32, 0x70, 0xd0, 0x68, 0x32, 0x00, 0x3a, 0x8e, 0x1b,
                                                         0xd6, 0xd0, 0x0b, 0x15, 0x24, 0x64, 0xff, 0xf9, 0xff,
                                                         0x0c, 0x30, 0x20, 0xd1, 0x88, 0x13, 0x00, 0x4b};
        i2c_master_mock.callibration_data_second_block = {0x4c, 0x01, 0x00, 0x19, 0x20, 0x03, 0x1e};
        i2c_master_mock.raw_measurement_data = {0x4e, 0xba, 0xc0, 0x7f, 0xe3, 0x0, 0x8e, 0x1a};

        jungles::BME280Driver bme280_driver{i2c_master_mock, [](auto) {
                                            }};
        auto [temperature, pressure, humidity] = bme280_driver.read();
        CHECK(temperature == Catch::Approx(20.56).epsilon(0.01));
        CHECK(pressure == Catch::Approx(98456.1875).epsilon(0.02));
        CHECK(humidity == Catch::Approx(54.42).epsilon(0.01));

        // Expected callibration data values:
        // dig_T1 = 0x7032;
        // dig_T2 = 0x68d0;
        // dig_T3 = 0x32;
        // dig_P1 = 0x8e3a;
        // dig_P2 = 0xd61b;
        // dig_P3 = 0xbd0;
        // dig_P4 = 0x2415;
        // dig_P5 = 0xff64;
        // dig_P6 = 0xfff9;
        // dig_P7 = 0x300c;
        // dig_P8 = 0xd120;
        // dig_P9 = 0x1388;
        // dig_H1 = 0x4b;
        // dig_H2 = 0x14c;
        // dig_H3 = 0x0;
        // dig_H4 = 0x190;
        // dig_H5 = 0x32;
        // dig_H6 = 0x1e;
    }

    SECTION("Positive test with room-like conditions with data from Internet")
    {
        I2CMasterMock i2c_master_mock;
        i2c_master_mock.callibration_data_first_block = {0xe6, 0x6e, 0xcf, 0x66, 0x32, 0x00, 0xfb, 0x90, 0x57,
                                                         0xd5, 0xd0, 0x0b, 0xea, 0x1a, 0x7b, 0xff, 0xf9, 0xff,
                                                         0xac, 0x26, 0x0a, 0xd8, 0xbd, 0x10, 0x00, 0x4b};
        i2c_master_mock.callibration_data_second_block = {0x66, 0x01, 0x00, 0x14, 0x0a, 0x00, 0x1e};
        i2c_master_mock.raw_measurement_data = {0x51, 0xe9, 0x05, 0x7f, 0x92, 0x0b, 0x74, 0x15};

        jungles::BME280Driver bme280_driver{i2c_master_mock, [](auto) {
                                            }};
        auto [temperature, pressure, humidity] = bme280_driver.read();
        CHECK(temperature == Catch::Approx(21.43).epsilon(0.01));
        CHECK(pressure == Catch::Approx(100819.0).epsilon(0.02));
        CHECK(humidity == Catch::Approx(47.33).epsilon(0.01));

        // Expected callibration data values:
        // dig_T1 = 28390;  // 0x6ee6
        // dig_T2 = 26319;  // 0x66cf
        // dig_T3 = 50;     // 0x0032
        // dig_P1 = 37115;  // 0x90fb
        // dig_P2 = -10921; // 0xd557
        // dig_P3 = 3024;   // 0x0bd0
        // dig_P4 = 6890;   // 0x1aea
        // dig_P5 = -133;   // 0xff7b
        // dig_P6 = -7;     // 0xfff9
        // dig_P7 = 9900;   // 0x26ac
        // dig_P8 = -10230; // 0xd80a
        // dig_P9 = 4285;   // 0x10bd
        // dig_H1 = 75;  // 0x4b
        // dig_H2 = 358; // 0x166
        // dig_H3 = 0x0; // 0x00
        // dig_H4 = 330; // 0x14a
        // dig_H5 = 0;  // 0x00
        // dig_H6 = 30; // 0x1e
    }
}
