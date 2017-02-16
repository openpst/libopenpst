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
* @file hdlc_encoder.cpp
* @package openpst/libopenpst
* @brief HDLC Encoding & Decoding
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "qualcomm/hdlc_encoder.h"

using namespace OpenPST::QC;

size_t HdlcEncoder::encode(uint8_t* data, size_t size, uint8_t** out) {

	size_t osize 	= 0;
	uint16_t crc    = crc16(reinterpret_cast<const char*>(data), size); // perform the crc on the original data
	
	if (!size) {
		*out = nullptr;
		return 0;
	}

	uint8_t* buffer = new uint8_t[(size * 2) + HDLC_OVERHEAD_LENGTH](); // give enough room for everything to possibly be escaped
	
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

	return osize;
}

size_t HdlcEncoder::decode(uint8_t* data, size_t size, uint8_t** out) {

	size_t osize = 0;

	if (!size) {
		*out = nullptr;
		return 0;
	}

	uint8_t* buffer = new uint8_t[size]();
	
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

	return osize;
}



size_t HdlcEncoder::encode(std::vector<uint8_t> &data) {

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

	return data.size();
}

size_t HdlcEncoder::decode(std::vector<uint8_t> &data) {
	// unescape the data first
	int count = data.size();
	for (int i = 0; i < count; i++) {
		if (data[i] == HDLC_ESC_CHAR) {
			data[i] = data[i + 1] ^ HDLC_ESC_MASK;
			data.erase(data.begin() + i + 1);
			count--;
		}
	}

	// remove begin frame character if it exists
	if (data[0] == HDLC_CONTROL_CHAR) {
		data.erase(data.begin());
	}

	// check the crc

	int trailer = (*data.end() == HDLC_CONTROL_CHAR) ? HDLC_TRAILER_LENGTH : (HDLC_TRAILER_LENGTH - 1) ;

	uint16_t crc = crc16(reinterpret_cast<const char*>(&data[0]), data.size() - trailer);
	uint16_t chk = *((uint16_t*)&data[data.size() - trailer]);

	if (crc != chk) {
		printf("Invalid Response CRC Expected: %04X - Received: %04X\n", crc, chk);
	} else {	
		data.erase(data.end() - trailer, data.end());	
	}	

	return data.size();
}

uint16_t HdlcEncoder::crc16(const char *buffer, size_t len) {
	uint16_t crc = 0xffff;

	while (len--)
		crc = crc_table[(crc ^ *buffer++) & 0xff] ^ (crc >> 8);
	return ~crc;
}
