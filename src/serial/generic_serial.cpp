
/**
* LICENSE PLACEHOLDER
*
* @file sahara_serial.cpp
* @class SaharaSerial
* @package OpenPST
* @brief Sahara protocol serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "serial/generic_serial.h"

using namespace OpenPST::Serial;

/**
* @brief GenericSerial
*
* @param std::string port
* @param int baudrate
* @param serial::Timeout - Timeout, defaults to 1000ms
*/
GenericSerial::GenericSerial(std::string port, int baudrate, serial::Timeout timeout) :
    serial::Serial(port, baudrate, timeout)
{
}

/**
* @brief ~GenericSerial
*/
GenericSerial::~GenericSerial()
{
}

/**
* @brief write
* @overload Serial:: write(uint8_t *data, size_t size)
* @return size_t
*/
size_t GenericSerial::write(uint8_t *data, size_t size)
{
    size_t bytesWritten = Serial::write(data, size);
    hexdump_tx(data, bytesWritten);
    return bytesWritten;
}

/**
* @brief read
* @overload Serial::read(uint8_t *buf, size_t size)
* @return size_t
*/
size_t GenericSerial::read(uint8_t *buf, size_t size)
{
    size_t bytesRead = Serial::read(buf, size);
    hexdump_rx(buf, bytesRead);
    return bytesRead;
}

/**
* @brief write
* @overload Serial::write(std::vector<uint8_t> &data)
* @return size_t
*/
size_t GenericSerial::write(std::vector<uint8_t> &data)
{
    size_t bytesWritten = Serial::write(data);
    hexdump_tx(&data[0], bytesWritten);
    return bytesWritten;
}

/**
* @brief read
* @overload Serial::read(std::vector<uint8_t> &buffer, size_t size)
* @return size_t
*/
size_t GenericSerial::read(std::vector<uint8_t> &buffer, size_t size)
{
    size_t bytesRead = Serial::read(buffer, size);
    hexdump_rx(&buffer[0], bytesRead);
    return bytesRead;
}
