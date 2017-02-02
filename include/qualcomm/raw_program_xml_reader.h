/**
* LICENSE PLACEHOLDER
*
* @file raw_program_xml_reader.h
* @class RawProgramXmlReader
* @package openpst/libopenpst
* @brief Parses a rawprogram0.xml file format for use with streaming dload automation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include <vector>
#include "definitions.h"
#include "pugixml.hpp"

namespace OpenPST {
	namespace QC {

		struct RawProgramXmlEntry {
			size_t sectorSize;
			off_t fileSectorOffset;
			int numPartitionSectors;
			bool partOfSingleImage;
			int physicalPartitionNumber;
			bool readBackVerify;
			bool sparse;
			off_t startByte;
			off_t startSector;
			std::string fileName;
			std::string label;
		};

		class RawProgramXmlReader
		{
			public:
				RawProgramXmlReader();
				~RawProgramXmlReader();
				std::vector<RawProgramXmlEntry> parse(const std::string& filePath); 
		};
	}
}