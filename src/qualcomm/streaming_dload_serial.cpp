/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_serial.cpp
* @class StreamingDloadSerial
* @package OpenPST
* @brief Streaming DLOAD protocol serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "qualcomm/streaming_dload_serial.h"

using namespace OpenPST::QC;

/**
* @brief StreamingDloadSerial() - Constructor
*
* @param std::string port - The device to connect to
* @param int baudrate - Defaults to 115200
* @param serial::Timeout - Timeout, defaults to 1000ms
*/
StreamingDloadSerial::StreamingDloadSerial(std::string port, int baudrate, serial::Timeout timeout) :
	HdlcSerial(port, baudrate, timeout), 
	state({})
{
	state.hello.maxPreferredBlockSize = STREAMING_DLOAD_MAX_DATA_SIZE;
}

/**

* @brief ~StreamingDloadSerial() - Deconstructor
*/
StreamingDloadSerial::~StreamingDloadSerial()
{

}

StreamingDloadHelloResponse StreamingDloadSerial::sendHello(std::string magic, uint8_t version, uint8_t compatibleVersion, uint8_t featureBits)
{
	size_t rxSize; 
	StreamingDloadHelloRequest hello = {};
	StreamingDloadHelloResponse response;

	hello.command = kStreamingDloadHello;
	memcpy(hello.magic, magic.c_str(), magic.size());
	hello.version = version;
	hello.compatibleVersion = compatibleVersion;
	hello.featureBits = featureBits;

	write((uint8_t*)&hello, sizeof(hello));

	if (!(rxSize = read(reinterpret_cast<uint8_t*>(&response), sizeof(response)))){
		throw StreamingDloadSerialError("Device did not respond");
	}
   
	validateResponse(kStreamingDloadHelloResponse, reinterpret_cast<uint8_t*>(&response), rxSize);
			
	/*memcpy(&state.hello, &buffer[0], sizeof(StreamingDloadHelloResponseHeader));
	
	int dataStartIndex = sizeof(StreamingDloadHelloResponseHeader);

	// parse the packet and get the things that are not obvious without calculation
	// flashIdenfier, windowSize, numberOfSectors, sectorSizes, featureBits
	memcpy(&state.hello.flashIdenfier, &buffer[dataStartIndex], state.hello.flashIdLength);
	memcpy(&state.hello.windowSize, &buffer[dataStartIndex + state.hello.flashIdLength], sizeof(state.hello.windowSize));
	memcpy(&state.hello.numberOfSectors, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize)], sizeof(state.hello.numberOfSectors));

	int sectorSize = 4 * state.hello.numberOfSectors;
	memcpy(&state.hello.sectorSizes, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize) + sizeof(state.hello.numberOfSectors)], sectorSize-1);
	memcpy(&state.hello.featureBits, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize) + sizeof(state.hello.numberOfSectors) + sectorSize-1], sizeof(state.hello.featureBits));
	state.hello.featureBits = flip_endian16(state.hello.featureBits);*/

	return response;
}

bool StreamingDloadSerial::sendUnlock(std::string code)
{
	size_t rxSize;
	std::vector<uint8_t> buffer; 
	StreamingDloadUnlockRequest packet = {};
	
	packet.command = kStreamingDloadUnlock;
	packet.code = std::stoul(code.c_str(), nullptr, 16);
	
	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	if (!rxSize) {
		LOGE("Device did not respond\n");
		return kStreamingDloadIOError;
	}

	return isValidResponse(kStreamingDloadUnlocked, buffer);
}

bool StreamingDloadSerial::setSecurityMode(uint8_t mode)
{
	
	size_t rxSize;
	std::vector<uint8_t> buffer;
	StreamingDloadSecurityModeRequest packet;

	packet.command = kStreamingDloadSecurityMode;
	packet.mode = mode;

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	return isValidResponse(kStreamingDloadSecurityModeReceived, buffer);
}

void StreamingDloadSerial::sendNop()
{
	size_t rxSize;
	std::vector<uint8_t> buffer;
	StreamingDloadNopRequest packet = {};

	packet.command = kStreamingDloadNop;
	packet.identifier = std::rand();

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	validateResponse(kStreamingDloadNopResponse, buffer);

	if (reinterpret_cast<StreamingDloadNopResponse*>(&buffer[0])->identifier != packet.identifier) {
		LOGD("Received NOP response but identifier did not match\n");
	}
}

void StreamingDloadSerial::sendReset()
{   
	size_t rxSize;
	std::vector<uint8_t> buffer;    
	StreamingDloadResetRequest packet = {};

	packet.command = kStreamingDloadReset;

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	validateResponse(kStreamingDloadResetAck, buffer);
}

void StreamingDloadSerial::sendPowerOff()
{
	size_t rxSize;
	std::vector<uint8_t> buffer;
	StreamingDloadResetRequest packet = {};

	packet.command = kStreamingDloadPowerOff;

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	validateResponse(kStreamingDloadPoweringDown, buffer);
}

uint8_t StreamingDloadSerial::readEcc()
{   
	size_t rxSize;
	std::vector<uint8_t> buffer; 
	StreamingDloadGetEccStateRequest packet = {};

	packet.command = kStreamingDloadGetEccState;
	
	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	validateResponse(kStreamingDloadCurrentEccState, buffer);

	return reinterpret_cast<StreamingDloadGetEccStateResponse*>(&buffer[0])->status;
}

bool StreamingDloadSerial::setEcc(uint8_t status)
{
	size_t  rxSize;
	std::vector<uint8_t> buffer; 
	StreamingDloadSetEccStateRequest packet = {};

	packet.command = kStreamingDloadSetEcc;
	packet.status = status;

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	return isValidResponse(kStreamingDloadSetEccResponse, buffer);
}

bool StreamingDloadSerial::openMode(uint8_t mode)
{
	size_t rxSize;
	std::vector<uint8_t> buffer;    
	StreamingDloadOpenRequest packet = {};

	packet.command = kStreamingDloadOpen;
	packet.mode = mode;

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	return isValidResponse(kStreamingDloadOpened, buffer);
}

bool StreamingDloadSerial::closeMode()
{
	size_t rxSize;
	std::vector<uint8_t> buffer; 
	StreamingDloadCloseRequest packet = {};

	packet.command = kStreamingDloadClose;

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	return isValidResponse(kStreamingDloadClosed, buffer);
}

bool StreamingDloadSerial::openMultiImage(uint8_t imageType)
{
	size_t rxSize;
	std::vector<uint8_t> buffer;
	StreamingDloadOpenMultiImageRequest packet = {};

	packet.command = kStreamingDloadOpenMultiImage;
	packet.type = imageType;

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}

	return isValidResponse(kStreamingDloadOpenedMultiImage, buffer);
}

/**
* @brief readAddress - Read x bytes from starting address
*                      into a memory allocated array
*
* @param uint32_t address - The starting address
* @param size_t length - The length to read from address
* @param uint8_t** - The memory allocated array containing the read data until success or error encountered.
* @param size_t& - The size of the memory allocated data
* @param size_t stepSize - The amount to request per read operation. The max size is 1024.
*
* @return int
*//*
int StreamingDloadSerial::readAddress(uint32_t address, size_t length, uint8_t** data, size_t& dataSize, size_t stepSize)
{
	if (!isOpen()) {
		LOGE("Port Not Open\n");
		return kStreamingDloadIOError;
	}

	size_t txSize, rxSize;

	uint8_t* out = new uint8_t[length];
	size_t outSize = length;

	uint8_t buffer[STREAMING_DLOAD_MAX_RX_SIZE];

	dataSize = 0;

	StreamingDloadReadRequest packet;
	packet.command = kStreamingDloadRead;

	StreamingDloadReadResponse* readRx;

	if (stepSize > state.hello.maxPreferredBlockSize) {
		stepSize = state.hello.maxPreferredBlockSize;
	}

	do {
		packet.address = address + dataSize;
		packet.length = length <= stepSize ? length : stepSize;

		LOGE("Requesting %lu bytes from %08X\n", packet.length, packet.address);

		txSize = write((uint8_t*)&packet, sizeof(packet));

		if (!txSize) {
			LOGE("Wrote 0 bytes requesting to read %lu bytes from 0x%08X\n", packet.length, packet.address);
			return kStreamingDloadIOError;
		}

		rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

		if (!rxSize) {
			LOGE("Device did not respond\n");
			delete out;
			return kStreamingDloadIOError;
		}

		if (!isValidResponse(kStreamingDloadReadData, buffer, rxSize)) {
			LOGE("Invalid Response\n");
			delete out;
			return kStreamingDloadError;
		}
		
		size_t newSize = dataSize + rxSize;
		if (newSize > outSize) {             
			out = (uint8_t*)realloc(out, newSize);
			outSize = newSize;
		}

		readRx = (StreamingDloadReadResponse*)&buffer[0];

		if (readRx->address != packet.address) {
			LOGE("Packet address and response address differ\n");
			return kStreamingDloadError;
		}
		
		memcpy(&out[dataSize], readRx->data, rxSize - sizeof(packet));
		dataSize += (rxSize - sizeof(packet));
		
		if (length <= dataSize) {
			break; //done
		}
			
	} while (true);

	*data = out;
	
	LOGI("\n\n\nFinal Data Size: %lu bytes\n", dataSize);
	hexdump(out, dataSize);

	return kStreamingDloadSuccess;
}*/

/**
* @brief readAddress - Read x bytes from starting address
*                      into a std::vector<uint8_t> container
*
* @param uint32_t address - The starting address
* @param size_t length - The length to read from address
* @param std::vector<uint8_t> &out - The populated vector containing the read data until success or error encountered.
* @param size_t stepSize - The amount to request per read operation. The max size is 1024.
*
* @return int
*/
int StreamingDloadSerial::readAddress(uint32_t address, size_t length, std::vector<uint8_t> &out, size_t stepSize)
{
	if (!isOpen()) {
		LOGE("Port Not Open\n");
		return kStreamingDloadIOError;
	}

	size_t txSize, rxSize;

	StreamingDloadReadRequest packet = {};
	packet.command = kStreamingDloadRead;
	packet.address = address;

	StreamingDloadReadResponse* readRx;

	if (out.size()) {
		out.clear();
	}

	out.reserve(length);

	std::vector<uint8_t> tmp;
	tmp.reserve(packet.length + sizeof(packet));

	if (stepSize > state.hello.maxPreferredBlockSize) {
		stepSize = state.hello.maxPreferredBlockSize;
	}

	do {
		packet.address = packet.address + out.size();       
		packet.length = length <= stepSize ? length : stepSize;
		
		LOGE("Requesting %lu bytes from %08X\n", packet.length, packet.address);

		txSize = write((uint8_t*)&packet, sizeof(packet));
		
		if (!txSize) {
			LOGE("Wrote 0 bytes requesting to read %lu bytes from 0x%08X\n", packet.length, packet.address);
			return kStreamingDloadIOError;
		}

		// read accounting for additional room for the data to be read
		// and the extra packet data in the header of the response
		// and possibly escaped content
		rxSize = read(tmp, STREAMING_DLOAD_MAX_RX_SIZE);

		if (!rxSize) {
			LOGE("Device did not respond to request to read %lu bytes from 0x%08X\n", packet.length, packet.address);
			return kStreamingDloadIOError;
		}
		
		if (!isValidResponse(kStreamingDloadReadData, tmp)) {
			LOGD("Invalid response in request to read %lu bytes from 0x%08X. Data read so far is %lu bytes.\n", packet.length, packet.address, out.size());
			return kStreamingDloadError;
		}

		readRx = (StreamingDloadReadResponse*)&tmp[0];

		if (readRx->address != packet.address) {
			LOGE("Packet address and response address differ\n");
			return kStreamingDloadError;
		}

		// remove the command code, address, and length to only
		// keep the real data
		tmp.erase(tmp.end() - rxSize, (tmp.end() - rxSize) + sizeof(packet));

		out.reserve(out.size() + tmp.size());
		std::copy(tmp.begin(), tmp.end(), std::back_inserter(out));

		if (length <= out.size()) {
			LOGE("Final read size is %lu bytes\n", out.size());
			break; //done
		}

		// clear out the tmp buffer for next read
		tmp.clear();

	} while (true);

	return kStreamingDloadSuccess;
}


size_t StreamingDloadSerial::readAddress(uint32_t address, size_t length, std::ofstream& out, size_t stepSize)
{
	size_t rxSize;
	size_t outSize = 0;
	StreamingDloadReadRequest packet = {};

	packet.command = kStreamingDloadRead;
	packet.address = address;

	uint8_t tmp[STREAMING_DLOAD_MAX_RX_SIZE];

	if (stepSize > state.hello.maxPreferredBlockSize) {
		stepSize = state.hello.maxPreferredBlockSize;
	}

	do {
		packet.address = packet.address + outSize;
		packet.length = length <= stepSize ? length : stepSize;

		LOGE("Requesting %lu bytes from %08X\n", packet.length, packet.address);

		write((uint8_t*)&packet, sizeof(packet));

		if (!(rxSize = read(tmp, STREAMING_DLOAD_MAX_RX_SIZE))) {
			throw StreamingDloadSerialError("Device did not respond");
		}

		validateResponse(kStreamingDloadReadData, tmp, rxSize);

		StreamingDloadReadResponse* resp = reinterpret_cast<StreamingDloadReadResponse*>(&tmp);
	
		if (resp->address != packet.address) {
			throw StreamingDloadSerialError("Packet address and response address differ");
		}

		size_t dataSize = rxSize - (sizeof(resp->command) + sizeof(resp->address));

		out.write((char *)resp->data, dataSize);

		if (packet.length != dataSize) {
			std::stringstream ss;
			ss << "Data Written Does Not Match Requested Length. Requested " << packet.length << " and wrote " << dataSize << " bytes";
			throw StreamingDloadSerialError(ss.str());
		}

		outSize += dataSize;

	} while (outSize < length);

	LOGD("Final read size is %lu bytes\n", outSize);

	return outSize;
}

uint8_t StreamingDloadSerial::writePartitionTable(std::string fileName, bool overwrite)
{
	std::stringstream ss; 
	size_t fileSize;
	std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open()) {      
		ss << "Could Not Open File " << fileName;
		throw StreamingDloadSerialInvalidArgument(ss.str());
	}

	file.seekg(0, file.end);
	fileSize = (size_t)file.tellg();    
	file.seekg(0, file.beg);
	
	if (fileSize > 512) {
		LOGE("File can\'t exceed 512 bytes");
		file.close();
		return kStreamingDloadError;
	}

	size_t rxSize;
	std::vector<uint8_t> buffer;
	StreamingDloadPartitionTableRequest packet = {};

	packet.command = kStreamingDloadPartitionTable;
	packet.overrideExisting = overwrite;

	file.read((char *)&packet.data, fileSize);
	file.close();

	write((uint8_t*)&packet, sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))) {
		throw StreamingDloadSerialError("Device did not respond");
	}

	validateResponse(kStreamingDloadPartitionTableReceived, &buffer[0], rxSize);

	return reinterpret_cast<StreamingDloadPartitionTableResponse*>(&buffer[0])->status;    
}

size_t StreamingDloadSerial::streamWrite(uint32_t address, uint8_t* data, size_t dataSize, bool unframed)
{
	uint8_t packetBuffer[STREAMING_DLOAD_MAX_TX_SIZE] = {};
	uint8_t responseBuffer[STREAMING_DLOAD_MAX_RX_SIZE] = {};

	StreamingDloadStreamWriteRequest* packet = (StreamingDloadStreamWriteRequest*)packetBuffer;

	if (dataSize > state.hello.maxPreferredBlockSize) {

		size_t bytesWritten = 0;
		size_t dataSegmentSize = state.hello.maxPreferredBlockSize;
		

		do {
			packet->command = unframed ? kStreamingDloadUnframedStreamWrite : kStreamingDloadStreamWrite;
			packet->address = address + bytesWritten;

			if (dataSegmentSize > (dataSize - bytesWritten)) {
				dataSegmentSize = dataSize - bytesWritten;
			}

			memcpy(packet->data, &data[bytesWritten], dataSegmentSize);

			size_t txSize = write((uint8_t*)packet, sizeof(packet->command) + sizeof(packet->address) + dataSize, unframed);

			if (!txSize) {
				LOGE("Wrote 0 bytes\n");
				return kStreamingDloadIOError;
			}

			size_t rxSize = read(responseBuffer, STREAMING_DLOAD_MAX_RX_SIZE, unframed);

			if (!rxSize) {
				LOGE("Device did not respond\n");
				return kStreamingDloadIOError;
			}

			StreamingDloadStreamWriteResponse* response = (StreamingDloadStreamWriteResponse*) responseBuffer;

			if (response->address != packet->address) {
				LOGE("Response address %04X differs from requeasted write address %04X\n", response->address, packet->address);
				return kStreamingDloadError;
			}

			bytesWritten += dataSegmentSize;

		} while (bytesWritten < dataSize);

	} else {
		packet->command = unframed ? kStreamingDloadUnframedStreamWrite : kStreamingDloadStreamWrite;;
		packet->address = address;
		memcpy(packet->data, data, dataSize);

		size_t bytesWritten = write((uint8_t*)packet, sizeof(packet->command) + sizeof(packet->address) + dataSize, (!unframed));

		if (!bytesWritten) {
			LOGE("Wrote 0 bytes\n");
			return kStreamingDloadIOError;
		}

		size_t rxSize = read(responseBuffer, STREAMING_DLOAD_MAX_RX_SIZE, (!unframed));

		if (!rxSize) {
			LOGE("Device did not respond\n");
			return kStreamingDloadIOError;
		}

		if (!isValidResponse(unframed ? kStreamingDloadUnframedStreamWriteResponse : kStreamingDloadBlockWritten, responseBuffer, rxSize)) {
			return kStreamingDloadError;
		}

		StreamingDloadStreamWriteResponse* response = (StreamingDloadStreamWriteResponse*)responseBuffer;

		if (response->address != packet->address) {
			LOGE("Response address %04X differs from requested write address %04X\n", response->address, packet->address);
			return kStreamingDloadError;
		}

	}

	return kStreamingDloadSuccess;
}

bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, uint8_t* response, size_t responseSize)
{
	if (response[0] != expectedCommand) {
		if (response[0] == kStreamingDloadLog) {
			StreamingDloadLogResponse* packet = (StreamingDloadLogResponse*)response;
			LOGE("Received Log Response\n");
			memcpy((uint8_t*)&state.lastLog, packet, responseSize);
		} else if (response[0] == kStreamingDloadError) {
			StreamingDloadErrorResponse* packet = (StreamingDloadErrorResponse*)response;
			LOGE("Received Error Response %02X - %s\n", packet->code, getNamedError(packet->code).c_str());
			memcpy((uint8_t*)&state.lastError, packet, responseSize);
		} else {
			LOGE("Unexpected Response\n");
		}
		return false;
	}

	return true;
}

bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, std::vector<uint8_t> &data)
{
	return isValidResponse(expectedCommand, &data[0], data.size());
}

void StreamingDloadSerial::validateResponse(uint8_t expectedCommand, uint8_t* response, size_t responseSize)
{

}

void StreamingDloadSerial::validateResponse(uint8_t expectedCommand, std::vector<uint8_t> &response)
{

}

std::string StreamingDloadSerial::getNamedError(uint8_t code)
{
	switch (code) {
		case kStreamingDloadErrorInvalidDestinationAddress:     return "Invalid Destination Address";
		case kStreamingDloadErrorInvalidLength:                 return "Invalid Length";
		case kStreamingDloadErrorUnexpectedPacketEnd:           return "Unexpected End of Packet";
		case kStreamingDloadErrorInvalidCommand:                return "Invalid Command";
		case kStreamingDloadErrorWrongFlashIntelligentId:       return "Wrong Flash Intelligent ID";
		case kStreamingDloadErrorBadProgrammingVoltage:         return "Bad Programming Voltage";
		case kStreamingDloadErrorWriteVerifyFailed:             return "Write Verify Failed";
		case kStreamingDloadErrorIncorrectSecurityCode:         return "Incorrect Security Code";
		case kStreamingDloadErrorCannotPowerDown:               return "Cannot Power Down";
		case kStreamingDloadErrorNandProgrammingUnsupported:    return "NAND Flash Programming Not Supported";
		case kStreamingDloadErrorCommandOutOfSequence:          return "Command Out of Sequence";
		case kStreamingDloadErrorCloseDidNotSucceed:            return "Close Did Not Succeed";
		case kStreamingDloadErrorIncompatibleFeatureBits:       return "Incompatible Features Bits";
		case kStreamingDloadErrorOutOfSpace:                    return "Out of Space";
		case kStreamingDloadErrorInvalidSecurityMode:           return "Invalid Security Mode";
		case kStreamingDloadErrorMultiImageNandUnsupported:     return "Multi Image NAND Not Supported";
		case kStreamingDloadErrorPowerOffCmdUnsupported:        return "Power Off Command Not Supported";
		default:                                                return "Unknown";
	}
}

std::string StreamingDloadSerial::getNamedOpenMode(uint8_t mode)
{
	switch (mode) {
		case kStreamingDloadOpenModeBootloader:         return "Bootloader Download";
		case kStreamingDloadOpenModeBootableImage:      return "Bootable Image Download";
		case kStreamingDloadOpenModeCefs:               return "CEFS Image Download";
		default:                                        return "Unknown";
	}
}

std::string StreamingDloadSerial::getNamedMultiImage(uint8_t imageType)
{
	switch (imageType) {
		case kStreamingDloadOpenModeMultiNone:          return "None";
		case kStreamingDloadOpenModeMultiPbl:           return "Primary boot loader";
		case kStreamingDloadOpenModeMultiQcsblhHdCfg:   return "Qualcomm secondary boot loader header and config data";
		case kStreamingDloadOpenModeMultiQcsbl:         return "Qualcomm secondary boot loader";
		case kStreamingDloadOpenModeMultiOemsbl:        return "OEM secondary boot loader";
		case kStreamingDloadOpenModeMultiAmss:          return "AMSS modem executable";
		case kStreamingDloadOpenModeMultiApps:          return "AMSS applications executable";
		case kStreamingDloadOpenModeMultiObl:           return "MSM6250 OTP boot loader";
		case kStreamingDloadOpenModeMultiFotaui:        return "FOTA UI binary";
		case kStreamingDloadOpenModeMultiCefs:          return "Compact EFS2 image";
		case kStreamingDloadOpenModeMultiAppsBL:        return "AMSS applications boot loader";
		case kStreamingDloadOpenModeMultiApps_CEFS:     return "Apps CEFS image";
		case kStreamingDloadOpenModeMultiFlashBin:      return "Flash.bin for Windows Mobile";
		case kStreamingDloadOpenModeMultiDsp1:          return "DSP1 runtime image";
		case kStreamingDloadOpenModeMultiCustom:        return "User-defined partition";
		case kStreamingDloadOpenModeMultiDbl:           return "DBL image for Secure Boot 2.0";
		case kStreamingDloadOpenModeMultiOsbl:          return "OSBL image for Secure Boot 2.0";
		case kStreamingDloadOpenModeMultiFsbl:          return "FSBL image for Secure Boot 2.0";
		case kStreamingDloadOpenModeMultiDsp2:          return "DSP2 executable";
		case kStreamingDloadOpenModeMultiRaw:           return "Apps EFS2 raw image";
		case kStreamingDloadOpenModeMultiRofs1:         return "ROFS1 - Symbian";
		case kStreamingDloadOpenModeMultiRofs2:         return "ROFS2 - Symbian";
		case kStreamingDloadOpenModeMultiRofs3:         return "ROFS3 - Symbian";
		case kStreamingDloadOpenModeMultiEmmcUser:      return "EMMC USER partition";
		case kStreamingDloadOpenModeMultiEmmcBoot0:     return "EMMC BOOT0 partition";
		case kStreamingDloadOpenModeMultiEmmcBoot1:     return "EMMC BOOT1 partition";
		case kStreamingDloadOpenModeMultiRpmb:          return "EMMC RPMB";
		case kStreamingDloadOpenModeMultiGpp1:          return "EMMC GPP1";
		case kStreamingDloadOpenModeMultiGpp2:          return "EMMC GPP2";
		case kStreamingDloadOpenModeMultiGpp3:          return "EMMC GPP3";
		case kStreamingDloadOpenModeMultiGpp4:          return "EMMC GPP4";
		default:                                        return "Unknown";
	}
}