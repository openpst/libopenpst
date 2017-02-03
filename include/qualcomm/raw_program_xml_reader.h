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
#include "util/string_helper.h"
#include "util/math_string_evaluator.h"
#include "pugixml.hpp"

using OpenPST::StringHelper;
using OpenPST::MathStringEvaluator;

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
			size_t startByte;
			size_t startSector;
			size_t size;
			std::string fileName;
			std::string label;
		};

		class RawProgramXmlReader
		{
			protected:
				StringHelper stringHelper;
				MathStringEvaluator evaluator;

			public:
				RawProgramXmlReader();
				~RawProgramXmlReader();
				std::vector<RawProgramXmlEntry> parse(const std::string& filePath, int numberOfSectors); 
				
			protected:
				bool requiresEvaluation(const std::string& v);
		};
	}
}