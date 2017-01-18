/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_serial.cpp
* @class StreamingDloadSerial
* @package openpst/libopenpst
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
StreamingDloadSerial::StreamingDloadSerial(std::string port, int baudrate, int timeout) :
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
	StreamingDloadHelloRequest  hello = {};
	std::vector<uint8_t> buffer; 

	hello.command 			= kStreamingDloadHello;
	hello.version 			= version;
	hello.compatibleVersion = compatibleVersion;
	hello.featureBits 		= featureBits;

	memcpy(hello.magic, magic.c_str(), magic.size());

	write((uint8_t*)&hello, sizeof(hello));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))){
		throw StreamingDloadSerialError("Device did not respond");
	}
   	
	validateResponse(kStreamingDloadHelloResponse, reinterpret_cast<uint8_t*>(&buffer[0]), rxSize);
			
	memcpy(&state.hello, &buffer[0], sizeof(StreamingDloadHelloResponseHeader));
	
	int dataStartIndex = sizeof(StreamingDloadHelloResponseHeader);

	// parse the packet and get the things that are not obvious without calculation
	// flashIdenfier, windowSize, numberOfSectors, sectorSizes, featureBits
	memcpy(&state.hello.flashIdenfier, &buffer[dataStartIndex], state.hello.flashIdLength);
	memcpy(&state.hello.windowSize, &buffer[dataStartIndex + state.hello.flashIdLength], sizeof(state.hello.windowSize));
	memcpy(&state.hello.numberOfSectors, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize)], sizeof(state.hello.numberOfSectors));

	int sectorSize = 4 * state.hello.numberOfSectors;
	memcpy(&state.hello.sectorSizes, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize) + sizeof(state.hello.numberOfSectors)], sectorSize-1);
	memcpy(&state.hello.featureBits, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize) + sizeof(state.hello.numberOfSectors) + sectorSize-1], sizeof(state.hello.featureBits));
	state.hello.featureBits = flip_endian16(state.hello.featureBits);

	state.negotiated = true;

	return state.hello;
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
* @param size_t amount - The amount to read from address
* @param std::vector<uint8_t> &out - The populated vector containing the read data until success or error encountered.
*
* @return size_t
*/
size_t StreamingDloadSerial::readFlash(uint32_t address, size_t amount, std::vector<uint8_t> &out)
{
	size_t rx;
	size_t total = 0;
	size_t step = amount;
	std::vector<uint8_t> temp;
	StreamingDloadReadRequest packet = {};	

	out.reserve(amount);	
	temp.reserve(STREAMING_DLOAD_MAX_RX_SIZE);

	if (step > state.hello.maxPreferredBlockSize) {
		step = state.hello.maxPreferredBlockSize;
	}

	while (total < amount) {
		
		if ((amount - total) < step) {
			step = amount - total;
		}

		packet.command = kStreamingDloadRead;
		packet.address = address + total;
		packet.length  = step;

		LOGD("Requesting %lu bytes from %08X\n", packet.length, packet.address);
		
		write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

		if (!(rx = read(temp, STREAMING_DLOAD_MAX_RX_SIZE))) {
			throw StreamingDloadSerialError("Device did not respond");
		}

		validateResponse(kStreamingDloadReadData, temp);
		
		if (reinterpret_cast<StreamingDloadReadResponse*>(&temp[0])->address != packet.address) {			
			throw StreamingDloadSerialError("Packet address and response address differ");
		}

		// remove the command code, address, and length to only
		// keep the real data
		temp.erase(temp.end() - rx, ((temp.end() - rx) + sizeof(packet.command) + sizeof(packet.address)));

		out.reserve(out.size() + temp.size());

		std::copy(temp.begin(), temp.end(), std::back_inserter(out));

		total += temp.size();

		temp.clear();
	    		
	}

	return total;
}


size_t StreamingDloadSerial::readFlash(uint32_t address, size_t amount, std::ofstream& out)
{
	size_t total  	  = 0;
	size_t step   	  = amount;
	std::vector<uint8_t> temp;

	if (state.negotiated && state.hello.maxPreferredBlockSize && step > state.hello.maxPreferredBlockSize) {
		step = state.hello.maxPreferredBlockSize;
	} else if (step > STREAMING_DLOAD_MAX_DATA_SIZE) {
		step = STREAMING_DLOAD_MAX_DATA_SIZE;
	}

	temp.reserve(STREAMING_DLOAD_MAX_RX_SIZE);
	
	while (total < amount) {
		
		if ((amount - total) < step) {
			step = amount - total;
		}

		readFlash(address+total, step, temp);

		out.write(reinterpret_cast<char *>(&temp[0]), temp.size());

		total += temp.size();

		temp.clear();
		
	}

	return total;
}

uint8_t StreamingDloadSerial::writePartitionTable(std::string fileName, bool overwrite)
{
	std::stringstream ss; 
	size_t fileSize;
	std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open()) {      
		ss << "Could Not Open File " << fileName;
		throw StreamingDloadSerialError(ss.str());
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

	file.read(reinterpret_cast<char *>(&packet.data), fileSize);

	file.close();

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (!(rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE))) {
		throw StreamingDloadSerialError("Device did not respond");
	}

	validateResponse(kStreamingDloadPartitionTableReceived, &buffer[0], rxSize);

	return reinterpret_cast<StreamingDloadPartitionTableResponse*>(&buffer[0])->status;    
}

size_t StreamingDloadSerial::writeFlash(uint32_t address, uint8_t* data, size_t length, bool unframed)
{
	size_t step = length;
	size_t written = 0;
	size_t rx = 0;
	StreamingDloadStreamWriteRequest* request = nullptr;
	uint8_t readbuff[STREAMING_DLOAD_MAX_RX_SIZE];
	StreamingDloadStreamWriteResponse* response = reinterpret_cast<StreamingDloadStreamWriteResponse*>(&readbuff);

	if (data == nullptr || !length) {
		throw StreamingDloadSerialError("No data to write");
	}

	if (state.negotiated && state.hello.maxPreferredBlockSize && step > state.hello.maxPreferredBlockSize) {
		step = state.hello.maxPreferredBlockSize;
	} else if (step > STREAMING_DLOAD_MAX_DATA_SIZE) {
		step = STREAMING_DLOAD_MAX_DATA_SIZE;
	}

	request = reinterpret_cast<StreamingDloadStreamWriteRequest*>(new uint8_t[sizeof(StreamingDloadStreamWriteRequest) + step]);

	if (!request) {
		throw StreamingDloadSerialError("Error allocating memory for write");
	}

	while (written < length) {
		request->command = unframed ? kStreamingDloadUnframedStreamWrite : kStreamingDloadStreamWrite;
		request->address = address + written;

		if ((length - written) < step) {
			step = length - written;
		}
		
		std::copy((data + written), (data + written + step), request->data);

		write(reinterpret_cast<uint8_t*>(request), step, (unframed ? false : true));

		if (!(rx = read(readbuff, sizeof(readbuff)))) {
			delete[] request;
			throw StreamingDloadSerialError("Device did not respond");
		}
			
		validateResponse(unframed ? kStreamingDloadUnframedStreamWriteResponse : kStreamingDloadBlockWritten, reinterpret_cast<uint8_t*>(&readbuff), rx);
		
		if (request->address != response->address) {	
			delete[] request;
			throw StreamingDloadSerialError("Response address differs from requeasted write address");
		}

		written += step;
	}
	
	delete[] request;

	return written;
}

bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, uint8_t* response, size_t responseSize)
{
	std::stringstream ss;

	if (responseSize <= 0) {
		return false;
	}

	if (response[0] != expectedCommand) {
		if (response[0] == kStreamingDloadLog) {
			StreamingDloadLogResponse* packet = (StreamingDloadLogResponse*)response;
			if (responseSize == sizeof(state.lastLog)) {
				memcpy(reinterpret_cast<uint8_t*>(&state.lastLog), packet, sizeof(state.lastLog));
			} else {
				LOGE("Response size doesnt match log response size\n");
			}	
			return false;
		} else if (response[0] == kStreamingDloadError) {
			StreamingDloadErrorResponse* packet = (StreamingDloadErrorResponse*)response;	
			if (responseSize == sizeof(state.lastError)) {
				memcpy(reinterpret_cast<uint8_t*>(&state.lastError), packet, sizeof(state.lastError));
			} else {
				LOGE("Response size doesnt match error response size\n");
			}
			return false;

		} else {
			return false;
		}
	}
	return true;
}

bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, std::vector<uint8_t> &data)
{
	return isValidResponse(expectedCommand, &data[0], data.size());
}

void StreamingDloadSerial::validateResponse(uint8_t expectedCommand, uint8_t* response, size_t responseSize)
{
	std::stringstream ss;

	if (responseSize <= 0) {
		throw StreamingDloadSerialError("No Response");
	}

	if (response[0] != expectedCommand) {
		if (response[0] == kStreamingDloadLog) {
			StreamingDloadLogResponse* packet = (StreamingDloadLogResponse*)response;
			if (responseSize == sizeof(state.lastLog)) {
				memcpy(reinterpret_cast<uint8_t*>(&state.lastLog), packet, sizeof(state.lastLog));
			} else {
				LOGE("Response size doesnt match log response size %li %li\n", sizeof(state.lastLog), responseSize);
			}

			ss << "Received Log Response: " << (char*)&packet->text;
			throw StreamingDloadSerialError(ss.str());
		} else if (response[0] == kStreamingDloadError) {
			StreamingDloadErrorResponse* packet = (StreamingDloadErrorResponse*)response;	
			if (responseSize == sizeof(state.lastError)) {
				memcpy(reinterpret_cast<uint8_t*>(&state.lastError), packet, sizeof(state.lastError));
			} else {
				LOGE("Response size doesnt match error response size %li %li\n", sizeof(state.lastError), responseSize);
			}
			ss << "Received Error Response: " << getNamedError(packet->code);
			throw StreamingDloadSerialError(ss.str(), packet->code);

		} else {
			throw StreamingDloadSerialError("Unexpected Response");
		}
	}
}

void StreamingDloadSerial::validateResponse(uint8_t expectedCommand, std::vector<uint8_t> &response)
{
	return validateResponse(expectedCommand, &response[0], response.size());
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

void StreamingDloadSerial::close()
{
	try {
		GenericSerial::close();
		state = {};
	} catch (...) {
		state = {};
	} 
}