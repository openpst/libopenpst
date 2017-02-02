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

using namespace OpenPST::QC;

RawProgramXmlReader::RawProgramXmlReader()
{

}

RawProgramXmlReader::~RawProgramXmlReader()
{

}

/*

		/*

		SECTOR_SIZE_IN_BYTES
file_sector_offset
filename
label
num_partition_sectors
partofsingleimage
physical_partition_number
readbackverify
size_in_KB
sparse
start_byte_hex
  <program SECTOR_SIZE_IN_BYTES="512" file_sector_offset="0" filename="laf.img" label="laf" num_partition_sectors="65536" partofsingleimage="false" physical_partition_number="0" readbackverify="false" size_in_KB="32768.0" sparse="false" 
  start_byte_hex="0x13000000" start_sector="622592" />


*/
std::vector<RawProgramXmlEntry> RawProgramXmlReader::parse(const std::string& filePath)
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


    	ret.push_back(e);
    }

    return ret;
}