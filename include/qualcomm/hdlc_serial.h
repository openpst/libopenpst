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
* @file hdlc_serial.h
* @class HdlcSerial
* @package openpst/libopenpst
* @brief HDLC serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include "definitions.h"
#include "serial/generic_serial.h"
#include "util/hexdump.h"
#include "qualcomm/hdlc_encoder.h"

using OpenPST::Serial::GenericSerial;
using OpenPST::Serial::SerialError;

namespace OpenPST {
	namespace QC {
		class HdlcSerial : public GenericSerial {
			protected:
				HdlcEncoder encoder;
			public:
				/**
				* @brief HdlcSerial
				*
				* @param std::string port
				* @param int baudrate
				* @param int timeout defaults to 1000ms
				*/
				HdlcSerial(std::string port, int baudrate, int timeout = 1000);
			
				/**
				* @brief ~HdlcSerial            
				*/
				~HdlcSerial();

				/**
				 * @brief write - Escapes the data and creates a CRC'ed HDLC packet
				 * then writes the data
				 *
				 * @super GenericSerial::write (uint8_t *data, size_t size);
				 *
				 * @param uint8_t* data
				 * @param size_t size
				 * @param bool encapsulate
				 *
				 * @return size_t - Bytes written
				 */
				size_t write(uint8_t *data, size_t size, bool encapsulate = true);

				/**
				 * @brief read - Reads and unescpaes theCRC'ed HDLC packet
				 * read from the device
				 *
				 * @super GenericSerial::read (uint8_t *data, size_t size);

				 * @param uint8_t* data
				 * @param size_t size
				 * @param bool unescape
				 *
				 * @return size_t bytes read
				 */
				size_t read(uint8_t *data, size_t size, bool unescape = true);
			
				/**
				* @brief write - Escapes the data and creates a CRC'ed HDLC packet
				* then writes the data
				*
				* @super GenericSerial::write (std::vector<uint8_t> &data);
				*
				* @param std::vector<uint8_t>& data
				* @param size_t size
				* @param bool encapsulate
				*
				* @return size_t - Bytes written
				*/
				size_t write(std::vector<uint8_t> &data, bool encapsulate = true);
			
				/**
				* @brief read - Reads and unescpaes theCRC'ed HDLC packet
				* read from the device
				*
				* @super GenericSerial::read (std::vector<uint8_t> &buffer, size_t size);

				* @param std::vector<uint8_t>& buffer
				* @param size_t size 
				* @param bool unescape
				*
				* @return size_t bytes read
				*/
				size_t read(std::vector<uint8_t> &buffer, size_t size, bool unescape = true);
		};
	}
}
