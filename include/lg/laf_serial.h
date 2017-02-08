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
* @file laf_serial.h
* @class LafSerial
* @package openpst/libopenpst
* @brief LAF serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/
#pragma once

#include "definitions.h"
#include "serial/generic_serial.h"
#include "qualcomm/hdlc_encoder.h"
#include "lg/laf.h"
#include "util/hexdump.h"

using OpenPST::Serial::GenericSerial;
using OpenPST::QC::HdlcEncoder;

namespace OpenPST {
	namespace LG {
		class LafSerial : public GenericSerial {
			protected:
				HdlcEncoder encoder;
			public:
				/**
				* @brief LafSerial
				*
				* @param std::string port
				* @param int baudrate
				* @param serial::Timeout - Timeout, defaults to 1000ms
				*/
				LafSerial(std::string port, int baudrate, int timeout = 1000);

				/**
				* @brief ~LafSerial
				*/
				~LafSerial();

				/**
				* @brief enterLaf
				*/
				void enterLaf();

				/**
				* @brief sendHello
				*/
				void sendHello();

				/**
				* @brief sendReset
				*/
				void sendReset();

				/**
				* @brief sendPowerOff
				*/
				void sendPowerOff();

				/**
				* @brief sendCommand
				*
				* @return response - Command Response String
				*/
				std::string sendCommand(std::string command);

				/**
				* @brief getProperty
				*/
				std::string getProperty(LafProperties property);

			private:

	};
}
