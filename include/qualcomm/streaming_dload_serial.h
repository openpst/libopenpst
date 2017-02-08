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
* @file streaming_dload_serial.h
* @class StreamingDloadSerial
* @package openpst/libopenpst
* @brief Streaming DLOAD protocol serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include <iostream>
#include <fstream>
#include "definitions.h"
#include "qualcomm/hdlc_serial.h"
#include "util/hexdump.h"
#include "qualcomm/streaming_dload.h"
#include "qualcomm/hdlc_encoder.h"

namespace OpenPST {
	namespace QC {
		
		struct StreamingDloadDeviceState {
			bool negotiated = false;
			uint8_t openMode = 0x00;
			uint8_t openMultiMode = 0x00;
			StreamingDloadHelloResponse hello = {};
			StreamingDloadErrorResponse lastError = {};
			StreamingDloadLogResponse   lastLog = {};
		};

		enum StreamingDloadOperationResult {
			kStreamingDloadIOError = -1,
			kStreamingDloadError = 0,
			kStreamingDloadSuccess = 1
		};

		class StreamingDloadSerial : public HdlcSerial {

		public:
			/**
			* @brief holds the current device state data
			*/
			StreamingDloadDeviceState state;

			/**
			* @brief StreamingDloadSerial()
			*
			* @param std::string port - The device to connect to
			* @param int baudrate - Defaults to 115200
			* @param int timeout in ms
			*/
			StreamingDloadSerial(std::string port, int baudrate = 115200, int timeout = 1000);

			/**
			* @brief ~StreamingDloadSerial()
			*/
			~StreamingDloadSerial();

			/**
			* @brief sendHello - sends the initial handshake for the session
			*
			* @param std::string magic - The magic handhsake word. Should be QCOM FAST DOWNLOAD HOST
			* @param uint8_t version - The max version to be compatible with
			* @param uint8_t compatibleVersion - The lowest version to be compatible with
			* @param uint8_t a set of feature bits of what features to enable.
			*   @see qc/streaming_dload.h
			*
			* @return int
			*/
			StreamingDloadHelloResponse sendHello(std::string magic, uint8_t version, uint8_t compatibleVersion, uint8_t featureBits);

			/**
			* @brief sendUnlock - send the unlock command for implementations that require it
			*
			* @param std::string code - The unlock code
			*
			* @return bool
			*/
			bool sendUnlock(std::string code);

			/**
			* @brief setSecurityMode - set the session security mode
			*
			* @param uint8_t mode - The security mode to set to
			*
			* @return int
			*/
			bool setSecurityMode(uint8_t mode);

			/**
			* @brief sendNop - send a NOP
			*
			* @return int
			*/
			void sendNop();

			/**
			* @brief sendReset - send a reset command
			*
			* @return int
			*/
			void sendReset();

			/**
			* @brief sendNop - send a power down command
			*
			* @return int
			*/
			void sendPowerOff();

			/**
			* @brief readEcc - read the current ECC state
			*
			* @param uint8_t& status - Will be set with the current status
			* @return int
			*/
			uint8_t readEcc();

			/**
			* @brief setEcc - set the current ECC state
			*
			* @param uint8_t status - The status to set it to
			*
			* @return int
			*/
			bool setEcc(uint8_t status);

			/**
			* @brief openMode - Open mode
			*
			* @param uint8_t mode - The mode to open
			*   @see qc/streaming_dload.h
			* @return int
			*/
			bool openMode(uint8_t mode);

			/**
			* @brief closeMode - Close the current opened mode
			*
			* @return int
			*/
			bool closeMode();

			/**
			* @brief openMultiImage - open mode for multi image devices
			*
			* @param uint8_t imageType - The image type to open
			*   @see qc/streaming_dload.h
			*
			* @return int
			*/
			bool openMultiImage(uint8_t imageType);


			/**
			* @brief readFlash - Read x bytes from starting address
			*                      into a std::vector<uint8_t> container
			*
			* @param uint32_t address - The starting address
			* @param size_t amount - The amount to read from address
			* @param std::vector<uint8_t> &out - The populated vector containing the read data until success or error encountered.
			*
			* @return int
			*/
			size_t readFlash(uint32_t address, size_t amount, std::vector<uint8_t> &out);

			/**
			* @brief readAddress - Read x bytes from starting address
			*                      into a file pointer
			*
			* @param uint32_t address - The starting address
			* @param size_t amount - The amount to read from address
			* @param std::ofstream& out - The file pointer to write the data to
			* @param size_t& outSize - The amount of bytes written to the file until success or error encountered.
			*
			* @return size_t
			*/
			size_t readFlash(uint32_t address, size_t amount, std::ofstream& out);

			/**
			* @brief writePartitionTable - Writes partition table for sessions that require it.
			*
			* You should send this request with overwrite set to false first and check the outStatus
			* If it is ok the write will take place. If it is not ok then to write the file you will
			* need to force it with overwrite set to true.
			*
			* @param std::string filePath - The path to the partition table (max 512 bytes)
			* @param bool overwritte - Defaults to false. If true will overwrite even if error is reported.
			*
			* @return uint8_t - The status reported back by the device.
			*/
			uint8_t writePartitionTable(std::string filePath, bool overwrite = false);

			/**
			* @brief writeFlash - Stream write data starting at specified address. Writes hdlc encoded chunks
			*                   of max block size specified by device. Try writeFlashUnframed first, its faster
			*
			* @param uint32_t address - The starting address to write to
			* @param uint8_t data - A pointer to the data to be written
			* @param size_t length - The amount of data to write.
			*
			* @return int
			*/
			size_t writeFlash(uint32_t address, uint8_t* data, size_t length);

			/**
			* @brief writeFlash - Stream write unframed data starting at specified address. 
			*
			* @param uint32_t address - The starting address to write to
			* @param uint8_t data - A pointer to the data to be written
			* @param size_t length - The amount of data to write.
			*
			* @return size_t
			*/
			size_t writeFlashUnframed(uint32_t address, uint8_t* data, size_t length);

			/**
			* @brief getNamedError - Get a named error from an error code
			*
			* @param uint8_t code - The error code
			*
			* @return std::string msg
			*/
			std::string getNamedError(uint8_t code);

			/**
			* @brief getNamedOpenMode - Get a named mode from an open mode integer
			*
			* @param uint8_t mode - The mode
			*
			* @return std::string mode
			*/
			std::string getNamedOpenMode(uint8_t mode);

			/**
			* @brief getNamedMultiImage - Get a named image from an open multi image type
			*
			* @param uint8_t imageType - The image type id
			*
			* @return std::string name
			*/
			std::string getNamedMultiImage(uint8_t imageType);

			/**
			* @overload GenericSerial::close()
			*/
			void close();

		private:
			/**
			* @brief isValidResponse
			*
			* @param uint8_t expectedCommand,
			* @param uint8_t* response
			* @param size_t responseSize
			*/
			bool isValidResponse(uint8_t expectedCommand, uint8_t* response, size_t responseSize);

			/**
			* @brief isValidResponse
			*
			* @param uint8_t expectedCommand,
			* @param std::vector<uint8_t> response
			*/
			bool isValidResponse(uint8_t expectedCommand, std::vector<uint8_t> &response);

			void validateResponse(uint8_t expectedCommand, uint8_t* response, size_t responseSize);

			void validateResponse(uint8_t expectedCommand, std::vector<uint8_t> &response);
		};


		/**
		* @brief StreamingDloadSerialError - Generic StreamingDload Serial exception
		* @super std::exception
		*/
		class StreamingDloadSerialError : public std::exception
		{
			const StreamingDloadSerialError& operator=(StreamingDloadSerialError);
			std::string _what;
			uint8_t code;
		public:
			StreamingDloadSerialError(std::string message, uint8_t code = 0) : _what(message), code(code)  { }
			StreamingDloadSerialError(const StreamingDloadSerialError& second) : _what(second._what), code(second.code) {}
			virtual ~StreamingDloadSerialError() throw() {}
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
