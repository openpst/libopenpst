/**
* LICENSE PLACEHOLDER
*
* @file raw_program_xml_reader.cpp
* @class RawProgramXmlReader
* @package openpst/libopenpst
* @brief Parses a rawprogram0.xml file format for use with streaming dload automation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "qualcomm/raw_program_xml_reader.h"
#include <iostream>
#include <string>

using namespace OpenPST::QC;

RawProgramXmlReader::RawProgramXmlReader()
{

}

RawProgramXmlReader::~RawProgramXmlReader()
{

}

uint32_t toUnsigned(const std::string& src)
{
    return std::strtoul(src.c_str(), nullptr, 10);
}

std::vector<RawProgramXmlEntry> RawProgramXmlReader::parse(const std::string& filePath, int numberOfSectors)
{
	pugi::xml_document doc;
	std::vector<RawProgramXmlEntry> ret;

    if (!doc.load_file(filePath.c_str())) {
    	throw std::invalid_argument("Unable to load XML file");
    }

	pugi::xpath_query query_programs("/data/program");

    pugi::xpath_node_set programs = query_programs.evaluate_node_set(doc);

    for (auto &program : programs) {
    	RawProgramXmlEntry e;

        for (auto &attribute : program.node().attributes()) {
            std::string name(attribute.name());
            std::string value(attribute.value());

            stringHelper.toUpper(name);

            stringHelper.replaceAll(value, "NUM_DISK_SECTORS", std::to_string(numberOfSectors));

            if (requiresEvaluation(value)) {                
                value = evaluator.evaluate(value);
            }

            if (name.compare("SECTOR_SIZE_IN_BYTES") == 0){
                e.sectorSize = toUnsigned(value);
            } else if (name.compare("FILE_SECTOR_OFFSET") == 0) {
                 e.fileSectorOffset = toUnsigned(value);
            } else if (name.compare("FILENAME") == 0) {
                e.fileName = value;
            } else if (name.compare("LABEL") == 0) {
                e.label = value;
            } else if (name.compare("NUM_PARTITION_SECTORS") == 0) {
                e.numPartitionSectors = toUnsigned(value);
            } else if (name.compare("PARTOFSINGLEIMAGE") == 0) {
                stringHelper.toUpper(value);
                e.partOfSingleImage = value.compare("TRUE") == 0;
            } else if (name.compare("PHYSICAL_PARTITION_NUMBER") == 0) {
                e.physicalPartitionNumber = toUnsigned(value);
            } else if (name.compare("READBACKVERIFY") == 0) {
                stringHelper.toUpper(value);
                e.readBackVerify = value.compare("TRUE") == 0;
            } else if (name.compare("SIZE_IN_KB") == 0) {
                e.size = toUnsigned(value);
                e.size = e.size * 1024;
            } else if (name.compare("SPARSE") == 0) {
                stringHelper.toUpper(value);
                e.sparse = value.compare("TRUE") == 0;
            } else if (name.compare("START_BYTE_HEX") == 0) {
                e.startByte = toUnsigned(value);
            } else if (name.compare("START_SECTOR") == 0) {
                e.startSector = toUnsigned(value);
            }
        }

    	ret.push_back(e);
    }

    return ret;
}

bool RawProgramXmlReader::requiresEvaluation(const std::string& v) {
    std::string c = v;
    
    stringHelper.toUpper(c);
    
    if (c.find("NUM_DISK_SECTORS") != std::string::npos) {
        return true;
    } else if (
        c.find("*") != std::string::npos ||
        c.find("/") != std::string::npos || 
        c.find("+") != std::string::npos ||
        c.find("-") != std::string::npos) {
        return true;
    }
    return false;
}
