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
* @file generic_serial.h
* @class GenericSerial
* @package libopenpst
* @brief A generic serial interface
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include "definitions.h"
#include "serial/serial.h"
#include "util/hexdump.h"
#include <iostream>
#include <fstream>

/**
* GenericSerial
*/
namespace OpenPST {
	namespace Serial {


		class GenericSerial : public serial::Serial {
			public:
				/**
				* @brief GenericSerial
				*
				* @param std::string port
				* @param int baudrate
				* @param serial::Timeout - Timeout, defaults to 1000ms
				*/
				GenericSerial(std::string port, int baudrate = 115200, int timeout = 1000);

				/**
				* @brief ~GenericSerial
				*/
				~GenericSerial();

				void open();

				void close();

				size_t available();
				
				/**
				* @brief write
				* @overload Serial:: write(uint8_t *data, size_t size)
				* @return size_t
				*/
				size_t write(uint8_t *data, size_t size);

				/**
				* @brief read
				* @overload Serial::read(uint8_t *buf, size_t size)
				* @return size_t
				*/
				size_t read(uint8_t *buf, size_t size);

				/**
				* @brief write
				* @overload Serial::write(std::vector<uint8_t> &data)
				* @return size_t
				*/
				size_t write(std::vector<uint8_t> &data);

				/**
				* @brief read
				* @overload Serial::read(std::vector<uint8_t> &buffer, size_t size)
				* @return size_t
				*/
				size_t read(std::vector<uint8_t> &buffer, size_t size);
		};


		/**
		* @brief SerialError - Generic Serial exception
		* @super std::exception
		*/
		class SerialError : public std::exception
		{
			const SerialError& operator=(SerialError);
			std::string _what;
			uint8_t code;
			public:
				SerialError(std::string message, uint8_t code = 0) : _what(message), code(code)  { }
				SerialError(const SerialError& second) : _what(second._what), code(second.code) {}
				virtual ~SerialError() throw() {}
				virtual const char* what() const throw () {
					return _what.c_str();
				}

				uint32_t getCode()
				{
					return code;
				}

		};
	}
}
