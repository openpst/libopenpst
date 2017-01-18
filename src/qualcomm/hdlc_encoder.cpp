/**
* LICENSE PLACEHOLDER
*
* @file hdlc_encoder.cpp
* @package openpst/libopenpst
* @brief HDLC Encoding & Decoding
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "qualcomm/hdlc_encoder.h"

using namespace OpenPST::QC;

int HdlcEncoder::encode(uint8_t* data, size_t size, uint8_t** out, size_t &osize) {


	uint16_t crc    = crc16(reinterpret_cast<const char*>(data), size); // perform the crc on the original data
	uint8_t* buffer = new uint8_t[(size * 2) + HDLC_OVERHEAD_LENGTH](); // give enough room for everything to possibly be escaped
	
	osize = 0;

	buffer[osize++] = HDLC_CONTROL_CHAR; // start of frame
	
	for (int i = 0; i < (int)size; i++) {
		if (data[i] == HDLC_CONTROL_CHAR || data[i] == HDLC_ESC_CHAR) {
			buffer[osize++]   = HDLC_ESC_CHAR;
			buffer[osize++] = (data[i] == HDLC_ESC_CHAR ? HDLC_ESC_CHAR : HDLC_CONTROL_CHAR) ^ HDLC_ESC_MASK;
		} else {
			buffer[osize++] = data[i];
		}
	}

	// add in each byte of the crc, but account for the crc containing HDLC_CONTROL_CHAR or HDLC_ESC_CHAR
	uint8_t crcArray[2];	
	
	crcArray[0] = (uint8_t)(crc & 0xFF);
	crcArray[1] = (uint8_t)((crc >> 8) & 0xFF);

	for (int i = 0; i < 2; i++) {
		
		if (crcArray[i] == HDLC_CONTROL_CHAR || crcArray[i] == HDLC_ESC_CHAR) {
			buffer[osize++]   = HDLC_ESC_CHAR;
			buffer[osize++] = (crcArray[i] == HDLC_ESC_CHAR ? HDLC_ESC_CHAR : HDLC_CONTROL_CHAR) ^ HDLC_ESC_MASK;
		} else {
			buffer[osize++]   = crcArray[i];
		}
	}

	buffer[osize++] = HDLC_CONTROL_CHAR; // Add ending control character

	*out  = buffer;

	return 1;
}

int  HdlcEncoder::decode(uint8_t* data, size_t size, uint8_t** out, size_t& osize) {

	uint8_t* buffer = new uint8_t[size]();

	osize = 0;
	for (int i = 0; i < (int)size; i++) {
		if (data[i] == HDLC_CONTROL_CHAR && i == 0) {
			continue;		
		} else if (data[i] == HDLC_CONTROL_CHAR) {		
			break; // stop from reading into another message
		} else if (data[i] == HDLC_ESC_CHAR) {
			buffer[osize++] = data[i + 1] ^ HDLC_ESC_MASK;
			i++;
		} else {
			buffer[osize++] = data[i];
		}
	}

	*out = buffer;

	uint16_t crc = crc16(reinterpret_cast<const char*>(buffer), osize - sizeof(crc));
	uint16_t chk = *((uint16_t*)&buffer[osize - 2]);

	if (crc != chk) {
		printf("Invalid Response CRC Expected: %04X - Received: %04X  \n", crc, chk);
	} else {
		osize -= 2;
	}

	return 1;
}



int HdlcEncoder::encode(std::vector<uint8_t> &data) {

	uint16_t crc = crc16(reinterpret_cast<const char*>(&data[0]), data.size()); // perform the crc or the original data
	
	data.push_back(crc & 0xFF);
	data.push_back((crc >> 8) & 0xFF);

	int count = data.size();
	for (int i = 0; i < count; i++) {
		if (data[i] == HDLC_CONTROL_CHAR) {
			data[i] = HDLC_ESC_CHAR;
			data.insert(data.begin() + i + 1, HDLC_CONTROL_CHAR ^ HDLC_ESC_MASK);
			count++;
		} else if (data[i] == HDLC_ESC_CHAR) {
			data[i] = HDLC_ESC_CHAR;
			data.insert(data.begin() + i + 1, HDLC_ESC_CHAR ^ HDLC_ESC_MASK);
			count++;
		}
	}

	data.insert(data.begin(), HDLC_CONTROL_CHAR);
	data.push_back(HDLC_CONTROL_CHAR);

	return 0;
}

int HdlcEncoder::decode(std::vector<uint8_t> &data) {
	// unescape the data first
	int count = data.size();
	for (int i = 0; i < count; i++) {
		if (data[i] == HDLC_ESC_CHAR) {
			data[i] = data[i + 1] ^ HDLC_ESC_MASK;
			data.erase(data.begin() + i + 1);
			count--;
		}
	}

	// check the crc
	if (data[0] == HDLC_CONTROL_CHAR) {
		data.erase(data.begin());
	}

	uint16_t crc = crc16(reinterpret_cast<const char*>(&data[0]), data.size() - HDLC_TRAILER_LENGTH);
	uint16_t chk = *((uint16_t*)&data[data.size() - HDLC_TRAILER_LENGTH]);

	if (crc != chk) {
		printf("Invalid Response CRC Expected: %04X - Received: %04X\n", crc, chk);
	} else {
		data.erase(data.end() - HDLC_TRAILER_LENGTH, data.end());
	}	

	return 0;
}

uint16_t HdlcEncoder::crc16(const char *buffer, size_t len) {
	uint16_t crc = 0xffff;

	while (len--)
		crc = crc_table[(crc ^ *buffer++) & 0xff] ^ (crc >> 8);
	return ~crc;
}
