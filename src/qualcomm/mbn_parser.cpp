/**
* LICENSE PLACEHOLDER
*
* @file mbn_parser.cpp
* @class OpenPST::QC::MbnParser
* @package OpenPST
* @brief Reads and parses various MBN information from a binary on disk
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "qualcomm/mbn_parser.h"

using namespace OpenPST::QC;

MbnParser::MbnParser(std::string filePath) : filePath(filePath)
{
	std::stringstream ss;
	std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
				

	if (!file.is_open()) {
		ss << "Could Not Open File " << filePath;
		throw std::invalid_argument(ss.str());
	}

	file.seekg(0, file.end);
	fileSize = (size_t)file.tellg();
	file.seekg(0, file.beg);

	if (fileSize < sizeof(header.header80)) {
		ss << "Invalid File Type. File is smaller than header. " << filePath;
		throw std::invalid_argument(ss.str());
	}

	file.read(reinterpret_cast<char*>(&header.header80), sizeof(header.header80));
	file.close();

	header.is80byte = header.header80.magic == MBN_EIGHTY_BYTE_MAGIC;
}

MbnParser::~MbnParser()
{

}

const MbnHeader& MbnParser::getHeader()
{
	return header;
}

int MbnParser::getHeaderSize()
{
	return header.is80byte ? 
		sizeof(MbnHeader80byte) : sizeof(MbnHeader40byte);
}

std::string MbnParser::getImageName()
{
	if (header.is80byte) {
		return getImageName(header.header80.image_id);
	} else {
		return getImageName(header.header40.image_id);
	}
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