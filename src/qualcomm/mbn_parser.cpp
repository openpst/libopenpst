/**
* LICENSE PLACEHOLDER
*
* @file mbn_parser.cpp
* @class OpenPST::QC::MbnParser
* @package openpst/libopenpst
* @brief Reads and parses various MBN information from a binary on disk
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "qualcomm/mbn_parser.h"

using namespace OpenPST::QC;

Mbn::Mbn(std::string filePath, int parserFlags) : filePath(filePath), parserFlags(parserFlags)
{

}

Mbn::~Mbn()
{

}

const std::string& Mbn::getFilePath()
{
	return filePath;
}

int Mbn::getParserFlags()
{
	return parserFlags;
}

size_t Mbn::getFileSize()
{
	return fileSize;
}

void Mbn::setFileSize(size_t _fileSize)
{
	fileSize = _fileSize;
}

std::string& Mbn::getImageName()
{
	return imageName;
}

void Mbn::setImageName(std::string _imageName)
{
	imageName = _imageName;
}

uint32_t Mbn::getImageId()
{
	return imageId;
}

void Mbn::setImageId(uint32_t _imageId)
{
	imageId = _imageId;
}

uint32_t Mbn::getImageDestPtr()
{
	return imageDestPtr;
}

void Mbn::setImageDestPtr(uint32_t _imageDestPtr)
{
	imageDestPtr = _imageDestPtr;
}

uint32_t Mbn::getImageSize()
{
	return imageSize;
}

void Mbn::setImageSize(uint32_t _imageSize)
{
	imageSize = _imageSize;
}

uint32_t Mbn::getCodeSize()
{
	return codeSize;
}

void Mbn::setCodeSize(uint32_t _codeSize)
{
	codeSize = _codeSize;
}

uint32_t Mbn::getSignaturePtr()
{
	return signaturePtr;
}

void Mbn::setSignaturePtr(uint32_t _signaturePtr)
{
	signaturePtr = _signaturePtr;
}

uint32_t Mbn::getSignatureSize()
{
	return signatureSize;
}

void Mbn::setSignatureSize(uint32_t _signatureSize)
{
	signatureSize = _signatureSize;
}

uint32_t Mbn::getOemRootCertSelection()
{
	return oemRootCertSelection;
}

void Mbn::setOemRootCertSelection(uint32_t _oemRootCertSelection)
{
	oemRootCertSelection = _oemRootCertSelection;
}

uint32_t Mbn::getOemNumberOfRootCerts()
{
	return oemNumberOfRootCerts;
}

void Mbn::setOemNumberOfRootCerts(uint32_t _oemNumberOfRootCerts)
{
	oemNumberOfRootCerts = _oemNumberOfRootCerts;
}


std::vector<uint8_t>& Mbn::getHeader()
{
	return header;
}

std::vector<uint8_t>& Mbn::getCode()
{
	return code;
}

std::vector<uint8_t>& Mbn::getSignature()
{
	return signature;
}

std::vector<uint8_t>& Mbn::getX509()
{
	return x509;
}

void Mbn::setImageSrc(uint32_t _imageSrc)
{
	imageSrc = _imageSrc;
}

uint32_t Mbn::getImageSrc()
{
	return imageSrc;
}

void Mbn::setCertChainPtr(uint32_t _certChainPtr)
{
	certChainPtr = _certChainPtr;
}

uint32_t Mbn::getCertChainPtr()
{
	return certChainPtr;
}

void Mbn::setCertChainSize(uint32_t _certChainSize)
{
	certChainSize = _certChainSize;
}

uint32_t Mbn::getCertChainSize()
{
	return certChainSize;
}

MbnParser::MbnParser()
{

}

MbnParser::~MbnParser()
{

}

std::unique_ptr<Mbn> MbnParser::parse(std::string filePath, int flags)
{
	uint32_t codeword;
	uint32_t magic;
	std::stringstream ss;
	char headerBuff[MBN_HEADER_MAX_SIZE] = {};
	std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		ss << "Could Not Open File " << filePath;
		throw MbnParserException(ss.str());
	}

	std::unique_ptr<Mbn> ret(new Mbn(filePath, flags));

	file.seekg(0, file.end);

	ret->setFileSize((size_t)file.tellg());

	if (ret->getFileSize() < MBN_HEADER_MAX_SIZE) {
		ss << "Invalid File Type. File " << filePath << " is smaller than max header size";
		throw MbnParserException(ss.str());
	}

	// read codeword and magic
	file.seekg(0, file.beg);

	file.read(reinterpret_cast<char*>(&codeword), sizeof(codeword));
	file.read(reinterpret_cast<char*>(&magic), sizeof(magic));

	int headerSize = 0;

	if (codeword == 0x464C457F) {
		ss << "Invalid File Type. File " << filePath << " appears to be an elf binary";
		throw MbnParserException(ss.str());	
	} else if (codeword > kMbnImageLast && magic != MBN_EIGHTY_BYTE_MAGIC) {
		ss << "Invalid File Type. File " << filePath << " does not seem to be an mbn";
		throw MbnParserException(ss.str());		
	}

	bool is80ByteHeader = magic == MBN_EIGHTY_BYTE_MAGIC;
	
	file.seekg(0, file.beg);

	if (is80ByteHeader) {
		headerSize = sizeof(MbnHeader80byte);
		file.read(reinterpret_cast<char*>(headerBuff), headerSize);
	} else {
		// 40 byte has no magic		
		headerSize = sizeof(MbnHeader40byte);
		file.read(reinterpret_cast<char*>(headerBuff), headerSize);
	}

	ret->header.reserve(headerSize);

	ret->header.insert(
		ret->header.begin(), 
		headerBuff, 
		headerBuff + headerSize
	);

	if (is80ByteHeader) {
		MbnHeader80byte* header = (MbnHeader80byte*)&ret->header[0];
		ret->setImageId(header->image_id);
		ret->setImageName(getImageName(header->image_id));
		ret->setImageDestPtr(header->image_dest_ptr);
		ret->setImageSize(header->image_size);
		ret->setCodeSize(header->code_size);
		ret->setSignaturePtr(header->signature_ptr);
		ret->setSignatureSize(header->signature_size);
		ret->setOemRootCertSelection(header->oem_root_cert_sel);
		ret->setOemNumberOfRootCerts(header->oem_num_root_certs);
		ret->setImageSrc(header->image_src);
		ret->setCertChainPtr(header->cert_chain_ptr);
		ret->setCertChainSize(header->cert_chain_size);
	} else {
		MbnHeader40byte* header = (MbnHeader40byte*)&ret->header[0];
		ret->setImageId(header->image_id);
		ret->setImageName(getImageName(header->image_id));
		ret->setImageDestPtr(header->image_dest_ptr);
		ret->setImageSize(header->image_size);
		ret->setCodeSize(header->code_size);
		ret->setSignaturePtr(header->signature_ptr);
		ret->setSignatureSize(header->signature_size);
		ret->setOemRootCertSelection(0);
		ret->setOemNumberOfRootCerts(0);
		ret->setImageSrc(header->image_src);
		ret->setCertChainPtr(header->cert_chain_ptr);
		ret->setCertChainSize(header->cert_chain_size);
	}
	
	int offset, size;

	if (flags & kMbnParserFlagExtractCode){
		offset = ret->getImageSrc();
		size   = ret->getCodeSize();
		readSegment(file, offset, size, ret->getFileSize(), ret->code);
	}
	
	if (flags & kMbnParserFlagExtractSignature) {
		offset = ret->getImageSrc() + ret->getCodeSize();
		size   = ret->getSignatureSize();				
		readSegment(file, offset, size, ret->getFileSize(), ret->signature);
	}
	
	if (flags & kMbnParserFlagExtractX509) {
		offset 	= ret->getImageSrc() + ret->getCodeSize() + ret->getSignatureSize();
		size   	= ret->getCertChainSize();
		readSegment(file, offset, size, ret->getFileSize(), ret->x509);
	}

	file.close();

	return std::move(ret);
}

size_t MbnParser::readSegment(std::ifstream& file, int offset, int size, size_t maxSize, std::vector<uint8_t>& dest)
{
	if (size > maxSize) {
		std::stringstream ss;
		ss  << "Requested offset exceeds max size for segment at " << offset << " but max size is " << maxSize << std::endl;
		throw MbnParserException(ss.str());
	}

	char* tmp = new char[size];

	dest.reserve(size);

	file.seekg(offset, file.beg);

	file.read(tmp, size);

	dest.insert(dest.begin(), tmp, tmp + size);

	delete[] tmp;

	return dest.size();
}


std::string MbnParser::getImageName(uint32_t imageId)
{
	switch (imageId) {
		case kMbnImageNone:          return "None";
		case kMbnImageOemSbl:        return "OEM SBL";
		case kMbnImageAmss:          return "AMSS";
		case kMbnImageOcbl:          return "QCSBL";
		case kMbnImageHash:          return "Hash";
		case kMbnImageAppbl:         return "APPSBL";
		case kMbnImageHostDl:        return "HOSTDL";
		case kMbnImageDsp1:          return "DSP1";
		case kMbnImageFsbl:          return "FSBL";
		case kMbnImageDbl:           return "DBL";
		case kMbnImageOsbl:          return "OSBL";
		case kMbnImageDsp2:          return "DSP2";
		case kMbnImageEhostdl:       return "EHOSTDL";
		case kMbnImageNandprg:       return "NANDPRG";
		case kMbnImageNorprg:        return "NORPRG";
		case kMbnImageRamfs1:        return "RAMFS1";
		case kMbnImageRamfs2:        return "RAMFS2";
		case kMbnImageAdspQ5:        return "ADSP Q5";
		case kMbnImageAppsKernel:    return "APPS Kernel";
		case kMbnImageBackupRamfs:   return "Backup RAMFS";
		case kMbnImageSbl1:          return "SBL1";
		case kMbnImageSbl2:          return "SBL2";
		case kMbnImageRpm:           return "RPM";
		case kMbnImageSbl3:          return "SBL3";
		case kMbnImageTz:            return "TZ";
		case kMbnImageSsdKeys:       return "SSD Keys";
		case kMbnImageGen:           return "GEN";
		case kMbnImageDsp3:          return "DSP3";
		case kMbnImageAcdb:          return "ACDB";
		case kMbnImageWdt:           return "WDT";
		case kMbnImageMba:           return "MBA";
		default:                     return "Unknown";
	}
}
