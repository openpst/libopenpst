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
* @file hdlc_serial.cpp
* @class HdlcSerial
* @package openpst/libopenpst
* @brief HDLC serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "qualcomm/hdlc_serial.h"

using namespace OpenPST::QC;

/**
* @brief HdlcSerial::HdlcSerial
*
* @param std::string port
* @param int baudrate
* @param serial::Timeout - Timeout, defaults to 1000ms
*/
HdlcSerial::HdlcSerial(std::string port, int baudrate, int timeout) :
	GenericSerial(port, baudrate, timeout)
{

}

/**
* @brief HdlcSerial::~HdlcSerial
*/
HdlcSerial::~HdlcSerial()
{

}

/**
* @brief HdlcSerial::write - Escapes the data and creates a CRC'ed HDLC packet
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
size_t HdlcSerial::write (uint8_t *data, size_t size, bool encapsulate)
{
	if (!encapsulate) {
		return GenericSerial::write(data, size);
	}

	uint8_t* encoded   = nullptr;
	size_t encodedSize = encoder.encode(data, size, &encoded);
	size_t txSize = 0;

	try {
		txSize = GenericSerial::write(encoded, encodedSize);
	} catch(...) {
		if (encoded != nullptr) {
			delete[] encoded;
		}
		throw;
	}

	if (encoded != nullptr) {
		delete[] encoded;
	}

	return txSize;    
}

/**
* @brief HdlcSerial::read - Reads and unescpaes theCRC'ed HDLC packet
* read from the device
*
* @super GenericSerial::read (uint8_t *buffer, size_t size);

* @param uint8_t* data
* @param size_t size
* @param bool unescape
*
* @return size_t bytes read
*/
size_t HdlcSerial::read (uint8_t* data, size_t size, bool unescape )
{
	auto line = GenericSerial::readline(65536, { HDLC_CONTROL_CHAR });
	line.push_back(HDLC_CONTROL_CHAR);
	size_t rxSize = line.size();
	memcpy(data, line.data(), std::min(line.size(), size));

	if (!unescape || !rxSize) {
		return rxSize;
	}

	uint8_t* decoded = nullptr;

	size_t decodedSize = encoder.decode(data, rxSize, &decoded);

	if (decodedSize > size) {
		delete[] decoded;
		throw SerialError("HDLC decoded response is larger than provided read buffer");
	}

	memcpy(data, decoded, decodedSize);

	if (decoded != nullptr) {
		delete[] decoded;
	}

	return decodedSize;
}

/**
* @brief HdlcSerial::write - Escapes the data and creates a CRC'ed HDLC packet
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
size_t HdlcSerial::write(std::vector<uint8_t> &data, bool encapsulate)
{
	if (!encapsulate) {
		return GenericSerial::write(data);
	}

	encoder.encode(data);

	return GenericSerial::write(data);
}

/**
* @brief HdlcSerial::read - Reads and unescpaes theCRC'ed HDLC packet
* read from the device
*
* @super GenericSerial::read (std::vector<uint8_t> &buffer, size_t size);

* @param std::vector<uint8_t>& buffer
* @param size_t size
* @param bool unescape
*
* @return size_t bytes read
*/
size_t HdlcSerial::read(std::vector<uint8_t> &buffer, size_t size, bool unescape)
{
	auto line = GenericSerial::readline(65536, { HDLC_CONTROL_CHAR });
	line.push_back(HDLC_CONTROL_CHAR);
	size_t rxSize = line.size();
	buffer.resize(line.size());
	memcpy(buffer.data(), line.data(), std::min(line.size(), buffer.size()));

	if (!unescape || !rxSize) {
		return rxSize;
	}

	encoder.decode(buffer);

	return buffer.size();
}
