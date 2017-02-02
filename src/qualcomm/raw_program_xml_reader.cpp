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
            std::string evaluated;

            stringHelper.toUpper(name);
            
            if (requiresEvaluation(value)) {
                stringHelper.replaceAll(value, "NUM_DISK_SECTORS", std::to_string(numberOfSectors));
                value = evaluate(value);
            }

            if (name.compare("SECTOR_SIZE_IN_BYTES") == 0){
                e.sectorSize = std::strtoul(value.c_str(), nullptr, 10);
            } else if (name.compare("FILE_SECTOR_OFFSET") == 0) {
                 e.fileSectorOffset = std::strtoul(value.c_str(), nullptr, 10);
            } else if (name.compare("FILENAME") == 0) {
                e.fileName = value;
            } else if (name.compare("LABEL") == 0) {
                e.label = value;
            } else if (name.compare("NUM_PARTITION_SECTORS") == 0) {
                e.numPartitionSectors = std::strtoul(value.c_str(), nullptr, 10);
            } else if (name.compare("PARTOFSINGLEIMAGE") == 0) {
                stringHelper.toUpper(value);
                e.partOfSingleImage = value.compare("TRUE") == 0;
            } else if (name.compare("PHYSICAL_PARTITION_NUMBER") == 0) {
                e.physicalPartitionNumber = std::strtoul(value.c_str(), nullptr, 10);
            } else if (name.compare("READBACKVERIFY") == 0) {
                stringHelper.toUpper(value);
                e.readBackVerify = value.compare("TRUE") == 0;
            } else if (name.compare("SIZE_IN_KB") == 0) {
                e.size = std::strtoul(value.c_str(), nullptr, 10);
                e.size = e.size * 1024;
            } else if (name.compare("SPARSE") == 0) {
                stringHelper.toUpper(value);
                e.sparse = value.compare("TRUE") == 0;
            } else if (name.compare("START_BYTE_HEX") == 0) {
                e.startByte = std::strtoul(value.c_str(), nullptr, 16);
            } else if (name.compare("START_SECTOR") == 0) {
                e.startSector = std::strtoul(value.c_str(), nullptr, 10);
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

std::string RawProgramXmlReader::evaluate(const std::string& expr)
{
    return evaluator.evaluate(expr);
}

std::string RawProgramXmlReader::readNextToken(std::string::iterator it)
{

}