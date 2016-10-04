/**
* LICENSE PLACEHOLDER
*
* @file laf_serial.cpp
* @class LafSerial
* @package OpenPST
* @brief HDLC serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/

#include "lg/laf_serial.h"
#include "qualcomm/hdlc.h"

#define BUFFER_SIZE (LAF_MAX_DATA_SIZE + sizeof(LafCmdHeader))

using namespace OpenPST::LG;

uint8_t enable_laf[] = { 0x3A, 0xA1, 0x6E, 0x7E };
uint8_t buffer[BUFFER_SIZE] = {};

/**
* @brief LafSerial::LafSerial
*
* @param std::string port
* @param int baudrate
* @param serial::Timeout - Timeout, defaults to 1000ms
*/
LafSerial::LafSerial(std::string port, int baudrate, serial::Timeout timeout) :
    GenericSerial(port, baudrate, timeout)
{

}

/**
* @brief LafSerial::~LafSerial
*/
LafSerial::~LafSerial()
{

}

/**
* @brief LafSerial::enterLaf
*/
void LafSerial::enterLaf()
{
	write(enable_laf, sizeof(enable_laf));
}

/**
* @brief LafSerial::sendHello
*/
void LafSerial::sendHello()
{
	LafCmdHeader header = {};
	header.command = LAF_CMD_HELO;
	header.arg0 = 0x01000001;

	uint16_t crc = encoder.crc16((char*)&header, (sizeof(header)));
	header.crc = crc;

	write((uint8_t*)&header, sizeof(header));
}

/**
* @brief LafSerial::sendReset
*/
void LafSerial::sendReset()
{
	LafCmdHeader header = {};
	header.command = LAF_CMD_CONTROL;
	header.arg0 = LAF_CMD_CONTROL_RESET;
	header.magic = LAF_CTRL_MAGIC;

	uint16_t crc = encoder.crc16((char*)&header, (sizeof(header)));
	header.crc = crc;

	write((uint8_t*)&header, sizeof(header));
}

/**
* @brief LafSerial::sendPowerOff
*/
void LafSerial::sendPowerOff()
{
	LafCmdHeader header = {};
	header.command = LAF_CMD_CONTROL;
	header.arg0 = LAF_CMD_CONTROL_POWER_OFF;
	header.magic = LAF_CTRL_MAGIC;

	uint16_t crc = encoder.crc16((char*)&header, (sizeof(header)));
	header.crc = crc;

	write((uint8_t*)&header, sizeof(header));
}

/**
* @brief LafSerial::sendCommand(std::string command)
*/
std::string LafSerial::sendCommand(std::string command) {
	if (command.compare("SPECIAL") == 0) {	

		return "SPECIAL";
	}

	std::stringstream lafCmdResponse;

	size_t responseSize;

	LafCmdHeader header = {};
	header.command = LAF_CMD_EXECUTE;
	header.magic = LAF_EXEC_MAGIC;

	LafCommand* packet = (LafCommand*)buffer;
	packet->header = header;

	if (command.size()) {
		memcpy(packet->data, command.c_str(), command.size());
		memset(packet->data + command.size(), 0x00, 1);
		packet->header.size = command.size() + 1;
		packet->header.crc = 0x000000;

		uint16_t crc = encoder.crc16((char*)packet, (sizeof(header) + command.size() + 1));
		packet->header.crc = crc;

		// printf("LAF PACKET REQUEST\n");
		// hexdump((unsigned char*)packet, 128);
		// printf("\n");

		write((uint8_t*)packet, (sizeof(header) + command.size() + 1));

		if (!(responseSize = read(buffer, BUFFER_SIZE))) {
			return "No Response";
		}

		LafCommand* response = (LafCommand*)buffer;

		if (response->header.command == LAF_CMD_FAIL) {
			return "Command Failed";
		} else {
			for (int i = 0; i < response->header.size; i++) {
				lafCmdResponse << response->data[i];
			}
		}
	}

	// printf("LAF PACKET RESPONSE\n");
	// hexdump((unsigned char*)packet, 256);
	// printf("\n");

	// printf(lafCmdResponse.str().c_str());

	return lafCmdResponse.str();
}

/**
* @brief LafSerial::getProperty
*/
std::string LafSerial::getProperty(LafProperties property)
{
	LafCmdHeader header = {};
	header.command = LAF_CMD_INFO;
	header.arg0 = LAF_CMD_INFO_GPRO;
	header.size = LAF_INFO_SIZE;
	header.magic = LAF_INFO_MAGIC;

	// LafCmd* packet = (LafCmd*)buffer;
	// packet->header = header;

	// memcpy(packet->data, &property, sizeof(property));
	// packet->header.size = sizeof(property);
	// packet->header.crc = 0x000000;

	// uint16_t crc = crc16((char*)packet, (sizeof(packet)));
	// packet->header.crc = crc;

	// uint16_t crc = crc16((char*)&header, (sizeof(header)));
	header.crc = 0x00000917;

	

	printf("LAF PROP REQUEST\n");
	hexdump((unsigned char*)&header, 32);
	printf("\n");

	write((uint8_t*)&header, sizeof(header));

	// write((uint8_t*)packet, sizeof(packet));

	printf("LAF PROP RESPONSE\n");
	hexdump((unsigned char*)&header, 2048);
	printf("\n");

	return "TEST";
}

}