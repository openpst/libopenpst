/**
* LICENSE PLACEHOLDER
*
* @file sahara_serial.h
* @class SaharaSerial
* @package openpst/libopenpst
* @brief Sahara protocol serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include "definitions.h"
#include "serial/generic_serial.h"
#include "qualcomm/sahara.h"
#include "qualcomm/mbn.h"
#include "util/hexdump.h"
#include "util/sleep.h"
#include <iostream>
#include <fstream>

/**
* SaharaSerial
*
* Keep in mind when working with this library that sahara protocol dictates actions for
* most things. So when a request is mentioned its actually a request from the device not the
* host. So replies are actually responses. It is opposite from other QC protocols like QCDM, or 
* Streaming DLOAD.
* 
* Guest - You
* Host - Phone
* A general session works like this:
*  Guest -> Connect
*  Host -> Guest | Command: Hello - Specifying which mode it is in, and its supported versions/buffer size 
*  Guest -> Host | Command: Hello - Respond with the same mode or a new mode, and tell your versions/buffer size
*  Host -> ACK or NCK | Branch depending on mode:
*				SaharaMemoryDebugRequest - Memory Debug
*				SaharaCommandReadyResponse - Send Command
*				SaharaReadDataRequest - Send Image
*				SaharaEndImageTransferResponse - Error
*/
namespace OpenPST {
	namespace QC {

		struct SaharaState {
			uint32_t version;
			uint32_t minVersion;
			uint32_t mode; // see enum SaharaMode
			union {
				SaharaMemoryDebugRequest		memoryDebug;
				SaharaCommandReadyResponse		clientCommand;
				SaharaReadDataRequest			imageTransfer;
				SaharaEndImageTransferResponse  errorOrDone;
			};
		};

		class SaharaSerial : public OpenPST::Serial::GenericSerial {

			public:          
				/**
				* @brief SaharaSerial
				*
				* @param std::string port
				* @param int baudrate
				* @param int timeout in ms
				*/
				SaharaSerial(std::string port, int baudrate = 115200, int timeout = 1000);
			
				/**
				* @brief ~SaharaSerial
				*/
				~SaharaSerial();

				/**
				* @brief readHello - Always start a session by reading hello
				* @return SaharaHelloRequest
				*/
				SaharaHelloRequest readHello();

				/**
				 * @brief sendHello
				 *
				 * @param uint32_t mode - @see enum SaharaMode
				 * @param uint32_t version -The version of sahara protocol to request, defaults to 2
				 * @param uint32_t minVersion - The minimum version we can support, defaults to 1
				 * @param uint32_t status - indicate to device our status, if set to anything other than 0x00, sahara protocol will abort
				 *
				 * @return SaharaState
				 */
				SaharaState sendHello(uint32_t mode, uint32_t version = 0x02, uint32_t minVersion = 0x01, uint32_t status = 0x00);

				/**
				 * @brief switchMode -  Switch the device mode out of command mode. You will manually
				 *						have to re-establish the hello handshake, and gives the option
				 *						to change modes yet again if needed. Alternately, you can use 
				 *						SaharaSerial::switchModeAndHello to automatically do this for you.
				 * @param uint32_t mode - @see enum SaharaMode
				 * @return int
				 */ 
				void switchMode(uint32_t mode);

				/**
				* @brief switchModeAndHello - Switch the device mode out of command mode and automatically
				*								establish the hello handshake.
				* @param uint32_t mode - @see enum SaharaMode
				* @return int
				*/
				SaharaState switchModeAndHello(uint32_t mode);

				/**
				 * @brief sendClientCommand
				 *
				 * @param uint32_t command - @see enum SaharaClientCommand
				 *
				 * @return std::vector<uint8_t> - The data returned by the command
				 */
				std::vector<uint8_t> sendClientCommand(uint32_t command);

				/**
				 * @brief sendImage - Send a whole image file to the device. 
				 *
				 * @param std::string filePath
				 * @param SaharaReadDataRequest initialReadRequest - The initial read request data to process the request
				 *
				 * @return size_t - The total amount of bytes sent
				 */
				size_t sendImage(std::string filePath, SaharaReadDataRequest initialReadRequest);
				
				/**
				* @brief sendImage - Send a specific offset and size of an image to the device
				*
				* @param std::ifstream& file - The ifstream of the image file you previously opened
				* @param uint32_t offset - The offset in the file to send from
				* @param size_t size - The amount to send
				*
				* @return SaharaReadDataRequest - The next offset and size to send 
				*/
				SaharaReadDataRequest sendImage(std::ifstream& file, uint32_t offset, size_t size);
				
				/**
				* @brief readNextImageChunkSize
				*
				* @return SaharaReadDataRequest
				*/
				SaharaReadDataRequest readNextImageOffset();

				/**
				* @brief readMemory - Read size starting from address and 
				*                     store it in a vector buffer
				*
				* @param uint32_t address - The starting address to read from
				* @param size_t size - If the size is over SAHARA_MAX_MEMORY_REQUEST_SIZE, it will read in chunks of SAHARA_MAX_MEMORY_REQUEST_SIZE
				* @param std::vector<uint8_t>&out - The vector buffer to hold the data
				*
				* @return size_t - The total amount of bytes read
				*/
				size_t readMemory(uint32_t address, size_t size, std::vector<uint8_t>&out);
			
				/**
				* @brief readMemory - Read size starting from address and
				*                     save the result into the specified outFilePath
				*
				* @param uint32_t address - The starting address to read from
				* @param size_t size - If the size is over SAHARA_MAX_MEMORY_REQUEST_SIZE, it will read in chunks of SAHARA_MAX_MEMORY_REQUEST_SIZE
				* @param const char* outFilePath - Path to the file to create and store the read data
				* @param size_t outSize - Total size of the read data
				* @return int
				*/
				size_t readMemory(uint32_t address, size_t size, std::string outFilePath);
			
				/**
				* @brief readMemory - Read size starting from address and
				*                     save the result into an existing file pointer.
				*               
				* @note - Will not close the file pointer handle
				*
				* @param uint32_t address
				* @param size_t size - If the size is over SAHARA_MAX_MEMORY_REQUEST_SIZE, it will read in chunks of SAHARA_MAX_MEMORY_REQUEST_SIZE
				* @param std::ofstream out - out file stream to write to
				* 
				* @return size_t outSize
				*/
				size_t readMemory(uint32_t address, size_t size, std::ofstream& out);


				/**
				* @brief sendDone - Sends the done command. In emergency mode this will
				*                    execute and transition to the uploaded programmer.
				* @return void
				*/
				void sendDone();
			
				/**
				 * @brief sendReset - Reset the session. In emergency mode this will 
				 *						make the device spin a blocking infiniate loop 			
				 *
				 * @return void
				 */
				void sendReset();

				/**
				 * @brief close
				 * @overload Serial::close
				 * @return void
				 */
				void close();

				/**
				 * @brief getNamedMode
				 * @param mode
				 * @return const char*
				 */
				std::string getNamedMode(uint32_t mode);

				/**
				 * @brief getNamedClientCommand
				 * @param command
				 * @return const char*
				 */
				std::string getNamedClientCommand(uint32_t command);

				/**
				 * @brief getNamedErrorStatus
				 *
				 * @param uint32_t status
				 * @return std::string
				 */
				std::string getNamedErrorStatus(uint32_t status);

				/**
				 * @brief getNamedRequestedImage
				 *
				 * @param uint32_t imageId
				 *
				 * @return std::string
				 */

				std::string getNamedRequestedImage(uint32_t imageId);

				/**
				* @brief validateResponse - Validate a response
				*
				* @param uint32_t expectedResponseCommand
				* @param SaharaHeader* data
				* @param size_t dataSize
				*
				* @throws SaharaSerialError
				*
				* @return void
				*/
				void validateResponse(uint32_t expectedResponseCommand, SaharaHeader* data, size_t dataSize);

				/**
				* @brief isValidResponse - Check a response is the expected response by command
				*
				* @param uint32_t expectedResponseCommand
				* @param SaharaHeader* data
				* @param size_t dataSize
				*
				* @return bool
				*/
				bool isValidResponse(uint32_t expectedResponseCommand, SaharaHeader* data, size_t dataSize);
				
				/**
				* @brief isErrorResponse - Check if a response is an error response
				*
				* @param SaharaHeader* data
				* @param size_t dataSize
				*
				* @return bool
				*/
				bool isErrorResponse(SaharaHeader* data, size_t dataSize);
		};



		/**
		* @brief SaharaSerialError - Generic Sahara Serial exception
		* @super std::exception
		*/
		class SaharaSerialError : public std::exception
		{
			const SaharaSerialError& operator=(SaharaSerialError);
			std::string _what;
			uint8_t code;
		public:
			SaharaSerialError(std::string message, uint8_t code = 0) : _what(message), code(code)  { }
			SaharaSerialError(const SaharaSerialError& second) : _what(second._what), code(second.code) {}
			virtual ~SaharaSerialError() throw() {}
			virtual const char* what() const throw () {
				return _what.c_str();
			}

			uint32_t getCode()
			{
				return code;
			}

		};

		/**
		* @brief SaharaSerialInvalidArgument - Exception thrown when a method has an invalid argument
		* @super SaharaSerialError, std::invalid_argument
		*/
		class SaharaSerialInvalidArgument : public SaharaSerialError, std::invalid_argument
		{
			const SaharaSerialInvalidArgument& operator=(SaharaSerialInvalidArgument);
			std::string _what;
		public:
			SaharaSerialInvalidArgument(std::string message) : SaharaSerialError(message), invalid_argument(message), _what(message)  {}
			SaharaSerialInvalidArgument(const SaharaSerialInvalidArgument& second) : SaharaSerialError(second), invalid_argument(second), _what(second._what) {}
			virtual ~SaharaSerialInvalidArgument() throw() {}
			virtual const char* what() const throw () {
				return _what.c_str();
			}
		};
	}
}
