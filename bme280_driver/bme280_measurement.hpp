/**
 * @file	bme280_measurement.hpp
 * @brief	Defines measurement structure.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BME280_MEASUREMENT_HPP
#define BME280_MEASUREMENT_HPP

namespace jungles
{

struct BME280Measurement
{
    float temperature;
    float pressure;
    float humidity;
};

} // namespace jungles

#endif /* BME280_MEASUREMENT_HPP */
