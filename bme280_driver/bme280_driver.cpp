/**
 * @file	BME280Driver.cpp
 * @brief	Implements the BME280 driver.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "bme280_driver.hpp"
#include "bme280_registers.hpp"
#include "jungles_os_helpers/generic_implementations/poller.hpp"

#include <chrono>

namespace jungles
{

template<typename E>
constexpr auto to_u_type(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

template<unsigned PollInterval, unsigned Timeout>
static auto make_poller(BME280Driver::MillisecondDelayer& millisecond_delayer)
{
    return jungles::generic::poller<PollInterval, Timeout>{[&](unsigned delay_ms) {
        millisecond_delayer(std::chrono::milliseconds{delay_ms});
    }};
}

BME280Driver::BME280Driver(I2CMaster& i2c, MillisecondDelayer millisecond_delayer) :
    bme280_i2c_io{I2CMaster::IO(i2c, BME280::address)},
    millisecond_delayer{std::move(millisecond_delayer)},
    callibration_data{get_callibration_data()}
{
    configure_sensor();
}

BME280Measurement BME280Driver::read()
{
    start_one_shot_measurement();
    wait_measurement_finished();
    auto raw_data{get_raw_data()};
    auto result{BME280::to_real_values(callibration_data, raw_data)};
    return {result.temperature, result.pressure, result.humidity};
}

BME280::CallibrationData BME280Driver::get_callibration_data()
{
    wait_device_accessible();

    BME280::CallibrationData callib_data{};
    read_from_device(to_u_type(BME280::RegisterAddress::callibration_first_part_beg),
                     reinterpret_cast<uint8_t*>(callib_data.mapped_region),
                     sizeof(callib_data.mapped_region));

    BME280::CallibrationDataSecondPart second_part{};
    read_from_device(to_u_type(BME280::RegisterAddress::callibration_second_part_beg),
                     reinterpret_cast<uint8_t*>(second_part.mapped_region),
                     sizeof(second_part.mapped_region));

    callib_data.dig_H2 = second_part.dig_H2;
    callib_data.dig_H3 = second_part.dig_H3;
    // Because of that we couldn't map this memory region to BME280_callibration_data union.
    callib_data.dig_H4 = second_part.dig_H4_msb << 4 | (second_part.dig_H4_lsb_H5_lsb & 0x0F);
    callib_data.dig_H5 = second_part.dig_H5_msb << 4 | (second_part.dig_H4_lsb_H5_lsb >> 4);
    callib_data.dig_H6 = second_part.dig_H6;

    return callib_data;
}

void BME280Driver::configure_sensor()
{
    bme280_i2c_io.write_byte(to_u_type(BME280::RegisterAddress::control_humidity),
                             BME280::ControlHumidity::oversampling_8);

    bme280_i2c_io.write_byte(to_u_type(BME280::RegisterAddress::control_measurement),
                             BME280::ControlMeasurement::temperature_oversampling_8
                                 | BME280::ControlMeasurement::pressure_oversampling_8);
}

void BME280Driver::wait_device_accessible()
{
    auto poller{make_poller<10, 1000>(millisecond_delayer)};

    auto is_no_timeout{poller.poll([&]() {
        auto is_device_id_matching{[&](auto device_id) {
            return device_id == BME280::RegisterValues::id;
        }};

        auto device_id{bme280_i2c_io.read_byte(to_u_type(BME280::RegisterAddress::id))};
        return is_device_id_matching(device_id);
    })};

    if (auto is_timeout{!is_no_timeout}; is_timeout)
        throw Error{"BME280 inaccessible"};
}

void BME280Driver::start_one_shot_measurement()
{
    auto control_measurement_register_address{to_u_type(BME280::RegisterAddress::control_measurement)};
    auto current_config{bme280_i2c_io.read_byte(control_measurement_register_address)};
    auto new_config_with_only_measurement_control_updated_bit{current_config | BME280::ControlMeasurement::forced_mode};
    bme280_i2c_io.write_byte(control_measurement_register_address,
                             new_config_with_only_measurement_control_updated_bit);
}

void BME280Driver::wait_measurement_finished()
{
    auto poller{make_poller<1, 100>(millisecond_delayer)};

    auto is_no_timeout{poller.poll([&]() {
        auto is_measuring_bit_cleared{[](auto status_register_content) {
            return (status_register_content & BME280::Status::measuring) == 0;
        }};

        auto status_register_content{bme280_i2c_io.read_byte(to_u_type(BME280::RegisterAddress::status))};
        return is_measuring_bit_cleared(status_register_content);
    })};

    if (auto is_timeout{!is_no_timeout}; is_timeout)
        throw Error{"Error waiting for BME280 measurement finished"};
}

BME280::RawData BME280Driver::get_raw_data()
{
    BME280::RawData result{};
    auto raw_data_size{sizeof(result.mapped_region)};
    auto raw_data{bme280_i2c_io.read(to_u_type(BME280::RegisterAddress::data_beg), raw_data_size)};
    std::copy(std::begin(raw_data), std::end(raw_data), result.mapped_region);
    return result;
}

void BME280Driver::read_from_device(unsigned char register_address, unsigned char* data, unsigned length)
{
    auto bytes{bme280_i2c_io.read(register_address, length)};
    std::copy(std::begin(bytes), std::end(bytes), data);
}

} // namespace jungles
