/**
* LICENSE PLACEHOLDER
*
* @file hdlc_serial.h
* @class HdlcSerial
* @package OpenPST
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
				* @param serial::Timeout - Timeout, defaults to 1000ms
				*/
				HdlcSerial(std::string port, int baudrate, serial::Timeout timeout = serial::Timeout::simpleTimeout(1000));
			
				/**
				* @brief ~HdlcSerial            
				*/
				~HdlcSerial();

				/**
				 * @brief write - Escapes the data and creates a CRC'ed HDLC packet
				 * then writes the data
				 *
				 * @super Serial::write (uint8_t *data, size_t size);
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
				 * @super Serial::read (uint8_t *buffer, size_t size);

				 * @param uint8_t* buf
				 * @param size_t size
				 * @param bool unescape
				 *
				 * @return size_t bytes read
				 */
				size_t read(uint8_t *buf, size_t size, bool unescape = true);
			
				/**
				* @brief write - Escapes the data and creates a CRC'ed HDLC packet
				* then writes the data
				*
				* @super Serial::write (std::vector<uint8_t> &data);
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
				* @super Serial::read (std::vector<uint8_t> &buffer, size_t size);

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
