
/**
*
* (c) Gassan Idriss <ghassani@gmail.com>
* 
* This file is part of libopenpst.
* 
* libopenpst is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* libopenpst is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with libopenpst. If not, see <http://www.gnu.org/licenses/>.
*
* @file sahara_serial.cpp
* @class SaharaSerial
* @package openpst/libopenpst
* @brief Sahara protocol serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "serial/generic_serial.h"

using namespace OpenPST::Serial;

// use macros for try and catch so we only have to edit it in one place
// we try and catch all exceptions here thrown by the serial library used
// and re-throw our own exception to cut down our library try/catch statements needed
// when being implemented

#define GENERIC_SERIAL_TRY try {

#define GENERIC_SERIAL_CATCH 	\
	} catch (serial::IOException e) { \
		throw SerialError(e.what(), e.getErrorNumber()); \
	} catch(serial::SerialException& e) { \
		throw SerialError(e.what()); \
	} catch(std::invalid_argument& e) { \
		throw SerialError(e.what()); \
	} catch(serial::PortNotOpenedException& e) { \
		throw SerialError(e.what()); \
	} catch (...) { \
		throw SerialError("Unhandled Exception Encountered"); \
	} 

/**
* @brief GenericSerial
*
* @param std::string port
* @param int baudrate
* @param serial::Timeout - Timeout, defaults to 1000ms
*/
GenericSerial::GenericSerial(std::string port, int baudrate, int timeout) :
	serial::Serial(port, baudrate, serial::Timeout::simpleTimeout(timeout))
{
}

/**
* @brief ~GenericSerial
*/
GenericSerial::~GenericSerial()
{
}

void GenericSerial::open()
{
	GENERIC_SERIAL_TRY
		return Serial::open();
	GENERIC_SERIAL_CATCH
}

void GenericSerial::close()
{
	GENERIC_SERIAL_TRY
		return Serial::close();
	GENERIC_SERIAL_CATCH
}

size_t GenericSerial::available()
{
	GENERIC_SERIAL_TRY
		return Serial::available();
	GENERIC_SERIAL_CATCH
}

	
/**
* @brief write
* @overload Serial:: write(uint8_t *data, size_t size)
* @return size_t
*/
size_t GenericSerial::write(uint8_t *data, size_t size)
{
	
	GENERIC_SERIAL_TRY
		size_t bytesWritten = Serial::write(data, size);
		hexdump_tx(data, bytesWritten);
		return bytesWritten;
	GENERIC_SERIAL_CATCH
}

/**
* @brief read
* @overload Serial::read(uint8_t *buf, size_t size)
* @return size_t
*/
size_t GenericSerial::read(uint8_t *buf, size_t size)
{
	GENERIC_SERIAL_TRY
		size_t bytesRead = Serial::read(buf, size);
		hexdump_rx(buf, bytesRead);
		return bytesRead;
	GENERIC_SERIAL_CATCH
}

/**
* @brief write
* @overload Serial::write(std::vector<uint8_t> &data)
* @return size_t
*/
size_t GenericSerial::write(std::vector<uint8_t> &data)
{
	GENERIC_SERIAL_TRY
		size_t bytesWritten = Serial::write(data);
		hexdump_tx(&data[0], bytesWritten);
		return bytesWritten;
	GENERIC_SERIAL_CATCH
}

/**
* @brief read
* @overload Serial::read(std::vector<uint8_t> &buffer, size_t size)
* @return size_t
*/
size_t GenericSerial::read(std::vector<uint8_t> &buffer, size_t size)
{
	GENERIC_SERIAL_TRY
		size_t bytesRead = Serial::read(buffer, size);
		hexdump_rx(&buffer[0], bytesRead);
		return bytesRead;
	GENERIC_SERIAL_CATCH
}
