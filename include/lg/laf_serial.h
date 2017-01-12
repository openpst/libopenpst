/**
* LICENSE PLACEHOLDER
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
