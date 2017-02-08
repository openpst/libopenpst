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

                StringHelper::toUpper(name);

                StringHelper::replaceAll(value, "NUM_DISK_SECTORS", std::to_string(numberOfSectors));

                if (requiresEvaluation(value)) {                
                    value = evaluator.evaluate(value);
                }

                if (name.compare("SECTOR_SIZE_IN_BYTES") == 0){
                    e.sectorSize = StringHelper::toInt<size_t>(value);
                } else if (name.compare("FILE_SECTOR_OFFSET") == 0) {
                     e.fileSectorOffset = StringHelper::toInt<off_t>(value);
                } else if (name.compare("FILENAME") == 0) {
                    e.fileName = value;
                } else if (name.compare("LABEL") == 0) {
                    e.label = value;
                } else if (name.compare("NUM_PARTITION_SECTORS") == 0) {
                    e.numPartitionSectors = StringHelper::toInt<int>(value);
                } else if (name.compare("PARTOFSINGLEIMAGE") == 0) {
                    StringHelper::toUpper(value);
                    e.partOfSingleImage = value.compare("TRUE") == 0;
                } else if (name.compare("PHYSICAL_PARTITION_NUMBER") == 0) {
                    e.physicalPartitionNumber = StringHelper::toInt<int>(value);
                } else if (name.compare("READBACKVERIFY") == 0) {
                    StringHelper::toUpper(value);
                    e.readBackVerify = value.compare("TRUE") == 0;
                } else if (name.compare("SIZE_IN_KB") == 0) {
                    e.sizeInKb = StringHelper::toFloat(value);
                    e.size     = static_cast<size_t>(e.sizeInKb * 1024);
                } else if (name.compare("SPARSE") == 0) {
                    StringHelper::toUpper(value);
                    e.sparse = value.compare("TRUE") == 0;
                } else if (name.compare("START_BYTE_HEX") == 0) {
                    e.startByte = StringHelper::toInt<size_t>(value, StringHelper::isHex(value));                    
                } else if (name.compare("START_SECTOR") == 0) {
                    e.startSector = StringHelper::toInt<size_t>(value);
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
