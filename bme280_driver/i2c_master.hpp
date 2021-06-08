/**
 * @file        I2CMaster.hpp
 * @brief       I2C master interface.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef I2C_MASTER_HPP
#define I2C_MASTER_HPP

#include <string_view>
#include <vector>

namespace jungles
{

struct I2CMaster
{
    using Bytes = std::vector<unsigned char>;

    virtual Bytes read(unsigned char device_address, unsigned char register_address, unsigned num_bytes) = 0;
    virtual unsigned char read_byte(unsigned char device_address, unsigned char register_address) = 0;
    virtual void write(unsigned char device_address, unsigned char register_address, std::string_view bytes) = 0;
    virtual void write_byte(unsigned char device_address, unsigned char register_address, unsigned char byte) = 0;

    struct IO
    {
        explicit IO(I2CMaster& i2c, unsigned char device_address) : i2c{i2c}, device_address{device_address}
        {
        }

        Bytes read(unsigned char register_address, unsigned num_bytes)
        {
            return i2c.read(device_address, register_address, num_bytes);
        }

        unsigned char read_byte(unsigned char register_address)
        {
            return i2c.read_byte(device_address, register_address);
        }

        void write(unsigned char register_address, std::string_view bytes)
        {
            i2c.write(device_address, register_address, bytes);
        }

        void write_byte(unsigned char register_address, unsigned char byte)
        {
            i2c.write_byte(device_address, register_address, byte);
        }

        I2CMaster& i2c;
        unsigned char device_address;
    };

    virtual ~I2CMaster() = default;
};

} // namespace jungles

#endif /* I2C_MASTER_HPP */
