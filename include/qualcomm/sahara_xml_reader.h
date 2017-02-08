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
* @file sahara_xml_reader.h
* @class SaharaXmlReader
* @package openpst/libopenpst
* @brief Parses a sahara.xml file format for use with sahara automation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include <vector>
#include "definitions.h"
#include "util/string_helper.h"
#include "pugixml.hpp"

using OpenPST::StringHelper;

namespace OpenPST {
	namespace QC {
		
		struct SaharaXmlEntry {
			uint32_t imageId;
			bool programmer;
			std::string imagePath;
		};

		class SaharaXmlReader
		{
			public:
				SaharaXmlReader();
				~SaharaXmlReader();
				std::vector<SaharaXmlEntry> parse(const std::string& filePath); 
		};

		class SaharaXmlReaderError : public std::exception
		{
			const SaharaXmlReaderError& operator=(SaharaXmlReaderError);
			std::string _what;
			public:
				SaharaXmlReaderError(std::string message) : _what(message)  { }
				SaharaXmlReaderError(const SaharaXmlReaderError& second) : _what(second._what) {}
				virtual ~SaharaXmlReaderError() throw() {}
				virtual const char* what() const throw () {
					return _what.c_str();
				}

		};
	}
}