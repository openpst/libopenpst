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
using OpenPST::MathStringEvaluatorError;

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
			float sizeInKb;
			std::string fileName;
			std::string label;
		};

		class RawProgramXmlReader
		{
			protected:
				MathStringEvaluator evaluator;

			public:
				RawProgramXmlReader();
				~RawProgramXmlReader();
				std::vector<RawProgramXmlEntry> parse(const std::string& filePath, int numberOfSectors); 
				
			protected:
				bool requiresEvaluation(const std::string& v);
		};

		class RawProgramXmlReaderError : public std::exception
		{
			const RawProgramXmlReaderError& operator=(RawProgramXmlReaderError);
			std::string _what;
			public:
				RawProgramXmlReaderError(std::string message) : _what(message)  { }
				RawProgramXmlReaderError(const RawProgramXmlReaderError& second) : _what(second._what) {}
				virtual ~RawProgramXmlReaderError() throw() {}
				virtual const char* what() const throw () {
					return _what.c_str();
				}

		};	
	}
}