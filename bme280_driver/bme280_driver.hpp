/**
 * @file bme280.hpp
 * @author Kacper Kowalski (kacper.s.kowalski@gmail.com)
 * @brief Implements driver which drives BME280 sensor to obtain core weather conditions
 *        (temperature, pressure, humidity).
 * @date 2020-07-31
 */
#ifndef __BME280__HPP__
#define __BME280__HPP__

#include "i2c_master.hpp"

#include "bme280_conversion.hpp"
#include "bme280_measurement.hpp"
#include "bme280_registers.hpp"

#include <chrono>
#include <exception>
#include <functional>

namespace jungles
{

class BME280Driver
{
  public:
    using MillisecondDelayer = std::function<void(std::chrono::milliseconds)>;

    explicit BME280Driver(I2CMaster&, MillisecondDelayer);

    BME280Measurement read();

    struct Error : std::exception
    {
        Error(const char* message) : message{message}
        {
        }

        virtual const char* what() const noexcept override
        {
            return message;
        }

        const char* message;
    };

  private:
    BME280::CallibrationData get_callibration_data();
    void configure_sensor();
    void wait_device_accessible();
    void start_one_shot_measurement();
    void wait_measurement_finished();
    BME280::RawData get_raw_data();
    void read_from_device(unsigned char register_address, unsigned char* data, unsigned length);

    I2CMaster::IO bme280_i2c_io;
    MillisecondDelayer millisecond_delayer;
    BME280::CallibrationData callibration_data;
};

} // namespace jungles

#endif // __BME280__HPP__
