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
#include <sstream>

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
    	throw RawProgramXmlReaderError("Unable to load XML file");
    }

	pugi::xpath_query query_programs("/data/program");

    pugi::xpath_node_set programs = query_programs.evaluate_node_set(doc);

    for (auto &program : programs) {
    	RawProgramXmlEntry e;

        try {
            for (auto &attribute : program.node().attributes()) {
                std::string name(attribute.name());
                std::string value(attribute.value());

                stringHelper.toUpper(name);

                stringHelper.replaceAll(value, "NUM_DISK_SECTORS", std::to_string(numberOfSectors));

                if (requiresEvaluation(value)) {                
                    value = evaluator.evaluate(value);
                }

                if (name.compare("SECTOR_SIZE_IN_BYTES") == 0){
                    e.sectorSize = stringHelper.toInt<size_t>(value);
                } else if (name.compare("FILE_SECTOR_OFFSET") == 0) {
                     e.fileSectorOffset = stringHelper.toInt<off_t>(value);
                } else if (name.compare("FILENAME") == 0) {
                    e.fileName = value;
                } else if (name.compare("LABEL") == 0) {
                    e.label = value;
                } else if (name.compare("NUM_PARTITION_SECTORS") == 0) {
                    e.numPartitionSectors = stringHelper.toInt<int>(value);
                } else if (name.compare("PARTOFSINGLEIMAGE") == 0) {
                    stringHelper.toUpper(value);
                    e.partOfSingleImage = value.compare("TRUE") == 0;
                } else if (name.compare("PHYSICAL_PARTITION_NUMBER") == 0) {
                    e.physicalPartitionNumber = stringHelper.toInt<int>(value);
                } else if (name.compare("READBACKVERIFY") == 0) {
                    stringHelper.toUpper(value);
                    e.readBackVerify = value.compare("TRUE") == 0;
                } else if (name.compare("SIZE_IN_KB") == 0) {
                    e.sizeInKb = stringHelper.toFloat(value);
                    e.size     = static_cast<size_t>(e.sizeInKb * 1024);
                } else if (name.compare("SPARSE") == 0) {
                    stringHelper.toUpper(value);
                    e.sparse = value.compare("TRUE") == 0;
                } else if (name.compare("START_BYTE_HEX") == 0) {
                    e.startByte = stringHelper.toInt<size_t>(value, stringHelper.isHex(value));                    
                } else if (name.compare("START_SECTOR") == 0) {
                    e.startSector = stringHelper.toInt<size_t>(value);
                }
            }

        } catch (MathStringEvaluatorError& e) {
            std::stringstream ss;
            ss << "Error evaluating math expression: " << e.what();
            throw RawProgramXmlReaderError(ss.str());
        } catch (std::out_of_range& e) {
            std::stringstream ss;
            ss << "A numeric value failed conversion: " << e.what();
            throw RawProgramXmlReaderError(ss.str());
        } catch (...) {
            throw RawProgramXmlReaderError("Unhandled Exception Encountered");
        }

    	ret.push_back(e);
    }

    return ret;
}

bool RawProgramXmlReader::requiresEvaluation(const std::string& v) {   
    for (auto &c : v) {
    	if (c == '+' || c == '-' || c == '*' || c == '/') {
            return true;
        }
    }
    return false;
}
